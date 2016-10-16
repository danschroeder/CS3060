//Michael Mickelson
//CS3060-001 Summer 2016
//Program 1

#include <stdio.h>

main(int argc, char* argv[])
{

printf("Michael Mickelson Program1\n");

int i;
	for (i =0; i < argc; i++)
	{
		printf("Argument %d:%s\n",i, argv[i]);
	}
printf("The number of strings printed: %d\n", argc);

}

