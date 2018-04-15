/*Este archivo contiene todas las funciones que utilizan arcvhivos en su implementacion.
Cabe destacar que para este labarotorio los archivos con los que se trabaja son archivo BMP*/
#include "formatoBMP.h"
/*Entrada:
        filename: cadena de caracteres que forman el nombre del archivo a leer el cual contendra la imagen a analizar.
        bInfoHeader: estructura previamente definida para almacenar informacion sobre la imagen.
        header: estructura previamente definida para contener la informacion mas basica del archivo.
Salida: cadena de caracteres la cual contedra solo la imagen a estudiar.
Descripcion: La funcion carga los datos que obtiene al leer el archivo y carga las estructuras que recibe
        segun corresponda. Los primeros 2 bytes son obtenidos para comprobar el formato del archivo luego los 12 bytes siguientes 
        del archivo corresponden a los datos a guardar en header, los 40 bytes siguientes seran los datos a cargar en 
        bInfoHeader, los bytes restantes seran retornados en una cadena estos representan a la imagen.*/
unsigned char* leerBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader* header)
{
  FILE *f;
  unsigned char *imgdata;   /* datos de la imagen */
  uint16_t type;            /* variable que contendra los 2 bytes identificativos */

  f=fopen (filename, "r");
  if (!f)
    return NULL;            /* Si no podemos leer, no hay imagen*/

  fread(&type, sizeof(uint16_t), 1, f);     /* Leemos los dos primeros bytes */
  if (type !=0x4D42)                        /* Comprobamos el formato */
    {
      fclose(f);
      return NULL;
    }
  /* Leemos la header de fichero completa */
  fread(header, sizeof(bmpFileHeader), 1, f);

  /* Leemos la header de la información completa de la imagen*/
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen, tanto como indique imgsize */
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);//bytes

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la header de fichero*/
  fseek(f, header->offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  fread(imgdata, bInfoHeader->imgsize,1, f);

  fclose(f);

  /* Retornamos la imagen */
  return imgdata;
}



/*Entrada:
        header: puntero a la estructura que contiene la informacion del archivo.
        info: puntero a la estructura que tiene la informacion de la imagen. 
        img: lista de tipo entero que contiene la imagen binarizada
        n: identificador de la imagen que se quiere escribir (Imagen_n)
Salida: Archivo bmp el cual contiene la imagen en escala de grises.
Descripcion: Esta funcion tiene como principal objetivo escribir un archivo en formato bmp, el cual contendra la
        nueva imagen. Los primeros dos bytes corresponden al identificador del formato, los 52 bytes siguientes a escribir son
        los entregados en las estructuras recibidas por parametro, los bytes restantes a escribir dependeran de la lectura
        de la imagen binarizada y de la cantidad de pixeles de la imagen.*/
void escribir(bmpFileHeader* header, bmpInfoHeader* info, int* img, int n)
{
  FILE *f;
  char file[200] = "salida_imagen_";
  sprintf(file,"salida_imagen_%d.bmp",n);
  f=fopen(file, "wb");
  uint16_t type = 0x4D42;
  unsigned char cero=0;
  unsigned char p= 255;

  /* Escribimos los dos primeros bytes (Formato BM)*/
  fwrite(&type, sizeof(uint16_t), 1, f); 
  /* Escribimos el header del fichero */
  fwrite(&header->size, 4, 1, f);
  fwrite(&header->resv1, 2, 1, f);
  fwrite(&header->resv2, 2, 1, f);
  fwrite(&header->offset, 4, 1, f);
  /* Escr&ibimos la header de información*/
  fwrite(&info->headersize, 4, 1, f);
  fwrite(&info->width, 4, 1, f);
  fwrite(&info->height, 4, 1, f);
  fwrite(&info->planes, 2, 1, f);
  fwrite(&info->bpp, 2, 1, f);
  fwrite(&info->compress, 4, 1, f);
  fwrite(&info->imgsize, 4, 1, f);
  fwrite(&info->bpmx, 4, 1, f);
  fwrite(&info->bpmy, 4, 1, f);
  fwrite(&info->colors, 4, 1, f);
  fwrite(&info->imxtcolors, 4, 1, f);
  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la header de fichero, para luego escribir la imagen*/
  fseek(f, header->offset, SEEK_SET);
  int i;
  /*Empezamos a leer la imagen binarizada y a escribir segun corresponda*/
  for(i=0;i<info->width *info->height;i++){

     if (img[i]==1){//  ---------------Si leemos un 1------
       if (info->bpp ==24){       //tres bytes por pixel
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);

       }
       else{                      //cuatro bytes por pixel
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
          fwrite(&p,sizeof(unsigned char),1,f);
       }
     }
     else{//  ------------------------Si leemos un 0-------
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
