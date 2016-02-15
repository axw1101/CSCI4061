#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>

#include "util.h"

//This function will parse makefile input from user or default makeFile. 
int parse(char * lpszFileName)
{
	int nLine=0;
	char szLine[1024];
	char * lpszLine;
	FILE * fp = file_open(lpszFileName);
	int index = 0;
	char * token;
	if(fp == NULL)
	{
		return -1;
	}

	while(file_getline(szLine, fp) != NULL) 
	{
		nLine++;
		// this loop will go through the given file, one line at a time
		// this is where you need to do the work of interpreting
		// each line of the file to be able to deal with it later
		
		//Remove newline character at end if there is one
		lpszLine = strtok(szLine, "\n"); 
		printf("lpszLine Ln#35: %s\n", lpszLine);

		//You need to check below for parsing. 
		//Skip if blank or comment.
		if(lpszLine == NULL){
			continue;		
		}
	
	
		if(strncmp("#", lpszLine, 1) == 0){
			continue;		
		}			
		//Skip if whitespace-only.
		//Only single command is allowed.
		if(targetArray[index].szTarget[0] == '\0'){

			//Remove leading whitespace.
			while(isspace(*lpszLine) || *lpszLine == '\t'){
				lpszLine++;
			}
			printf("lpszLine AFTER REMOVING LEADING WHITESPACE Ln#55: %s\n", lpszLine);

			token = strtok(lpszLine, ":");
			strcpy(targetArray[index].szTarget, token);
			printf("target from parse Ln#59: %s\n", targetArray[index].szTarget);
			printf("lpszLine from if Ln#60: %s\n", lpszLine);
			token = strtok(NULL, " ");
			targetArray[index].nDependencyCount = 0;
			while(token != NULL){
				strcpy(targetArray[index].szDependencies[targetArray[index]
				.nDependencyCount].szDependent, token);
				//printf("Print out dependencies in while: %s\n", targetArray[index].szDependencies[targetArray[index]
				//.nDependencyCount].szDependent);
				targetArray[index].nDependencyCount++;
				token = strtok(NULL, " ");

			}	
		}else{
			//Check for syntax error
			if(strncmp("\t", lpszLine, 1) != 0){
				fprintf(stderr, "Syntax Error\n");
				exit(EXIT_FAILURE);		
			}
			
			//Remove leading whitespace.
			while(isspace(*lpszLine) || *lpszLine == '\t'){
				lpszLine++;
			}

			strcpy(targetArray[index].szCommand, lpszLine);	
			printf("Printing Commands: %s\n", targetArray[index].szCommand);
			index++;
		}

		//If you found any syntax error, stop parsing. 
		
		//If lpszLine starts with '\t' it will be command else it will be target.

		//It is possbile that target may not have a command as you can see from the example on project write-up. (target:all)
		//You can use any data structure (array, linked list ...) as you want to build a graph
	}

	//Close the makefile. 
	fclose(fp);

	return 0;
}

void execute(char* szTarget, int execute, int checkTimeStamp, char* szLog){
	// Tottaly, we have 3 cases here
}

void show_error_message(char * lpszFileName)
{
	fprintf(stderr, "Usage: %s [options] [target] : only single target is allowed.\n", lpszFileName);
	fprintf(stderr, "-f FILE\t\tRead FILE as a maumfile.\n");
	fprintf(stderr, "-h\t\tPrint this message and exit.\n");
	fprintf(stderr, "-n\t\tDon't actually execute commands, just print them.\n");
	fprintf(stderr, "-B\t\tDon't check files timestamps.\n");
	fprintf(stderr, "-m FILE\t\tRedirect the output to the file specified .\n");
	exit(0);
}

//Searches through targetArray for target, returns index if target found, else -1
int search(char* target) 
{
	int index = -1;
	int i = 0;
	while((index == -1) && (targetArray[i].szTarget[0] != '\0'))
	{
		if(strcmp(target, targetArray[i].szTarget) == 0)
			index = i;
		i++;
	}

	return index;
}

int main(int argc, char **argv) 
{
	// Declarations for getopt
	extern int optind;
	extern char * optarg;
	int ch;
	char * format = "f:hnBm:";
	
	// Default makefile name will be Makefile
	char szMakefile[64] = "Makefile";
	char* szTarget;
	char szLog[64];
	int execute = 1;
	int checkTimeStamp = 1;
	while((ch = getopt(argc, argv, format)) != -1) 
	{
		switch(ch) 
		{
			case 'f':
				strcpy(szMakefile, strdup(optarg));
				break;
			case 'n':
				execute = 0; // if case is n, we don't need to execute the program
				break;
			case 'B':
				checkTimeStamp = 0; // Do not check time stamp for target and input
				break;
			case 'm':
				strcpy(szLog, strdup(optarg));
				break;
			case 'h':
			default:
				show_error_message(argv[0]);
				exit(1);
		}
	}

	argc -= optind;
	argv += optind;

	// at this point, what is left in argv is the targets that were 
	// specified on the command line. argc has the number of them.
	// If getopt is still really confusing,
	// try printing out what's in argv right here, then just running 
	// with various command-line arguments.
	
	/* Parse graph file or die */
	if((parse(szMakefile)) == -1) 
	{
		printf("FAIL");
		return EXIT_FAILURE;
	}

	if(argc > 1)
	{
		show_error_message(argv[0]);
		return EXIT_FAILURE;
	}

	//You may start your program by setting the target that make4061 should build.
	//if target is not set, set it to default (first target from makefile)
	
	if(argc == 1)
	{
		szTarget = argv[0];
	}
	else
	{
		szTarget = targetArray[0].szTarget;
	}

	printf("\nSUCCESS\n");
	//execute(szTarget, execute, checkTimeStamp, szLog);
	
	//Check to see if target exists
	int targetIndex = search(szTarget);
	if(targetIndex == -1){
		fprintf(stderr, "Error: Non-existent target\n");
		exit(EXIT_FAILURE);
	}
	
	
	//after parsing the file, you'll want to check all dependencies (whether they are available targets or files)
	//then execute all of the targets that were specified on the command line, along with their dependencies, etc.
	return EXIT_SUCCESS;
}
