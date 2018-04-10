
#include "base.h"

int main(int argc, char** argv)
{
  int cantImg=0;//• -c: cantidad de imágenes
  int umbralB=0;//• -u: UMBRAL para Binarizar la imagen.
  int umbralC=0;//• -n: UMBRAL para Clasificación
  char *bandera=NULL;//• -b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusión obtenida al leer la imagen binarizada.
  int c;
  opterr=0;
  while((c= getopt(argc,argv, "c:u:n:b:")) != -1)
    {
      switch(c)
      {
        case 'c':
          sscanf(optarg, "%d", &cantImg);
          break;
        case 'u':
          sscanf(optarg, "%d", &umbralB);
          break;
        case 'n':
          sscanf(optarg, "%d", &umbralC);//Esta variable se establece por getoptapuntar al valor del argumento de la opción, para aquellas opciones que aceptan argumentos.
          break;
        case 'b':
          bandera= optarg;
          break;
        case '?':
          if(optopt == 'b')
            fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
          else if(isprint(optopt))
            fprintf(stderr, "Opcion desconocida -%c\n", optopt);
          else
            fprintf(stderr, "Opcion con caracter desconocido ' \\x%x ' \n", optopt);
          return 1;
        default:
          abort();
      }
    } 
   printf("cantImg=%d , umbralB=%d , umbralC=%d , bandera=%s\n", cantImg, umbralB, umbralC, bandera);
   pipeline(cantImg,umbralB,umbralC,1);
  return 0;
}