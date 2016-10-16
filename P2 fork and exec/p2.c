#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
int val;
val = fork();

  if (val == 0)
  {
  	printf("Child process has started. ");
	
	if (argc == 1)
	{
		printf("No arguments provided. Terminating Child.\n");	
	}
	else if (argc == 2)
	{
		printf("One argument provided. Calling execlp(), never to return (sniff)\n");		
		execlp(argv[1], argv[1], NULL);
	}
	else
	{
		printf("More than one argument provided. Calling execvp(), never to return (sniff)\n");
		
		printf("size of argc is: %d\n", argc);
		
		execvp(argv[1], argv+1);
	}
  }
  else
  {
    printf("Parent process has started. Waiting for process id#%d\n", val);
	wait(&val);
	printf("Parent resumed. Child exit code of %d. Now terminating parent.\n", val);

  }
exit(EXIT_SUCCESS);

}
