
#include "formatoBMP.h"
/*Entrada:
        info: puntero a la estructura la cual contiene la informacion de la imagen a convertir.
        img: lista de unsigned char la cual contiene la imagen.
Salida: grises: lista de enteros que contiene la imagen recibida pero en escala de grises.
Descripcion: Tomamos los datos recibidos por parametro y procedemos a recorrer la imagen y a calcular
        los datos para obtener el valor en escala de grises pedido una vez obtenido el valor se procede
        a almacenar la informacion en la lista que sera retornada una vez completado el recorrido
        de la imagen entregada.*/
unsigned int *ConversionEscalaGrises(bmpInfoHeader *info, unsigned char *img)
{
  int x, y,i,valor,r,g,b;;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  unsigned int* grises =(unsigned int*)malloc(sizeof(unsigned int*)*info->height*info->width);

  int total=info->height*info->width; /*calculamos el total de pixeles de la imagen*/
  int cant_colores=(info->bpp/8);     /*obtenemos la cantidad de bytes por pixel*/
   x =info->height * info->width*cant_colores;
    int j =0;
    for (i=0; i<x; i=i+cant_colores){
        b=img[i];
        g=img[i+1];
        r=img[i+2];
        valor = r*0.3 + g*0.59 + b*0.11;
        grises[j]=valor;
        j++;
    }
    return grises;
}

/*Entrada:
      info: puntero a la estructura que contiene la informacion de la imagen a binarizar.
      img: lista de entero que contiene la imagen en escala de grises la cual sera binarizada.
      umbral:entero a utilizar para definir el umbral de imagen.
Salida: lista de enteros la cual contendra la imagen binarizada.
Descripcion: Se reserva memoria para almacenar la nueva imagen binarizada, el tamaño sera igual
      a la cantidad de pixeles totales de la imagen entregada luego se procede a recorrer la imagen
      recibida por parametro y se consulta si es mayor que el umbral entregado se ser asi se almacena
      un 1 y de lo contrario un 0, una vez finalizado el recorrido de la imagen se retorna la lista
      con los datos de la binarizacion.*/
int *Binarizar(bmpInfoHeader *info, unsigned int *img, int umbral)
{
  int x;
  int* binarizado =(int*)malloc(sizeof(int)*info->height*info->width);
  for (x=0; x<(info->width*info->height); x++)
  {
    if((int)img[x]>umbral){
      binarizado[x]=1;
    }
    else{
      binarizado[x]=0;
    }
  }
  return binarizado;
}

/*Entrada:
      info:puntero a la estructura que contiene la informacion de la imagen a clasificar.
      img: lista de enteros que contiene los datos binarizados de la imagen.
      umbral: entero a ocupar para clasificar.
Salida: 1: entero que representa que la imagen tiene mas porcetaje de negro que el umbral.
        0: entero que indica que el procentaje de negro fue menor que el umbral entregado.
Descripcion: La funcion procede a recorrer la imagen binarizada recibida (img) y a contar los
        unos y ceros que encuentre estos representan negro y blanco respectivamente. Luego se
        calcula el procentaje de negro encontrado en la imagen para finalizar comparando
        el resultado obtenido con el umbral entregado de ser mayor se retorna un 1 se lo contario
        se entrega un cero*/
int clasificar(bmpInfoHeader *info, int *img, int umbral)
{
   int i ;
   int negro =0;
   int blanco =0;
      for (i=0;i<(info->height*info->width);i++){
        if (img[i]==0)
          negro ++;
        else
          blanco ++;
      }
    float porcentaje = ((float)negro/(float)(negro+blanco)) * 100;
    if (porcentaje>umbral){
         return 1;
    }else{
        return 0;
    }
}

/*Entrada:
        c: cantidad de imagenes entregadas.
        umbralB: UMBRAL para Binarizar la imagen.
        umbralC: UMBRAL para Clasificación.
        b: bandera que indica si se deben mostrar los resultados por pantalla, es decir, la conclusión obtenida al leer la imagen binarizada.
Descripcion: Funcion encargada de llamar secuencialmente las funciones previamentes definidas para realizar
        las transformaciones correspondientes a la imagen. Se procede por declarar las estructuras a utilizar, luego
        se realiza un ciclo while el cual se detendra una vez que todas las imagenes hayan sido estudiadas. Para comenzar se procede a llamar
        a la funcion encargada de llenar nuestras estructuras y la que nos entregara la imagen con la que trabajaremos, luego 
        tanto la imagen como las estructuras son pasadas como parametros a la funcion encargada de entregarnos una imagen en escala de grises
        luego la imagen obtenida se pasa como parametro a la funcion Binarizar, luego se procede a llamar a la funcion
        encargada de clasificar la imagen y para finalizar se escribe el archivo bmp con la imagen trabajada.
        */
void pipeline(int c, int umbralB, int umbralC,int b)
{
  int i;
  bmpInfoHeader info; 
  bmpFileHeader header;
  unsigned char* img;
  unsigned int* grises;
  int* binarizado;
  int* clasificaciones=(int*)malloc(sizeof(int)*c);
  char file[200] = "Imágenes/imagen_";
     
  int cont=1; //contador de imagenes
  while(cont<=c){
    sprintf(file,"Imágenes/imagen_%d.bmp",cont);
    img=leerBMP(file, &info,&header);               //llenamos info y retornamos imagen
    //DisplayInfo(&info);
    grises=ConversionEscalaGrises(&info, img);
    binarizado=Binarizar(&info,grises,umbralB);
    clasificaciones[cont-1]=clasificar(&info,binarizado,umbralC);
    escribir(&header,&info,binarizado,cont);
    cont++;
  }
  if(b==1){/* Si la bandera es 1 se procede a mostrar el resultado de la clasificacion de todas las imagenes.*/
    DisplayClasificacion(clasificaciones,c); 
  }
}

/*Entrada: 
      info: puntero a estructura que contiene todos los datos de la imagen
Descripcion: Esta funcion tiene como objetivo mostrar los datos almacenados los cuales deben corresponder
      con la informacion obtenida de un archivo bmp*/
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

/*Entrada:
      clasificaciones: lista de enteros que contiene 0 o 1 dependiendo si la imagen cumple o no con los
                      las caracteristicas entregas(umbrales).
      c: entero que indica la cantidad de imagen.
Descripcion: Funcion que muestra por consola las clasificaciones de las imagenes estudiadas.*/
void DisplayClasificacion(int*clasificaciones, int c)
{
  int i;
  printf("   Imagen       |       Nearly Black\n");
  for(i=0; i<c;i++){
    if(clasificaciones[i]==1){
      printf("  imagen_%d       |     yes     \n",i+1);
    }
    else{
      printf("  imagen_%d       |     no     \n",i+1);
    }
  }
}



