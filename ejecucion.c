/*-----------------------------------------------------+      
 |     R E D I R E C C I O N . C                       |
 +-----------------------------------------------------+
 |     Version    :                                    |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |
 |     Descripcion:                                    |
 +-----------------------------------------------------*/
#include "defines.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int ejecutar (int nordenes , int *nargs , char **ordenes , char ***args , int bgnd){
	pid_t pid;
    int status;

    for (int i = 0; i < nordenes; i++) {
        pid = fork();
        if (pid == 0) { // Child process
            redirigir_entrada(i);
            redirigir_salida(i);
            execvp(ordenes[i], args[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            return -1;
        }
    }
    cerrar_fd();

    if (!bgnd) {
        for (int i = 0; i < nordenes; i++) {
            wait(&status);
        }
    }

    return 0;
} // Fin de la funcion "ejecutar"
