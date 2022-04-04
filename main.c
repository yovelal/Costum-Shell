#include "funcs.c"


int main (int argc, char **argv) {
	int childPid;	
	char * cmdLine=NULL; 
	parseInfo *info=NULL; 
	

	printf(BGRN"---------------STARTING CUSTOM SHELL---------------\n");
	while(1){ 

		printPath();
		cmdLine= readline(" >> "); 
		add_history(cmdLine);
		//printf("the cmdlone is:%s\n",cmdLine);
		info = parse(cmdLine); 
		
		if(info->command && strcmp(info->command[0],"exit")==0){
			exit(1);
		}
		else if(info->command && strcmp(info->command[0],"cd")==0){
			//if the directory not found print error 
			if(chdir(info->command[1])==-1){
				printf("%s NOT FOUND\n", info->command[1]);
			}
			continue;
    	}

		childPid = fork();   

		if (childPid == 0)
		{		
			/* child code */
			executeCommand(info); //calls  execvp
			freeInfo(info);
			
		}
		else 
		{ 
			/* parent code */
			waitpid(childPid);

		}
		
		
 
 	} 
}
