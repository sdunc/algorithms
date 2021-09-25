#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct TNode{
  char* val;
  struct TNode* left;
  struct TNode* right;
} TNode;

TNode* makeEmptyTree()
{
  return NULL;
}

TNode* insertIntoTree(TNode* root,char* string)
{
  /* If the root is NULL create a new TNode, copy the string into it,
     and return the pointer to the new TNode.  */
  if (root == NULL) {
    TNode* newNode = (TNode*)malloc(sizeof(TNode));
    char* newStr = (char*)malloc(sizeof(char)*strlen(string)+1);
    strcpy(newStr, string);
    newNode->val = newStr;
    newNode->left = makeEmptyTree();
    newNode->right = makeEmptyTree();
    return newNode;
  }
  else {
    /*   otherwise the root is not null, we need to do comparisons to
	 decide where to place the string */
    /*   if the root value is less than the value we are adding, place
	 into left subtree */
    if (strcmp(root->val, string) < 0)
      root->left = insertIntoTree(root->left, string);
    
    /*   if larger, place into right subtree  */
    else if (strcmp(root->val, string) > 0)
      root->right = insertIntoTree(root->right, string);
    /*   if they are equal (returns 0) then the string is already in the
	 tree and we do not need to do anything */
    return root;
  }
}

TNode* searchTree(TNode* root, char* string)
{
  if(root){
    if (strcmp(root->val, string)==0)
      return root;
    if (strcmp(string, root->val)<0)
      searchTree(root->right, string);
    else
      searchTree(root->left, string);
  }else
    return NULL;
}

void printTree(TNode* root)
{
  if (root == NULL)
    return;
  else {
    printf("(");
    printTree(root->left);
    printf(" %s ",root->val);
    printTree(root->right);
    printf(")");      
  }
}

void destroyTree(TNode* root)
{
  if (root == NULL)
    return;
  if (root->left != NULL)
    destroyTree(root->left);
  if (root->right != NULL)
    destroyTree(root->right);
  free(root->val);
  free(root);
}

int main(int argc, char* argv[])
{
  if(argc != 2){
    printf("Usage: ./tree wordList\n");
    return 1;
  }
  TNode* tree = makeEmptyTree();
  
  FILE* fp; /* initalize fp as a pointer to a file */
  /* The path to the file containing the words is stored in argv[1]. Call fopen to open it.  */
  fp = fopen(argv[1], "r");
  if (!fp) return -1; /* if the file does not exist exit with error */

  int c;
  char word[200];
  size_t length;
  
  /* use fscanf to read a word at a time from the file, and call
     insertIntoTree to insert the word into the tree. */
  while (!feof(fp))
    {
      fscanf(fp, "%s", word);
      /* call insertIntoTree with a pointer to the tree and a pointer
	 to the currently read word */
      tree = insertIntoTree(tree, word);
      /* move the pointer forward to the next word in the file */
      /* fp+=strlen(word); */
    } /* we have readed the eof and added all words to the tree */

  fclose(fp); /* close the file pointed to by fp */
  
/*   printTree(tree); */
  while(1){
    printf("word: ");
    int r = scanf("%s", word);
    if(r != 1)
      break;
    length = strlen(word);
    if(length < 1)
      break;
    TNode* node = searchTree(tree, word);
    if(node){
      printf("PRESENT\n");
    }else{
      printf("ABSENT\n");
    }    
  }
  destroyTree(tree);
  return 0;
}
