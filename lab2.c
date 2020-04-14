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
    char *a[MAXWORD], *rdr_str; 
    int i = 0, j = 0, k, err_exec, ch, inp, out, check = 0;
    
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
                    if ((check == 2) || (check == 3)) {     //if out-way was directed
                        rdr_str = argv[out];
                        a[out] = NULL;
                        int fdout = open(rdr_str, O_WRONLY | O_CREAT);  //open outp file
                        if (fdout == -1) {
                            perror ("open out");
                            EXIT_FAILURE;
                        }
                        if (-1 == dup2(fdout, STDOUT_FILENO)) {     //duplication for output
                            perror("dup out");
                            EXIT_FAILURE;
                        }
                    }
                    if ((check == 1) || (check == 3)){      //if inp-way was directed
                        rdr_str = argv[inp];
                        a[inp] = NULL;
                        int fdin = open(rdr_str, O_RDONLY);  //open inpt file
                        if (fdin == -1) {
                            perror ("open inp");
                            EXIT_FAILURE;
                        }
                        if (-1 == dup2(fdin, STDIN_FILENO)) { //dupliication for input
                            perror ("dup inp");
                            EXIT_FAILURE;
                        --k;
                        }
                    }                     
                    err_exec = execvp(a[0],a);  //execution of user's commands
                    if (err_exec == -1) {
                        perror("execvp");
                    }
                }
            }
            pid = wait(NULL);           //Finish of branching
            if (pid == -1) {
                perror("wait");
            }
            printf("$");
            i = 0; j = 0; check = 0;
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
            check = 1;                     //input was redirected
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
            check += 2;                    //output was redirected
            continue;
        }
        
        argv[i][j] = ch;              //symbol was writed in string
        ++j;                          //Go to the next cymbol
    }
    printf("\n");
    return 0;
}
