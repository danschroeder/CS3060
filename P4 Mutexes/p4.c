//Michael Mickelson
// CS3060 Program 3

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>	
#include <stdbool.h>

#define BUFFER_SIZE (100)
#define MAX_FACTORS (11)

//global variables
int numbersToFactor[BUFFER_SIZE];
int numberCount, inputCount, outputCount, numbersDone;
pthread_mutex_t inputLock, outputLock;
pthread_t consumer, producer;

struct outputBuffer{
	int factors[BUFFER_SIZE][MAX_FACTORS];
	int numbersFactored[BUFFER_SIZE];
};

struct outputBuffer outBuffer;

void factor(int num) 
{
	outBuffer.numbersFactored[outputCount] = num;
	int size = 0;
	int i;

	//clear the array of old factors
	for (i = 0; i < MAX_FACTORS; i++)
	{
		outBuffer.factors[outputCount][i] = 0;
	}

	for (i = 2; i <= num;i++)
	{
		while (num % i == 0)
		{
			num = num / i;
			outBuffer.factors[outputCount][size++] = i;
		}	
	}
	outputCount++;
}

void *producerLogic()
{
	int* temp[MAX_FACTORS];
	int num;

	while (numbersDone < numberCount)
	{
		//Wait for numbers to be added to the input buffer
		while (inputCount == 0);	

		//factor each number
		int i;
		for (i = 0; inputCount > 0 && i < BUFFER_SIZE; i++)
		{
			//work with input buffer
			pthread_mutex_lock(&inputLock);
			num = numbersToFactor[i];	
			inputCount--;
			pthread_mutex_unlock(&inputLock);

//	printf(" input count: %d, num: %d, i inside producer: %d\n", inputCount, num, i);
			
			//output buffer
			while (outputCount == BUFFER_SIZE);
			pthread_mutex_lock(&outputLock);
			factor(num);
			numbersDone++;
			pthread_mutex_unlock(&outputLock);
		}
	}
}

void printArray(int index)
{
	//print out each factor
	printf("%d: ", outBuffer.numbersFactored[index]);
	int i;
	for(i = 0; outBuffer.factors[index][i] != 0; i++)
	{
		printf("%d ", outBuffer.factors[index][i]);	
	}
	printf("\n");
}

void *consumerLogic()
{
	int numbersOutput = 0;

	//wait for numbers to be added to the output buffer
	while (numbersOutput < numberCount)
	{
		while (outputCount == 0);

		int i;
		for (i = 0; outputCount > 0 && i < BUFFER_SIZE; i++)
		{ 
			pthread_mutex_lock(&outputLock);
			printArray(i);
		//	printf("factored number[i] inside consumer: %d\n", outBuffer.numbersFactored[i]);
			outputCount--;
			numbersOutput++;
			pthread_mutex_unlock(&outputLock);
//	printf(" output count: %d, numbersOutput: %d, i inside consumer: %d  ", outputCount, numbersOutput, i);
		}
	}
	//printf("done inside of consumerLogic");
}

int main(int argc, char *argv[])
{
	//check for no arguments
	if (argc == 1)
	{
		printf("Usage:./p3 <number to factor>...\n");
		exit(0);
	}
	else 
	{
		numberCount = argc - 1;
	}

inputCount = 0;
	//create the producer and consumer threads
	pthread_create(&producer, NULL, producerLogic, NULL);
	pthread_create(&consumer, NULL, consumerLogic, NULL);

	//set up numbers to factor with numbers
	int i;	
	for(i = 0; i < argc-1; i++)
	{
		while (inputCount == BUFFER_SIZE);
		
		pthread_mutex_lock(&inputLock);
		numbersToFactor[inputCount] = atoi(argv[i+1]);
		inputCount++;
	//printf("numbersToFacter at input: %d, inputCount: %d\n", numbersToFactor[inputCount - 1], inputCount);
		pthread_mutex_unlock(&inputLock);
	}

	//Join up the threads
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
}
