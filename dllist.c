#include "dllist.h"
#include <stdio.h>
#include <assert.h>

void initList(DLList* list)
{
  list->head = list->tail = NULL;
  pthread_mutex_init(&list->mtx,NULL);
}
void clearList(DLList* list)
{
  DLNode* cur = list->head;
  while (cur) {
    DLNode* next = cur->next;
    pthread_mutex_destroy(&cur->mtx);
    free(cur);
    cur = next;
  }
  list->head = list->tail = NULL;
}
void destroyList(DLList* list)
{
  clearList(list);
  pthread_mutex_destroy(&list->mtx);
}

void insertSortedList(DLList* list,int v)
{
  DLNode* newNode = malloc(sizeof(DLNode));
  newNode->next = NULL;
  newNode->prv = NULL;
  newNode->value = v;
  pthread_mutex_init(&newNode->mtx,NULL);

  /* CRITICAL SECTION START */
  pthread_mutex_lock(&list->mtx);
  if (list->head==NULL) {
    list->head = newNode;
    pthread_mutex_unlock(&list->mtx);
    return;
  }
  else if (list->tail==NULL) {
    if (list->head->value < newNode->value) {
      list->head->next = newNode;
      newNode->prv = list->head;
      list->tail = newNode;
      pthread_mutex_unlock(&list->mtx);
      return;
    }
    /* else if (list->head->value == newNode->value) { */
    /*   free(newNode); */
    /*   pthread_mutex_unlock(&list->mtx); */
    /*   return; // no dupes */
    /* } */

    else {
      DLNode* oldHead = list->head;
      oldHead->prv = newNode;
      newNode->next = oldHead;
      list->head = newNode;
      list->tail = oldHead;
      pthread_mutex_unlock(&list->mtx);
      return;
    }
  }
  
  DLNode* cur = list->head;
  pthread_mutex_t* last = &list->mtx;
  while (cur && cur->value <= newNode->value) {
    /* if (cur->value == newNode->value) { */
    /*   free(newNode); */
    /*   pthread_mutex_unlock(last); */
    /*   return; */
    /* } no dupes!*/
    pthread_mutex_lock(&cur->mtx);
    pthread_mutex_unlock(last);
    last = &cur->mtx;
    cur = cur->next;
  }

  if (cur) { // cur->val >= newNode->val
    newNode->next = cur;
    newNode->prv = cur->prv;
    cur->prv->next = newNode;
    cur->prv = newNode;
    pthread_mutex_unlock(last);
  }
  else { // add to end of dll
    list->tail->next = newNode;
    newNode->prv = list->tail;
    list->tail = newNode;
    pthread_mutex_unlock(last);
  }
  return;
  /* TODO: Iterate through the list, find the correct place to insert
     the integer v. Each node has a mutex; use those mutexes to
     coordinate the threads. */
}

void printListForward(DLList* list)
{
  pthread_mutex_lock(&list->mtx);
  DLNode* cur = list->head;
  pthread_mutex_t* last = &list->mtx;
  while (cur) {
    pthread_mutex_lock(&cur->mtx);
    printf("%d ",cur->value);
    pthread_mutex_unlock(last);
    last = &cur->mtx;
    cur = cur->next;
  }
  printf("\n");
  pthread_mutex_unlock(last);
}
