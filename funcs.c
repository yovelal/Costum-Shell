

#include "funcs.h"
#define GRN "\e[0;32m"
#define BGRN "\e[1;32m"
#define BLU "\e[0;34m"
#define BBLU "\e[1;34m"
#define WHT "\e[0;37m"
#define RED "\033[0;31m"
#define READ 0
#define WRITE 1


void printPath(){
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    char * user = getenv("USER");
    printf( RED"%s:",user);
    printf(BLU"%s:",cwd);
    printf(WHT);


}


parseInfo* parse(char* line){
   // printf("ENTER PARSE\n");
    parseInfo* info = (parseInfo*)malloc(sizeof(parseInfo));
    info->pipe_index=-1;

    //scan line
    int size=0;
    for(int i=0;line[i]!='\0';i++){
        if(line[i]==' '){
            size++; //count spaces
            }
        if(line[i]=='|'){
            info->pipe_index=i;
            info->command = (char**)malloc(sizeof(char*)+1);
            info->command[0]=strdup(line);
            return info;
        }
    }

    size+=2; // num of words + null

	
    info->command = (char**)malloc(sizeof(char*)*(size+1));

    //if the line is empty
    if (strcmp(line,"")==0){
        info->command[0]=strdup("");
        return info;      
    }
   

    char* temp;
    int index=0;
    temp = strtok(line," ");
    info->command[0]=strdup(temp);
    while(temp!=NULL){
        //printf("temp is %s \n",temp);
        info->command[index]=strdup(temp);
        index++;
        temp = strtok(NULL," ");
    }
    //add NULL to the end:
    info->command[index]=NULL; 
    info->size=size;
    
    //if the command is "cd" then add "/"
    if((strcmp(info->command[0],"cd")==0) && ((size==2) || (strcmp(info->command[1],"~")==0)))
        {
            info->command[1]=strdup(getenv("HOME"));    
             
        }

    return info;
}


void executeCommand(parseInfo * info){

    //if pipe 
    if(info->pipe_index!=-1){
        execute_pipe(info->pipe_index,info->command[0]);
    }
    //if the command is empty - exit
    else if(strcmp(info->command[0],"")==0)
        exit(1);

    else if(strcmp(info->command[0],"cat")==0 && strcmp(info->command[1],">")==0){
        execute_cat(info);
    }

    //genral case - use execvp on the command:
    else if(strcmp(info->command[0],"pwd")==0 || strcmp(info->command[0],"nano")==0 || strcmp(info->command[0],"cat")==0 || strcmp(info->command[0],"wc")==0
            || strcmp(info->command[0],"cp")==0|| strcmp(info->command[0],"clear")==0 || strcmp(info->command[0],"grep")==0 || strcmp(info->command[0],"wget")==0 
            || strcmp(info->command[0],"ls")==0 || strcmp(info->command[0],"tree")==0 )
    {
        if(execvp(info->command[0],info->command)<0){
            printf("faild to execute the '%s' command\n",info->command[0]);

        }
    }
    else {
        printf("%s is not supported! \n",info->command[0]);
    }

    exit(1);

}

void execute_cat(parseInfo* info){

    //check if the file name exists:
    if(info->command[2]==NULL){
        printf("bash: syntax error near unexpected token `newline'\n");
        exit(1);
    }

    char* fileName=strdup(info->command[2]);
    //open file and input text 
    FILE* fptr;
    fptr = fopen(fileName,"w");
    if(fptr==NULL)
    {
        printf("error - could not open %s\n",fileName);
        exit(1);
    }
    
    //read line from terminal into buffer until ctrl+d :
    char buffer[1024];
    while(fgets(buffer,1024,stdin)!=NULL){
        fprintf(fptr,"%s",buffer);
    }
    fclose(fptr);
    exit(1);

}

void execute_pipe(int pipe_index , char* line){
    parseInfo* info1 ,*info2;
    char *line1 , *line2;
    line1 = (char*)malloc(sizeof(char)+strlen(line));
    line2 = (char*)malloc(sizeof(char)+strlen(line));
    int index1=0,index2=0;
    for (int i=0; i < strlen(line); i++){
        if(i<pipe_index-1){
            line1[index1]=line[i];
            index1++;
        }
        else if (i>pipe_index+1){
            line2[index2]=line[i];
            index2++;
        }
    }
    line1[index1]='\0';
    line2[index2]='\0';


    info1=parse(line1);
    info2=parse(line2);

    int p[2];
    pipe(p);
    int childPid = fork();

    if(childPid == 0) //child
    {
        close(p[READ]);
        dup2(p[WRITE],WRITE);
        executeCommand(info1);


       
    }
    else
    {
        close(p[WRITE]);
        dup2(p[READ],READ);
        waitpid(childPid);
        if (strcmp(info2->command[0],"exit")==0 || strcmp(info2->command[0],"cd")==0)
            exit(1);
        executeCommand(info2);

    }

 }






void freeInfo(parseInfo* info){

    if(info != NULL){
			if(info->command != NULL){
				for(int i=0;i<info->size;i++){
					if(info->command[i]!=NULL)
						free(info->command[i]);
				}
				
				free(info->command);
			}
			free(info);
    }
}


