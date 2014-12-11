//Author Joe Waldron
//Project 3: Wite a command shell with the following commands:
//	!N- where n is the past command to be re-executed
//	help - prints a list of Internal commands
//	history - prints a list of commands executed so far, including their ar	//		guments
//		*only remember the last 10 commands printed
//	quit - Clean up memory and terminate the shell to the the bash
//	verbose on/off - prints a list of commands and their arguments as shown
//		in the examples.
//			*print error message if not on/ off
//				usage: verbose [on | off]


//TODO:

// Deallocate lists histL, tokens, and line
//create .h file
//use a repository yes
//put the verbose function through yes
//make warning symbols yes
//comment out printf#
//explain 


//explain full porblem with stack overflow


#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <assert.h>
#include <getopt.h>

//#define MAX_LEN 100 // This big for comments

static const char help[] = "help";
static const char history[] = "history";
static const char quit[] = "quit";
static const char verboseCMD[] = "verbose";
//static const char verboseOffCMD[] = "verbose off";
static const char N[] = "!";


static int histLSize;
static bool isVerbose;

void display_help( void *argc, void *argv[] ) {
	free(argc);
	free(argv);
	printf("List of Internal Command:\n");
	printf(">!N\n>help\n>history\n>quit\n>verbose [on|off]\n");

}

void show_hist(int size, char** histL2) {
	//size is the count of the number of inputs
	
	int count = 0;
	int tempHistL;
	//printf("%i is size of size, %i is size of histL\n", size, histLSize);
	if ((size- histLSize > 0 )   ) {
		//printf("larger  than size of history\n");
		count = size - histLSize; 		//could be 0 or greater
		tempHistL = histLSize;			
	} else {
		
		//printf("smaller than size of history\n");
		tempHistL = histLSize;
		histLSize = size;
	}
	//start at -h size spaces away from List end 
	for (int strt = size-histLSize; strt <size;strt++ ) {	//starts at 0
		printf("%i:  %s \n", count+1,histL2[strt]  );
		count++;
	}
	
	histLSize = tempHistL;
	//printf("HistLSize is now %i", histLSize);
	return;
}
void quit_c( int argc, char **argv ) {
	//Free memory from history lists

	//printf("Freeing memory, size of argv %i\n", sizeof(argv));
	for ( int i = 0; i < argc; i++ ) {
		//printf("Freeing memory, size of argv %i\n", sizeof(argv[i]));		
		free(argv[i]);
		//printf("Freeing memory, size of argv %i\n", sizeof(argv[i]));	
	}	
	free(argv);
	//printf("Freeing memory, size of argv %i\n", sizeof(argv));
	}


void verbose( int argc, char* argv[]) {
	//make the verbose true
	//verbose = true;
	//printf("Verbose until I los\n");
	printf("\tcommand: %s\n\n", argv[0]);
	printf("\tinput command tokens:\n");

	int vi = 0;
	
	while ( argv[vi] != NULL ) {
		printf("\t%i: %s\n", vi, argv[vi]);
		vi++;	
	}
	printf("\twait for pid %i: %s\n", argc, argv[0]);
	printf("\texecvp: %s\n", argv[0]);

	return;
}

int moreThanHist( int indx, int list) {
	if (indx > list || indx == list ) {
		return 1;
	} 
	return 0;
	}

