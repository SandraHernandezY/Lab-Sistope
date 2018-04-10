
#include "base.h"


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

int* Binarizar(bmpInfoHeader *info, unsigned int *img, int umbral)
{
  int x;
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
unsigned int* ConversionEscalaGrises(bmpInfoHeader *info, unsigned char *img)
{
  int x, y,i,valor;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  unsigned int* grises =(unsigned int*)malloc(sizeof(unsigned int*)*info->height*info->width);
  int r,g,b;
  i=0;
  int cant_colores=(info->bpp/8);
  printf("cantidad de colores = %d\n", cant_colores);
  /* Recorremos la imagen */
  for (y=info->height; y>0; y--)
    {
      for (x=0; x<info->width; x++)
    {
      b=(img[cant_colores*(x+y*info->width)]);
      g=(img[cant_colores*(x+y*info->width)+1]);
      r=(img[cant_colores*(x+y*info->width)+2]);
      valor= r*0.3+ g*0.59 + b*0.11;
      grises[i]=valor;
      //printf("grises[%d]=%d ",i,grises[i]);
      i++;
    }
      //printf("\n");
    }
    return grises;
}

unsigned char* LoadBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader* header)
{
  FILE *f;
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
  fread(header, sizeof(bmpFileHeader), 1, f);

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen, ¿cuánta? 
     Tanto como indique imgsize */
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header->offset, SEEK_SET);

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

void escribir(bmpFileHeader* cabecera, bmpInfoHeader* info, int* img, int n){
  FILE *f;
  char file[200] = "salida_imagen_";
  sprintf(file,"salida_imagen_%d.bmp",n);
  f=fopen(file, "w");
  uint16_t type = 0x4D42;
  unsigned char cero=0;
  unsigned char p= 255;

  /* Escribimos los dos primeros bytes */
  fwrite(&type, sizeof(uint16_t), 1, f);//Formato BM

  /* Escribimos la cabecera de fichero completa */
  fwrite(cabecera, sizeof(bmpFileHeader), 1, f);

  /* Escribimos la cabecera de información completa */
  fwrite(info, sizeof(bmpInfoHeader), 1, f);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, cabecera->offset, SEEK_SET);
  int i;
  for(i=0;i<info->width *info->height;i++){
     if (img[i]==1){
       if (info->bpp ==24){//tres colores
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);

       }
       else{//cuatro
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
       }
     }
     else{//  0-------------------------------
      if (info->bpp ==24){
          fwrite(&cero,sizeof(unsigned char),1,f);
          fwrite(&cero,sizeof(unsigned char),1,f);
          fwrite(&cero,sizeof(unsigned char),1,f);

       }
       else{
          fwrite(&cero,sizeof(unsigned char),1,f);
          fwrite(&cero,sizeof(unsigned char),1,f);
          fwrite(&cero,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
       }

     }
  }
  fclose(f);
  
}
void pipeline(int c, int umbralB, int umbralC,int b){
  bmpInfoHeader info;
  bmpFileHeader header;
  unsigned char* img;
  unsigned int* grises;
  int* binarizado;
  int* clasificaciones=(int*)malloc(sizeof(int)*c);
  char file[200] = "Imágenes/imagen_";
     
  int cont=1;//contador de imagenes
  while(cont<=c){
    sprintf(file,"Imágenes/imagen_%d.bmp",cont);
    img=LoadBMP(file, &info,&header);//llenamos info y retornamos imagen
    DisplayInfo(&info);
    grises=ConversionEscalaGrises(&info, img);
    printf("Fuera de escala de grises\n");
    binarizado=Binarizar(&info,grises,umbralB);
    printf("Fuera de Binarizar\n");
    clasificaciones[cont-1]=clasificar(&info,binarizado,umbralC);
    escribir(&header,&info,binarizado,cont);
    cont++;
  }
  if(b==1){
    DisplayClasificacion(clasificaciones,c);
  }
}

