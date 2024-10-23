/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                        
 |     Autor :   
 |     Asignatura :  SOP-GIIROB                                                       
 |     Descripcion: 
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

CMDFD cmdfd;


void redireccion_ini(void){
	for (int i = 0; i < PIPELINE; i++) {
        	cmdfd[i].infd = STDIN_FILENO;
        	cmdfd[i].outfd = STDOUT_FILENO;
    	}
}//Inicializar los valores de la estructura cmdfd



int pipeline(int ncmd, char * infile, char * outfile, int append, int bgnd){
redireccion_ini();

    if (infile && strlen(infile) > 0) {
        cmdfd[0].infd = open(infile, O_RDONLY);
        if (cmdfd[0].infd < 0) {
            perror("open infile");
            return -1;
        }
    }

    if (outfile && strlen(outfile) > 0) {
        int flags = O_WRONLY | O_CREAT;
        flags |= append ? O_APPEND : O_TRUNC;
        cmdfd[ncmd - 1].outfd = open(outfile, flags, 0644);
        if (cmdfd[ncmd - 1].outfd < 0) {
            perror("open outfile");
            return -1;
        }
    }
    for (int i = 0; i < ncmd - 1; i++) {
        int pipefd[2];
        if (pipe(pipefd) < 0) {
            perror("pipe");
            return -1;
        }
        cmdfd[i].outfd = pipefd[1];
        cmdfd[i + 1].infd = pipefd[0];
    }

    if (bgnd) {
        cmdfd[0].infd = open("/dev/null", O_RDONLY);
    }

    return 0;
} // Fin de la función "pipeline"



int redirigir_entrada(int i){
	if (cmdfd[i].infd != STDIN_FILENO) {
        if (dup2(cmdfd[i].infd, STDIN_FILENO) < 0) {
            perror("dup2");
            return -1;
        }
        close(cmdfd[i].infd);
    	}
    	return 0;
} // Fin de la función "redirigir_entrada"



int redirigir_salida(int i){
	if (cmdfd[i].outfd != STDOUT_FILENO) {
        if (dup2(cmdfd[i].outfd, STDOUT_FILENO) < 0) {
            perror("dup2");
            return -1;
        }
        close(cmdfd[i].outfd);
   	}
    	return 0;
} // Fin de la función "redirigir_salida"


int cerrar_fd(){
	for (int i = 0; i < PIPELINE; i++) {
        if (cmdfd[i].infd != STDIN_FILENO) {
            close(cmdfd[i].infd);
        }
        if (cmdfd[i].outfd != STDOUT_FILENO) {
            close(cmdfd[i].outfd);
        }
    	}
    	return 0;
} // Fin de la función "cerrar_fd"


