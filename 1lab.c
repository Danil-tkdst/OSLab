/* This programm implement a basic functional of bash-terminal.
  Made by Danil Mikhailov, student pm-2 groupe of MSTUCA in 2020 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 80
#define MAXWORD 16

int main()
{
    char argv[MAXWORD][MAXLINE];
    char *a[MAXWORD];
    int i = 0, j = 0, k, err_exec, ch;
    
    printf("$");
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            argv[i][j] = '\0';
            k = i;
            for (i = 0; i <= k; ++i) {
                a[i] = argv[i];
            }
            a[i] = NULL;
            pid_t pid = fork();
            if (pid == -1){
                perror("fork");
            }
            else {
                if (!pid) {
                    err_exec = execvp(a[0],a);
                    if (err_exec == -1) {
                        perror("execvp");
                    }
                }
            }
            pid = wait(NULL);
            if (pid == -1) {
                perror("wait");
            }
            printf("$");
            i = 0; j = 0;
            continue;
        }
        if (ch == ' ') {
            argv[i][j] = '\0';
            ++i;
            j = 0;
            continue;
        }
        argv[i][j] = ch;
        ++j;
    }
    return 0;
}
