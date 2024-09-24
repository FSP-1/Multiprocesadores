# Actividad: Bandera serie.

# A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : < Franco Sergio Pereyra GT3 >
 * Nombre y apellidos alumno 2   : < Rosa Maria Lopez GT2 >
 * Nombre y apellidos alumno 3   : < David Guil GT2 >
 * Mayoría en GTA1, GTA2 o GTA3  : < .... >
 * Nombre de grupo de actividades: < .... >


# Descripción de la arquitectura utilizada:
Ver la información del micro en Internet. Otra información se puede ver con los comandos:
  * $ free -h}
  * $ lscpu  (versión reducida de $less /proc/cpuinfo)
  * $ lstopo

A la BIOS se accede justo después de arrancar pulsando ESC, F1 o F2.

## Arquitectura: 
  * Microprocesador: Intel core I5-10500 CPU @ 3.10GHz
  * Número de núcleos: 6
  * Cantidad de subprocesos por nucleo: 12
  * Tiene hyperthreading (SMT) activado en BIOS: si
  * HDD/SDD: 500G
  * RAM: 16G
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

## Instrucciones:
 * El ejemplo muestra como generar una imagen RGB de la bandera de España.
 * Los parámetros de anchura y altura se introducen por la línea de comandos.
 * La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.

## Librerias
Se necesita tener instalados los siguientes paquetes:
 * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
 * eog para visualizar la imagen.
 * valgrind para ver las posibles perdidas de memoria.
 * perf para medir el rendimiento del programa.

## Objetivos
 * Practicar los conocimientos de C.
 * Familiarizar al alumno con 
    - El uso de varios archivos fuente. 
    - El uso de makefile y el comando make.
  * Uso de memoria dinámica. Ver getmem.h y getmem.c
  * Gestión de parámetros de entrada. Ver argshand.h y argshand.c
  * Uso de script de ejemplo para compilación y ejecución. Ver Run.sh
  * Ejemplo secuencial donde el trabajo computacional se sabe de antemano.
  * Observar que el visor de imágenes eog actualiza la salida si se cambia la imagen.
  * Chequear memory leaks.
  * Mejorar el rendimiento del programa

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Bandera -h
```

## Ejemplo de compilacion y ejecución
 * En el script Run.sh

 * El script tiene dos ejecuciones. Para ejecutar solo una, hay que copiarla en el línea de comandos del shell. Normalmente se usa la línea de comandos, no el Run.sh. El Run.sh es para recordar los comandos a ejecutar y sus parámetros.

- - - 

# Entrega: 

## Parte 1: Verificar el Software.

Ejecutar en la consola:
```console 
$ make 
$ ./Bandera -r 300 -c 400 -o España
$ ./eog España.png
```
* Se debería ver la bandera de España.

Hay que evaluar si hay alguna perdida de memoria. 
* Ejecutar en la consola:
```console 
$ valgrind ./Bandera -r 300 -c 400
```
1. **Muestra aquí la salida de pantalla:**
![captura](img/captura.png)

2. **¿Hay perdida de memoria?**
	Si
	
3. **¿Qué has tenido que corregir en Bandera.c para que no haya perdida de memoria**. 
* Hay que entregar el Bandera.c corregido. 

4. **Muestra aquí la salida de pantalla del comando valgrind sobre Bandera sin perdida de memoria.**
![captura](img/captura2.png)
- - - 
## Parte 2: Mejorar el rendimiento.

**Copia Bandera.c en Bandera2.c y en vez de usar tres bucles dobles: dos para el rojo y uno para el amarillo; hay que usar un solo bucle doble para establecer el color de cada pixel.** 

**Nota**: El makefile está ya preparado para poder compilar  Bandera2 con `$ make Bandera2`. Hay que verificar que Bandera2 funciona correctamente viendo si genera bien la imagen. 

**Nota**: El amarillo se compone de rojo y verde. Todos los pixeles tienen rojo. Solo los de la banda central tienen verde.

### Comparar el rendimiento de Bandera y de Bandera2:

**Nota**: Hay que elegir un número de filas y columnas (Rows=Cols) múltiplo de 1024 que haga que el programa tarde varios segundos y que no consuma toda la RAM.

**Nota**: cada pixel necesita tres bytes, uno para cada color.

**Nota**: 1 kB =1024 bytes.

5. **¿Que valor de Rows=Cols has elegido? ¿Cuanta memoria coge el programa con GetMem para imagen?** 

6. **Mostrar el menor tiempo real de varias ejecuciones de la salida de:**
```console 
& time ./Bandera -r Rows -c Cols
```

7. **¿Que algoritmo tiene menor tiempo de ejecución, Bandera o Bandera2?**
* Muestra los tiempos de Bandera2.c

**Ejecutar el comando:**
```console 
& perf stat -r 3 -d ./Bandera2 -r Rows -c Cols
```

8. **Mostrar los valores:** 
 * page-faults:u = 
 * L1-dcache-load-misses:u =
 * LLC-load-misses:u =

**Nota**: perf stat -r 3 -ddd permite obtener más detalles.

**Nota**: La memoria se ha cogido por filas, por lo que es más eficiente visitar las matrices por filas ya que los datos a modificar estarán más tiempo en las caches. 

9. **En Bandera2 cambia el orden del doble bucle, primero las columnas (j) y luego las filas (i). Muestra para esta versión los resultados solicitados en los puntos 7 y 9.**
 * Hay que usar Rows=Cols de 5.
 * (7) Muestra los tiempos de Bandera2 primero columnas.
 * (8) Muestra los resultados de perf stat para Bandera2 primero columnas.

10. **¿Cuando se tienen más fallos de cache, visitando la matriz primero por filas o primero por columnas? ¿Porqué?** 

11. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

- - - 
### Como ver este .md en el navegador
En linux instalar grip:

```console 
$ pip install grip 
```

y ejecutar
```console
$ grip -b README.md
```

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