int main( int argc, char* argv[]) {
    	//initialize lists
	int count=1;
	static int i;				//token list size
	bool isBang = false;				//dont create a non nuLL
	//char line[MAX_LEN ];
	char *line = NULL;
	size_t len_alloc = 0; //siz
	bool isNotHist = false;			//initialize the type of isNotHist
	isVerbose = false;		//init verbose to false
	//Find the optional Tokens
	int opt = 0;
	histLSize = 10;
	//opt = getopt(argc, argv, "vh:");
	while( (opt = getopt(argc, argv, "vh:")) != -1){ 
		switch( opt) {
			case 'v':
				printf("The function verbose is on\n");
				isVerbose = true;
				//printf("%s\n", "verbose");
				break;
			case 'h':
				if (atof(optarg) <= 0  ) {
					fprintf( stderr, "usage: mysh [-v] [-h pos_num]");
					return 1;
					}
				//printf("the function History is on\n");
				//printf("%i\n", atoi(optarg));
				histLSize = atoi(optarg);
				printf("Changed to size of for history\n");
				break;
			default:
				exit(EXIT_FAILURE);
		}

	}

	//char *result, *cmd;
	//char *cmd;
	size_t result;
	printf("mysh[%i]>", count);
	pid_t my_id;		//id of pids
    	//create a list of history commands



	char **histL;//[15][10];
	//set size of list to -h param or 10
	histL = ( char** )malloc( sizeof( char* ) * histLSize );	
	//
	for (int jdx = 0; jdx < histLSize; jdx++) {
		histL[jdx] =	NULL;	//( char*)malloc(sizeof(char)*15);
	}
	//strdup for each histL
	
	//use getline instead of fgets
	//dynam buffering for getline
	//	

	//line = (char*)malloc(MAX_LEN+1);
	//	while ( ( result = fgets( line, MAX_LEN, stdin ) ) ) {
	//line = NULL;
	while (true) {
		
		result = getline( &line, &len_alloc, stdin);
		if( count > histLSize ) {  //by default, count is 10 and idx of HISTL is 9
		//always realloc the size of the histL without changin -h param
			//printf( "The size of the count is %i, and histLsize is %i\n ", count, histLSize);
			histL = ( char** )realloc(histL, sizeof(char*)*count);
			//count starts at 1, at count =  10,histL is out of range->realloc
			//printf("putting in index %i \n", count-1);
			histL[count-1] = (char*)malloc(sizeof(char)*40);
		}
		
		if (strlen(line) == 1) {		//Find the user enters no input
			fprintf( stderr, "fatal error");
			break;
			//return 0;	
		}
		
		if ( line[ 0 ] == '#' ) {
	    		printf( "%s", line );
		}
		//save line to the history list;

		else {
		
		//printf("Size of line is %lu\n", strlen(line));
		char* p = NULL;
		//char* tokens[strlen(line)];
		
		//tokens = (char*)malloc(strlen(line)*sizeof(char));
		LOOP:
		for ( int tkI = 0; tkI < 10; tkI++ ) {
	
			argv[tkI] = NULL;
		}
		p = strtok(line, " \n");
		
		i=0;
		for( i = 0; p != NULL; i++ )
		{
		
			argv[i] = malloc(strlen(p) + 1);
			strcpy(argv[i], p);
			p = strtok(NULL, "\"\n");
			//printf("token added %s\n", argv[i]);
			
		} 
		//printf("%i is number \n", i);
		if ( !isBang ) {
			i = i+1;
		
		
		//null terminating string
		
			argv[i] = NULL;
			//printf("NUll added\n");
		}
		isBang = false;
		for (int j = 0; j < 1; j++)
		{
			// printf("Token %i is '%s'.\n", j, argv[j]);
		}
		
		 
		//printf("Looking at argv\n");
		//char *newString; 		//malloc
		//strcpy(newString,cmd);
	   	// printf("[0][0] %s \n", newString[0]);
	   	if ( strcmp( argv[0], help ) == 0 ) {
			display_help(NULL, NULL );

			//hisL[count-1] = (char*)malloc(sizeof("help")+1);
			histL[count-1] = strdup("help");
			//printf("added the histL\n");
		} else if ( strcmp( argv[0], history ) == 0 ) {
			//check if bang function
			histL[count-1] = strdup( "history");
			//strcpy( histL[count-1], "history");
			show_hist(count, histL );
		
		} else if (strcmp( argv[0], quit ) == 0 ) { 
			//free the histL, histList in function
			quit_c(count, histL);
			//free p
			free(line);
			for ( int lmt = 0; lmt < i; lmt++) {
				free(argv[lmt]);
			}
			exit(0);
		
		} else if (strncmp( argv[0], N, 1) == 0 ) {		//should I use a for loop for history?
	     		//printf("Found the N function\n");

			
			char cInt[10];				//initialize to hold number
			strncpy(cInt, argv[0]+1,strlen(argv[0]+1));
			int bang = atoi(cInt);			//convert bang number
			/* Confusing Test on Tokens:
				size_t exclam = atoi(&tokens[0]);
			
				printf("%i is int", exclam);	//returns zero always
			*/
			
			//case 1: number is less than count
			if ( moreThanHist( bang, count ) == 1) {
			
			
				fprintf( stderr, "%i:No history listed, too large\n", bang);
				//printf("Not in hist range\n");
				isNotHist = true;
			}
				//quit and free memory
				
			
			//case 2: number is less than history listed or not in range:
			
			if (bang < (count - histLSize) ) {
				fprintf( stderr, " %i: No history listed, too small\n", bang);
				//quit and free memory
				isNotHist = true;
				//printf("Not in hist range\n");
				
				}
			if (isNotHist) {
				isNotHist =false;
				goto RSTRT;
				}
			if (bang > 0 ) {
				
				//printf("Found a digit\n");
	      		//printf("Exectuing history command, %i : %s\n", bang, histL[bang-1]);			
				//
				//free memory and make a list of tokens

				//int idx = 0;
				isBang = true;
				//mafree(line);		//dont want to free line here is used again
				strcpy( line, histL[bang-1]);
				//printf("Command is %s.\n", line);			
				goto LOOP;
			
			}

	   		} else if (strcmp(argv[0], verboseCMD) == 0) {

				if (strcmp(argv[1], "on") == 0) {
					//printf("Turn on\n");
					isVerbose = true;
					histL[count-1] = strdup("verbose on");

				} else if (strcmp(argv[1], "off")==0) {
					//printf("turn off\n");	
					isVerbose = false;
					histL[count-1]= strdup( "verbose off");

				} else {
					perror( "usage: verbose [on|off]\n" );
			
				}
			
			}

			else {
				my_id = getpid();
				int status;

				fflush( stdout );
			
				if ( isVerbose ) {
					verbose( my_id, argv);
				} 
				pid_t id = fork();
			
				switch( id ) {

				case -1: // the fork() failed
					perror( "fork" );
					exit( EXIT_FAILURE );

				case 0:	// we are the child process

				
					// child will execvp() an 'echo' command
					//argv[0] = "echo";    // command name
					//argv[1] = "hello,";  // first argument
					//argv[2] = "world!";  // second argument
					//argv[3] = NULL;      // terminating NULL pointer
					//printf( "Adding to list %s, \n", argv[0] );
					strcpy( histL[count-1], "Echo");
				
					execvp( argv[0], argv );
					//save to history list

					//printf( "Adding to list %s, \n", argv );
				
				
				
					//printf(cmd[1], cmd);
				
					// we only get here if the execvp() failed
					perror( "execvp" );
					printf("%s: cannot access %s: No such file or directory\n", argv[0], argv[1]);
					// use _exit() to avoid problems with the shared
					// stdout still being used by our parent
					_exit( EXIT_FAILURE );

					// will never reach this statement!
					break;

				default: // we are the parent
					break;

				}
				// parent will wait for child to exit
				id = wait( &status );

				if( id < 0 ) {
					perror( "wait" );

				} else {

					int idx = 0;
					histL[count-1] =  strdup(argv[idx]);
					printf("Token added %s size of %i is\n", argv[idx], i);
					for (idx = 1; idx < i-1; idx++) {		//-1 because no need last one
						strcat( histL[count-1], " ");
						strcat( histL[count-1], argv[idx]);
						printf("Token added %s\n", argv[idx]);
					}
					printf("Exit for Loop\n");
				//puts( "Parent is now exiting." );

				//exit( EXIT_SUCCESS );
					}	
			
			
		}
	    fflush(stdout);
					//for finding a non entered range of histl

		count++;
		RSTRT:	
		//isNotHist = false;
		if (isVerbose )	{
			printf("command status: %i\n", count);
		}
		
		free(line);
		line = NULL;
		len_alloc = 0;
		//result = 0;
		for ( int lmt = 0; lmt < i; lmt++) {
			free(argv[lmt]);
		}
		
		printf("mysh[%i]>", count);
	}

	}	

	return 0;
	}
 
