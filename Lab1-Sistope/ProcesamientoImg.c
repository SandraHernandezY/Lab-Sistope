
#include "base.h"


void TextDisplay(bmpInfoHeader *info, unsigned char *img)
{
  int x, y;
  /* Reducimos la resolución vertical y horizontal para que la imagen entre en pantalla */
  static const int reduccionX=6, reduccionY=4;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  static const int umbral=90;
  /* Asignamos caracteres a los colores en pantalla */
  static unsigned char colores[9]=" bgfrRGB";
  int r,g,b;

  /* Dibujamos la imagen */
  for (y=info->height; y>0; y-=reduccionY)
    {
      for (x=0; x<info->width; x+=reduccionX)
    {
      b=(img[3*(x+y*info->width)]>umbral);
      g=(img[3*(x+y*info->width)+1]>umbral);
      r=(img[3*(x+y*info->width)+2]>umbral);

      printf("%c", colores[b+g*2+r*4]);
    }
      printf("\n");
    }
}

unsigned char* LoadBMP(char *filename, bmpInfoHeader *bInfoHeader)
{

  FILE *f;
  bmpFileHeader header;     /* cabecera */
  unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */

  f=fopen (filename, "r");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

  /* Leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42)        /* Comprobamos el formato */
    {
      fclose(f);
      return NULL;
    }

  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(bmpFileHeader), 1, f);

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen, ¿cuánta? 
     Tanto como indique imgsize */
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header.offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  fread(imgdata, bInfoHeader->imgsize,1, f);

  /* Cerramos */
  fclose(f);

  /* Devolvemos la imagen */
  return imgdata;
}

void DisplayInfo(bmpInfoHeader *info)
{
  printf("Tamaño de la cabecera: %u\n", info->headersize);
  printf("Anchura: %d\n", info->width);
  printf("Altura: %d\n", info->height);
  printf("Planos (1): %d\n", info->planes);
  printf("Bits por pixel: %d\n", info->bpp);
  printf("Compresión: %d\n", info->compress);
  printf("Tamaño de datos de imagen: %u\n", info->imgsize);
  printf("Resolucón horizontal: %u\n", info->bpmx);
  printf("Resolucón vertical: %u\n", info->bpmy);
  printf("Colores en paleta: %d\n", info->colors);
  printf("Colores importantes: %d\n", info->imxtcolors);
}

int main(int argc, char** argv)
{
  bmpInfoHeader info;
  unsigned char* img;
  img=LoadBMP("Imágenes/imagen_1.bmp", &info);
  DisplayInfo(&info);
  TextDisplay(&info, img);

  int cantImg=0;//• -c: cantidad de imágenes
  int umbralB=0;//• -u: UMBRAL para Binarizar la imagen.
  int umbralC=0;//• -n: UMBRAL para Clasificación
  char *bandera=NULL;//• -b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusión obtenida al leer la imagen binarizada.
  int index;
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
  return 0;
}
