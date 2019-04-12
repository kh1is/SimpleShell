#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
//#include <syslog.h>


int readLine();
void handler();

int flag=0;

int main()
{

    char command_line[100];
    char* parms[10];

    printf("Shell > ");

    while(readLine(command_line,parms)){

        //interrupt
        signal(SIGCHLD, handler);

        pid_t pid = fork();

        //child process
        if(pid == 0){

            execvp(parms[0],parms);

        }
        //parent process
        else if(pid > 0){

            //not a background process
            if(flag == 0){
                wait();
            }
            //background process
            else{
                flag = 0;
            }
        }

        printf("Shell > ");

    }

    return 0;
}

int readLine(char command_line[],char* parms[]){

    //read a command
    fgets(command_line,100,stdin);

    //put null in the end of command string
    int i = strlen(command_line);
    command_line[i-1] = '\0';


    //check if process a background process or not
    if(command_line[i-2] == '&'){
        command_line[i-2] = '\0';
        if(command_line[i-3] == ' '){
            command_line[i-3] = '\0';
        }
        flag = 1;
    }


    //check if user wants to exit
    if(strcmp(command_line,"exit") == 0){
        exit(0);
    }


    //create array of parameters from a command string
    strdup(command_line);
    int j = 0;
    parms[j] = strsep(&command_line, " ");

    while(parms[j] != NULL){

        j++;
        parms[j] = strsep(&command_line, " ");

    }

    return 1;

}

void handler(){

    FILE *f;
    f = fopen("/home/khalil/Desktop/interrupt", "a+");
    if (f == NULL) {
        printf("error");
    }

    fprintf(f, "Child process was terminated\n");

    fclose(f);


}


