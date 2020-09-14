#include "array.h"
#include <stdio.h>


int main(){
	long capacity = 2;
	long stuff = 4100;
	Array testArray = array_new(capacity);
	for (int i=0;i<1000;i++){
		array_insertBack(&testArray,stuff);
	}

	printf("startpointer: %p endpointer: %p \n",&testArray.data[testArray.front],&testArray.data[testArray.back-1]);

	//array_print(testArray);
	
	array_destroy(testArray);
}