//Michael Mickelson
// CS3060 Program 3

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>	

#define MAX_NUMBERS (26)
#define MAX_FACTORS (11)

int numbersToFactor[MAX_NUMBERS];
int factors[MAX_NUMBERS][MAX_FACTORS];
pthread_t thread[MAX_NUMBERS];

void *factor(void *index) 
{
	int* indexPnt = (int*)index;
	int temp = numbersToFactor[*indexPnt];
	int size = 0;
	int i;
	for (i = 2; i <= temp;i++)
	{
		while (temp % i == 0)
		{
			temp = temp / i;
			factors[*indexPnt][size] = i;			
			size++;
		}	
	}
}

int main(int argc, char *argv[])
{
	
	//check for no arguments
	if (argc == 1)
	{
		printf("Usage:./p3 <number to factor>...\n");
		exit(0);
	}
	//set up numbers to factor with numbers
	int i;	
	for(i = 0; i < argc-1; i++)
	{
		numbersToFactor[i] = atoi(argv[i+1]);
	}

	//start new threads and join
	for (i =0; numbersToFactor[i] != 0; i++)
	{
		pthread_create(&thread[i], NULL, factor, (void*) &i);
		
		pthread_join(thread[i], NULL);
	}

	for (i =0; numbersToFactor[i] != 0; i++)
	{
		
		//wait for a thread and print its work
//		pthread_join(thread[i], NULL);

	}	

	//print out each factor
	for (i = 0; numbersToFactor[i] != 0; i++)
	{

		printf("%d: ", numbersToFactor[i]);
	
		int j;
		for(j = 0; factors[i][j] != 0; j++)
		{
			printf("%d ", factors[i][j]);	
		}

		printf("\n");
	}

}
