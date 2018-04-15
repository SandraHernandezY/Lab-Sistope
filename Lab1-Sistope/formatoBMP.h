#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
//Archivo de cabecera
/*bmpFileHeader: Estructura pensada para almacenar de manera especifica los datos basicos de un archivo BMP.*/
typedef struct bmpFileHeader // 14 bytes -2 que se ocuparon para el identificador del formato
{/* 2 bytes de identificación */
  uint32_t size;        /* Tamaño del archivo*/
  uint16_t resv1;       /* Reservado */
  uint16_t resv2;       /* Reservado */
  uint32_t offset;      /* Offset distancia en bytes desde que termina la cabecera de información hasta que empiezan los datos
                          de la imagen.*/
} bmpFileHeader;
/*bmpInfoHeader: Estructura que tiene como objetivo almacenar los datos principales de la imagen que contienen*/
typedef struct bmpInfoHeader//40 bytes
{
  uint32_t headersize;    /* Tamaño de la cabecera */
  uint32_t width;         /* Ancho en pixeles*/
  uint32_t height;        /* Alto en pixeles*/
  uint16_t planes;        /* Planos de color (Siempre 1) */
  uint16_t bpp;           /* bits por pixel */
  uint32_t compress;      /* compresión */
  uint32_t imgsize;       /* tamaño de los datos de imagen en bytes*/
  uint32_t bpmx;          /* Resolución X en bits por metro */
  uint32_t bpmy;          /* Resolución Y en bits por metro */
  uint32_t colors;        /* colors used en la paleta */
  uint32_t imxtcolors;    /* Colores importantes. 0 si son todos */
} bmpInfoHeader;
// Declaraciones de funciones.
unsigned char *leerBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader* header);
void escribir(bmpFileHeader* header, bmpInfoHeader* info, int* img, int n);
unsigned int *ConversionEscalaGrises(bmpInfoHeader *info, unsigned char *img);
int *Binarizar(bmpInfoHeader *info, unsigned int *img, int umbral);
int clasificar(bmpInfoHeader *info, int *img, int umbral);
void pipeline(int c, int umbralB, int umbralC,int b);
void DisplayInfo(bmpInfoHeader *info);
void DisplayClasificacion(int*clasificaciones, int c);