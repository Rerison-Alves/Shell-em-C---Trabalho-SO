#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_NUM_ARGS 10
#define TRUE 1

extern int errno;

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_NUM_ARGS];
    int num_args;

    while (TRUE) {
      //Indica inicio do comando com $
        printf("$ ");
      //Leitura do comando
        fgets(command, MAX_COMMAND_LENGTH, stdin);
      // Remove o caractere de nova linha
        command[strlen(command)-1] = '\0';  

        // Extrai os argumentos do comando
        num_args = 0;
        args[num_args] = strtok(command, " ");
        while (args[num_args] != NULL && num_args < MAX_NUM_ARGS-1) {
            num_args++;
            args[num_args] = strtok(NULL, " ");
        }
        args[num_args] = NULL;
      
      //Define comando cd
        if (strcmp(args[0], "cd") == 0) {
            if (num_args != 2) {
                printf("Uso: cd <diretório>\n");
            } else {
                if (chdir(args[1]) != 0) {
                    printf("Erro ao mudar de diretório\n");
                }
            }
      //Define comando exit para sair do interpretador
        } else if (strcmp(args[0], "exit") == 0) {
            break;
      //Se não for cd nem exit executar comando com execvp
        } else {
          // Cria um novo processo com fork
            pid_t pid = fork(); 
          
            if (pid == 0) {  // Processo filho para pid 0
              //Verifica se houve erro
              //Se houve erro no execvp ele irá salvar em errono
                if (execvp(args[0], args) < 0) { 
                  //Determina tipo do erro com switch
                  switch (errno) {
                    case EACCES:
                        printf("Permissão negada\n");
                        break;
                    case ENOENT:
                        printf("Comando não encontrado\n");
                        break;
                    default:
                        printf("Erro ao executar o comando\n");
                  }
                  exit(1);
                  }
            } else if (pid > 0) {  // Processo pai para pid positivo
              //Pai aguarda que o processo filho
                wait(NULL);
            } else {
              //Pid negativo, logo houve erro
                printf("Erro ao criar processo filho\n");
            }
        }
    }

    return 0;
}