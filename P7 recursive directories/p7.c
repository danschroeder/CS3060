//Michael Mickelson
// CS3060 Program 7

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <inttypes.h>

//format spacing before files needs to be corrected
char *slash = "/";
char *current = ".";
char *parent = "..";
int dirIndent = 3;
int fileIndent = 6;

int printDir(char *path, int indent)
{
	int space= 0;
	DIR *directory = opendir(path);
	struct dirent *dirEntry = readdir(directory);
	struct stat fileStat;
	char *filePath = malloc(5000);

	printf("%*cdir %s\n", indent * dirIndent, ' ', path);
//	printf("%s\n", filePath);
	while (dirEntry != NULL)
	{
		//set filepath
		filePath[0] = '\0';
		strncat(filePath, path, strlen(path));
		strncat(filePath, slash, strlen(slash));
		strncat(filePath, dirEntry->d_name, strlen(dirEntry->d_name)); 

		//check for directory	
		if (strcmp(dirEntry->d_name, current) != 0 && strcmp(dirEntry->d_name, parent) != 0)
		{
			if (dirEntry->d_type == DT_DIR)
			{
				space += printDir(filePath, indent + 1);
			}
			else 
			{
				lstat(filePath, &fileStat);
				printf("%*c%10lld:%s\n", indent * dirIndent, ' ', (long long) fileStat.st_size, dirEntry->d_name);
				space += fileStat.st_size;
			}
		}
		dirEntry = readdir(directory);
	}
	
	//return statement
	free(filePath);
	filePath = NULL;
	return space;
}

int main(int argc, char *argv[])
{
	int totalSpace = 0;

	//check for argument path
	if (argc == 1)
	{
//		printf("No argument provided\n");
		totalSpace = printDir(".", 0);
	}
	else
	{
//		printf("argument provided: %s\n", argv[1]);
		totalSpace = printDir(argv[1], 0);	
	}	
	
	printf("\nTotal file space used:%d\n", totalSpace);
}
