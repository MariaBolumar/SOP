/*-----------------------------------------------------+
 |     U S H. C                                        
 +-----------------------------------------------------+
 |     Versión :                                       |                      
 |     Autor :                                         |
 |     Asignatura :  SOP-GIIROB                        |                               
 |     Descripción :                                   |
 +-----------------------------------------------------*/
#include "defines.h"
#include "analizador.h"
#include "redireccion.h"
#include "ejecucion.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "profe.h"

#include <stdio.h>


//
// Declaraciones de funciones locales
//
void visualizar( void );
int leerLinea( char *linea, int tamanyLinea );


//
// Prog. ppal.
// 
int main(int argc, char * argv[])
{
    
  char line[255];
  int res;
  char **m_ordenes;
  char ***m_argumentos;
  int *m_num_arg;
  int m_n;

  while(1){
    
    do{
        res=leerLinea(line,MAXLINE);    
        if (res==-2) {
      		fprintf(stdout,"logout\n");
		      exit(0);
    	}
	if (res==-1){
	 	fprintf(stdout,"linea muy larga\n")
	}
     }while(res<=0);

     if (analizar(line)==OK){
	m_n=num_ordenes();
	m_num_arg=num_argumentos();
	m_ordenes=get_ordenes();
	m_argumentos=get_argumentos();
	if(m_n>0){
		if (pipeline(m_n,fich_entrada(),fich_salida(),es_append(),es_background())==OK)
                    ejecutar(m_n,m_num_arg,m_ordenes,m_argumentos,es_background());
        }

       visualizar();
     }
 }    

  return 0;
}





/****************************************************************/
/*                       leerLinea                             
  --------------------------------------------------------------
                                                               
   DESCRIPCIÓN:                                                 
   Obtiene la línea de órdenes para el mShell.    
   Util para depuracion.                                        
                                                                
   ENTRADA:                                                 
    linea - puntero a un vector de carácteres donde se almancenan los caracteres 
   leídos del teclado
    tamanyLinea - tamaño máximo del vector anterior

   SALIDA:
    -- linea - si termina bien, contiene como último carácter el retorno de carro.
    -- leerLinea -  Entero que representa el motivo de finalización de la función:
     > 0 - terminación correcta, número de caracteres leídos, incluído '\n'
     -1 - termina por haber alcanzado el número máximo de caracteres que se 
    espera leer de teclado, sin encontrar '\n'.
     -2 - termina por haber leído fin de fichero (EOF).
*/
/****************************************************************/
//char * getline(void)
int leerLinea( char *linea, int tamanyLinea )
{
	char cwd[80];
    getcwd(cwd, sizeof(cwd));
    printf("%s%s", PROMPT, cwd);

    int c;
    char *p = linea;
    for (int i = 0; i < tamanyLinea; i++) {
        c = getchar();
        if (c == '\n') {
            *p = c;
            p++;
            break;
        } else if (c == EOF) {
            return -2;
        } else {
            *p = c;
            p++;
        }
    }

    if (p - linea >= tamanyLinea) {
        return -1;
    }
    *p = '/0';

    return p- linea;	
}




/****************************************************************/
/*                       visualizar                             */
/*--------------------------------------------------------------*/
/*                                                              */
/* DESCRIPCIÓN:                                                 */
/* Visualiza los distintos argumentos de la orden analizada.    */
/* Util para depuracion.                                        */
/*                                                              */
/* ENTRADA: void                                                */
/*                                                              */
/* SALIDA: void                                                 */
/*                                                              */
/****************************************************************/
void visualizar( void )
{  
	int nord = num_ordenes();
    char **ord = get_ordenes();
    int *narg = num_argumentos();
    char ***arg = get_argumentos();
    char *ent = fich_entrada();
    char *sal = fich_salida();
    int append = es_append();
    int bgnd = es_background();

    printf("Número de órdenes: %d\n", nord);
    for (int i = 0; i < nord; i++) {
        printf("Orden %d: %s\n", i, ord[i]);
        printf("Número de argumentos: %d\n", narg[i]);
        for (int j = 0; j < narg[i]; j++) {
            printf("Argumento %d: %s\n", j, arg[i][j]);
        }
    }

    if (ent && strlen(ent) > 0) {
        printf("Redirección de entrada: %s\n", ent);
    }
    if (sal && strlen(sal) > 0) {
        printf("Redirección de salida: %s\n", sal);
        printf("Modo Append: %s\n", append ? "Sí" : "No");
    }

    printf("Ejecución en segundo plano: %s\n", bgnd ? "Sí" : "No");

   
} // Fin de "visualizar"



