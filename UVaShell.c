//Fernando San José Domínguez
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#define error_message "An error has ocurred\n"

char**separar(char*);
void cambiarDirectorio(char**);
int checkRedireccion(char**);

int main (int argc, char *argv[]){

        if(argc >2 ){
                fprintf(stderr,"%s", error_message);
                exit(1);
        }

                char *Linea =malloc(200*sizeof(char));
                char**argumentos;
                int redireccion;
                pid_t p_Hijo;
                int wstatus;

//modo interactivo
        if(argc == 1){
                while(1){

                        printf("UVash> ");
                                scanf("\n%[^\n]", Linea);
                                argumentos=separar(Linea);
                                redireccion = checkRedireccion(argumentos);

                                //redireccionar
                                if(redireccion!=-1){
                                }
                                //imprimir por pantalla
                                else{
                                        if(strcmp(argumentos[0],"cd")==0){
                                                cambiarDirectorio(argumentos);
                                        }
                                                p_Hijo= fork();
                                                if(p_Hijo==0){//ejecuta hijo
                                                        if((execvp(argumentos[0],argumentos)<0)){
                                                                fprintf(stderr,"%s",error_message);
                                                                exit(1);
                                                        }else{
                                                                execvp(argumentos[0],argumentos);
                                                        }
                                                }else if(p_Hijo <0){
                                                        fprintf(stderr, "%s", error_message);
                                                }else{//espera al padre
                                                        waitpid(p_Hijo, &wstatus, WUNTRACED);
                                                }
                                }

                }

        }

//modo batch
        else{
                FILE *fichOpen = fopen(argv[1], "r");
                char * almacen=malloc(50*sizeof(char));
                if (fichOpen==NULL){
                        fprintf(stderr, "%s", error_message);
                        exit(1);
                }
                while(fscanf(fichOpen,"%[^\n] ", almacen)!=EOF){
                        argumentos=separar(almacen);
                        if(strcmp(argumentos[0], "cd")==0){
                                cambiarDirectorio(argumentos);
                        }

                        p_Hijo=fork();
                        if(p_Hijo==0){
                                if((execvp(argumentos[0], argumentos)<0)){
                                        fprintf(stderr, "%s", error_message);
                                        exit(1);
                                 }else{
                                        execvp(argumentos[0], argumentos);
                                }

                        }else if(p_Hijo <0){
                                fprintf(stderr, "%s", error_message);
                                exit(1);

                        }else{
                                waitpid(p_Hijo, &wstatus, WUNTRACED);
                        }
                }

        fclose(fichOpen);
        }
        exit(0);
}


//Separar string de comandos y argumentos
char** separar(char*Linea){
        char ** argumentos = malloc(50* sizeof(char));
        char * comando;
        int i=0;

        comando = strtok(Linea, " ");
        while(comando!=NULL){
                if(strcmp(comando,"exit")==0 || strcmp(Linea,"exit\n")==0){
                        exit(1);
                }
                argumentos[i]=comando;
                i++;
                comando=strtok(NULL, " ");

        }
        argumentos[i]=NULL;
        return argumentos;
}


//comando para redireccionar directorio
void cambiarDirectorio(char**argumentos){
        int ret;
        if((argumentos[1] != NULL) && (argumentos[2] != NULL)){
                fprintf(stderr, "%s", error_message);
                
        }else{
                if(argumentos[1]!=NULL){
                        ret=chdir(argumentos[1]);
                        if(ret !=0){
                                fprintf(stderr, "%s", error_message);
                        }
                }else{
                        fprintf(stderr,"%s",error_message);
                }
        }

        free(argumentos);
}


//validar la redireccion
int checkRedireccion(char**argumentos){
        int num = -1;
        int i = 0;
        while(argumentos[i]!=NULL){
                if(strcmp(argumentos[i],">")==0){
                        num=1;
                        i++;
                }else{
                        i++;
                }
        }
        return num;
}