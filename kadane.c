#include <stdio.h>
#include <stdlib.h>

struct SubArray{
  int start;
  int end;
  int sum;
};

/* function that returns a SubArray structure */
struct SubArray kadane(int* numbers, int length){
  /* Given the numbers array containing length integers, compute the max sub array and return struct SubArray 
     
    We begin by initializing a struct that contains the best sub array we've found so far (bestSubArray), and the best subarray ending at the previous position. 
   */
  /* the base case is handled here numbers[0] */


  /* current best sub array */
  struct SubArray bestSubArray = {0, 0, numbers[0]};
  /* best sub array ending at previous position */
  struct SubArray prevSubArray = {0, 0, numbers[0]};
  
  for(int j = 1; j < length; j++){
    /* the best starting index remains the same */
    if (prevSubArray.sum > 0)
      {
	/* extend bestSubArray to include new large number */
	if (prevSubArray.sum + numbers[j] >= bestSubArray.sum)
	  {
	    bestSubArray.end = j;
	    bestSubArray.sum = prevSubArray.sum + numbers[j];
	  }
	prevSubArray.end = j;
	prevSubArray.sum += numbers[j];
      }
    /* otherwise the best sub array starts (and ends) at j */
    else
      {
	if (numbers[j] >= bestSubArray.sum)
	  {
	    /* the best subarray up to j-1: */
	    prevSubArray.start = j; /* bestSubArray.start; */
	    prevSubArray.end = j;/* bestSubArray.end; */
	    prevSubArray.sum = numbers[j];
	    
	    bestSubArray.start = j;
	    bestSubArray.end = j;
	    bestSubArray.sum = numbers[j];
	  }
	else
	  {
	    prevSubArray.start = j;
	    prevSubArray.end = j;
	    prevSubArray.sum += j;
	  }
      }
  }
  return bestSubArray;
}


int main(int argc, char* argv[]){
  printf("Enter numbers (then press 0 and enter): ");
  int numbersLength = 1;
  int* numbers =  (int*)malloc(sizeof(int)*numbersLength)
    /* TODO: Call malloc to create an array with space for numbersLength ints*/;
  int x = 0;
  int i = 0;
  while(scanf("%d", &x) == 1 && x != 0){
    if(i == numbersLength){
      numbers = (int*)realloc(numbers,sizeof(int)*numbersLength+1);
      numbersLength++;
      /* numbersLength++; */
    }
    numbers[i] = x;
    i++;
  }
  struct SubArray result = kadane(numbers, i);
  printf("Max Sub Array: ");
  for(i = result.start; i <= result.end; i++){
    printf("%d ", numbers[i]);
  }
  printf("\n");
  free(numbers);
  return 0;
}
