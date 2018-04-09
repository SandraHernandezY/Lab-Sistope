
#include "base.h"
//PIPELINE

int clasificar(bmpInfoHeader *info, int *img, int umbral){
   int i ;
   int negro =0;
   int blanco =0;
      for (i=0;i<(info->height*info->width);i++){
        if (img[i]==0){
          blanco ++;
        }
        else{
          negro ++;
        }
      }
    int porcentaje = (negro*100)/(negro+blanco);
    if (porcentaje>=umbral){
         return 1;
    }else{
        return 0;
    }
}

int* Binarizar(bmpInfoHeader *info, unsigned char *img, int umbral)
{
  int x, y;
  int* binarizado =(int*)malloc(sizeof(int)*info->height*info->width);
  for (x=0; x<(info->width*info->height); x++)
  {
    if(img[x]>umbral){
      binarizado[x]=1;
    }
    else{
      binarizado[x]=0;
    }
  }
  return binarizado;
}
unsigned char* ConversionEscalaGrises(bmpInfoHeader *info, unsigned char *img)
{
  int x, y,i,valor;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  unsigned char* grises =(unsigned char*)malloc(info->height*info->width);
  int r,g,b;
  i=0;
  /* Dibujamos la imagen */
  for (y=info->height; y>0; y--)
    {
      for (x=0; x<info->width; x++)
    {
      b=(img[3*(x+y*info->width)]);
      g=(img[3*(x+y*info->width)+1]);
      r=(img[3*(x+y*info->width)+2]);
      valor= r*0.3+ g*0.59 + b*0.11;
      grises[i]=valor;
      //printf("grises[%d]=%d ",i,grises[i]);
      i++;
    }
      //printf("\n");
    }
    return grises;
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
void DisplayClasificacion(int*clasificaciones, int c)
{
  int i;
  printf("   Imagen       |       Nearly Black\n");
  for(i=0; i<c;i++){
    if(clasificaciones[i]==1){
      printf("imagen_%d       |     yes     \n",i+1);
    }
    else{
      printf("imagen_%d       |     no     \n",i+1);
    }
  }
}

int main(int argc, char** argv)
{
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
   pipeline(cantImg,umbralB,umbralC,bandera);
  return 0;
}

void escribir(){
  printf("fin de pipeline\n");
}
void pipeline(int c, int umbralB, int umbralC,int b){
  bmpInfoHeader info;
  unsigned char* img;
  unsigned char* grises;
  int* binarizado;
  int* clasificaciones=(int*)malloc(sizeof(int)*c);
  char file[128] = "Imágenes/imagen_";
     
  int cont=1;//contador de imagenes
  while(cont<=c){
    sprintf(file,"Imágenes/imagen_%d.bmp",cont);
   // strcat(file, ".bmp");
    img=LoadBMP(file, &info);//llenamos info y retornamos imagen
    DisplayInfo(&info);
    grises=ConversionEscalaGrises(&info, img);
    binarizado=Binarizar(&info,grises,umbralB);
    clasificaciones[cont-1]=clasificar(&info,binarizado,umbralC);
    escribir();
    cont++;
  }
  b=1;
  if(b==1){
    DisplayClasificacion(clasificaciones,c);
  }
}

