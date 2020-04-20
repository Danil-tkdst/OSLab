/* This programm implement a basic functional of bash-terminal.
  Version 2.0: Was added funcrional of redirection input and output data.
  Made by Danil Mikhailov, student pm-2 groupe of MSTUCA in 2020 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 80
#define MAXWORD 16

int main()
{
    char argv[MAXWORD][MAXLINE];
    char *a[MAXWORD]; 
    int i = 0, j = 0, k, err_exec, ch, inp, out, inp_flag = 0, out_flag = 0;
    
    printf("$");
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {                //if end of string, that let's stop reading
            argv[i][j] = '\0';
            k = i;
            for (i = 0; i <= k; ++i) {  //connect of array of strings with array of pointers
                a[i] = argv[i];
            }
            a[i] = NULL;
            pid_t pid = fork();         //Start branching for exec
            if (pid == -1){
                perror("fork");
            }
            else {
                if (!pid) {
                    if (out_flag == 1) {     //if out-way was directed
                        int fdout = open(a[out], O_WRONLY | O_CREAT | O_TRUNC, 0664);  //open outp file
                        if (fdout == -1) {
                            perror ("open out");
                            exit(EXIT_FAILURE);
                        }
                        if (-1 == dup2(fdout, STDOUT_FILENO)) {     //duplication for output
                            perror("dup out");
                            exit(EXIT_FAILURE);
                        }
                        a[out] = NULL;
                    }
                    if (inp_flag == 1){      //if inp-way was directed
                        int fdin = open(a[inp], O_RDONLY);  //open inpt file
                        if (fdin == -1) {
                            perror ("open inp");
                            exit(EXIT_FAILURE);
                        }                        
                        if (-1 == dup2(fdin, STDIN_FILENO)) { //dupliication for input
                            perror ("dup inp");
                            exit(EXIT_FAILURE);
                        }
                        a[inp] = NULL;
                    }                     
                    err_exec = execvp(a[0],a);  //execution of user's commands
                    if (err_exec == -1) {
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCESS);
                }
            }
            pid = wait(NULL);           //Finish of branching
            if (pid == -1) {
                perror("wait");
            }
            printf("$");
            i = 0; j = 0;
            inp_flag = 0; out_flag = 0;
            continue;                 //User, I wait your new command
        }
    /*-------------------------------------------------------------------------------*/
        if (ch == ' ') {              //If space was readed
            argv[i][j] = '\0';        //add null-terminator in the end of string
            ++i;                      //Ok, next sring
            j = 0;
            continue;                 //reading again
        }

        if (ch == '<') {                  //redirection of input
            inp = i;
            if ((ch = getchar()) != ' ') { //if first symbol isn't space
                argv[i][j] = ch;           //then write this symbol in string
                ++j;
            }
            ++inp_flag;                    //input was redirected
            continue;
        }

        if (ch == '>') {                  //redirection of output
            if (i == inp) {               //if after input redirection wasn't space
                argv[i][j] = '\0';
                ++i; j = 0;
            }
            out = i;
            if ((ch = getchar()) != ' ') {
                argv[i][j] = ch;
                ++j;
            }
            ++out_flag;                   //output was redirected
            continue;
        }
        
        argv[i][j] = ch;              //symbol was writed in string
        ++j;                          //Go to the next cymbol
    }
    printf("\n");
    return 0;
}

