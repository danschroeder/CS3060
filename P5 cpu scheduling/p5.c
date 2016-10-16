//Michael Mickelson
// CS3060 Program 5

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES (25)
#define QUANTUM (100)

//global variables
struct process{
	int arrival, burstTime, responseTime, waitTime, turnAroundTime, timeRemaining, interuptTime;
	bool done;
};

struct process processList[MAX_PROCESSES];
double averageWait, averageResponse, averageTurnAround;
int numProcesses;

void PrintProcessList()
{
	int i;
	for (i = 0; i < numProcesses; i++)
	{
		printf("Process %d Arrival: %d BurstTime: %d\n", i, processList[i].arrival, processList[i].burstTime);
	}
}

void ResetList()
{
	int i;
	for (i = 0; i < numProcesses; i++)
	{
		processList[i].done = false;
		processList[i].responseTime = -1;
		processList[i].waitTime = 0;
		processList[i].turnAroundTime = 0;

	}
}
void PrintStats(char *schedule)
{
	printf("%s\nAvg. Resp.: %.2f, Avg. T.A.: %.2f, Avg. Wait: %.2f\n", schedule, averageResponse, averageTurnAround, averageWait); 
}

//Calculates the average wait response and turnaround for each process in process list
void Average()
{
	int i, sumWait, sumResponse, sumTA;
	sumWait = 0;
	sumResponse = 0;
	sumTA = 0;

	for (i = 0; i < numProcesses; i++)
	{
		sumWait += processList[i].waitTime;
		sumResponse += processList[i].responseTime;
		sumTA += processList[i].turnAroundTime;	
	}
//	printf("sumwait: %d, sumR: %d, sumTA: %d\n", sumWait, sumResponse, sumTA);
	averageWait = sumWait /(double) numProcesses;
	averageResponse = sumResponse / (double)numProcesses;
	averageTurnAround = sumTA / (double)numProcesses;
}

void FCFS()
{
	//loop through each process
	int i, clock = 0;

	for (i = 0; i < numProcesses; i++)
	{	
		if (clock < processList[i].arrival)
		{
			clock = processList[i].arrival;
		}
		//waitTime responseTime turnAroundTime
		
		//calculate and store response and wait
		processList[i].waitTime = clock - processList[i].arrival;
		processList[i].responseTime = clock - processList[i].arrival;

		//update clock
		clock += processList[i].burstTime;
	
		//calculate and store turn around	
		processList[i].turnAroundTime = clock - processList[i].arrival;
//		printf("Process %d Arrival: %d BurstTime: %d waitTime: %d rT: %d TAT: %d\n", i, processList[i].arrival, processList[i].burstTime, processList[i].waitTime, processList[i].responseTime, processList[i].turnAroundTime);

	}
	
	Average();
}

void ShortestJobFirst()
{
	ResetList();

	int numDone = 0, shortestIndex = -1, clock = 0;
	//loop through each process
	while (numDone < numProcesses)
	{
		//check if clock is smaller than next arrival 
		if (processList[numDone].arrival > clock)
		{
			clock = processList[numDone].arrival;
		}	
		
		//check for shortest within clock 
		int i;
		for (i = 0; processList[i].arrival <= clock && i < numProcesses; i++)
		{
			//set smallest index to an initial index
			if (shortestIndex == -1)
			{
				if (!processList[i].done)
				{
					shortestIndex = i;
				}	
			}
			//loop and find shortest
			else if (!processList[i].done && processList[i].burstTime < processList[shortestIndex].burstTime)
			{
				
				shortestIndex = i;
			}
		}

		processList[shortestIndex].done = true;

		//calculate wait time & response time
		processList[shortestIndex].waitTime = clock - processList[shortestIndex].arrival;
		processList[shortestIndex].responseTime = clock - processList[shortestIndex].arrival;

		//update the clock
		clock += processList[shortestIndex].burstTime;

		//calculate the turn around time
		processList[shortestIndex].turnAroundTime = clock - processList[shortestIndex].arrival;


		//update looping variables
		numDone++;
		shortestIndex = -1;
	}
	Average();
}

void ShortestRemaining()
{

}

void RoundRobin()
{
	ResetList();

	int numDone = 0, clock = 0;	

	while (numDone < numProcesses)
	{
		//check if clock is smaller than next arrival 
		if (processList[numDone].arrival > clock)
		{
			clock = processList[numDone].arrival;
		}
		//loop through each within clock
		int i;
		for (i = 0; processList[i].arrival <= clock && i < numProcesses; i++)
		{
			//check if time remaining greater than quantum
			if (!processList[i].done)
			{
				//update response time
				if (processList[i].responseTime == -1)
				{
					processList[i].responseTime = clock - processList[i].arrival;
					processList[i].interuptTime = processList[i].arrival;

					//update wait time 
					processList[i].waitTime = clock - processList[i].arrival;
				}

				if (processList[i].timeRemaining > QUANTUM)
				{
					//update variables
					processList[i].timeRemaining -= QUANTUM;
					clock += QUANTUM;					
					processList[i].interuptTime = clock;
				}
				else
				{
					//calculate wait time 
					processList[i].waitTime += clock - processList[i].interuptTime;
					//update variables
					clock += processList[i].timeRemaining;
					processList[i].done = true;
					numDone++;
	
					//calculate the turn around time
					processList[i].turnAroundTime = clock - processList[i].arrival;				
				}
			}
		}
	}
	Average();
}


int main(int argc, char *argv[])
{
//	printf("we got inside main");

	//populate the process list
	int num;
	int i = 0;		
	bool arrivalFlag = true;
	while (fscanf(stdin, "%d", &num) == 1)
	{
		//check for arrival vs burst time
		if(arrivalFlag)
		{
			processList[i].arrival = num;
			arrivalFlag = false;
		}
		else
		{
			processList[i].timeRemaining = num;
			processList[i++].burstTime = num;
			arrivalFlag = true;
			numProcesses++;
				
		}
	}

	//run each schedule
	FCFS();
	PrintStats("\nFirst Come, First Served");

	ShortestJobFirst();
	PrintStats("\nShortest Job First");

//	ShortestRemaining();
	PrintStats("\nShortest Remaining Time First");

	RoundRobin();
	PrintStats("\nRound Robbin with Time Quantum of 100");


	
//	PrintProcessList();
}
