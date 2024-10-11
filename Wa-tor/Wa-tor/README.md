# Actividad Wa-tor serie:

## A rellenar por el alumno
 * Nombre y apellidos alumno 1   : 
 * Nombre y apellidos alumno 2   : 
 * Nombre y apellidos alumno 3   : 
 * Mayoría en GTA1, GTA2 o GTA3  : 
 * Nombre de grupo de actividades: 

## Descripción de la arquitectura utilizada: 
  * Microprocesador:
  * Número de núcleos:
  * Cantidad de subprocesos por núcleo:
  * Tiene hyperthreading (SMT) activado en BIOS:
  * RAM:
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

## Instrucciones:
* El alumno debe realizar la rutina de iteración de un pez.
* Se puede basar en la función de iteración de un tiburón, que ya está realizada.
* Ambas funciones están en la librería ocean.h, ocean.c.
Buscar "**//TODO**" en los ficheros:
```console 
$ grep -n TODO *
```
* Además para la versión de producción hay que entregar dos ficheros:
    - **Profiling.txt** con la salida de **gprof**. 
    - **MemProf.txt** con la salida del uso de **valgrind**.
* Para el MemProf.txt la compilación en modo producción debe ser sin -g ni -pg.
* Para obtener datos razonables del % de tiempo de cada rutina en el profiling, el algoritmo debe tardar.  Se puede necesitar incrementar el número de iteraciones 
```console 
$ ./Wa-tor -ni 10000
```
* Las reglas de como funciona Wa-tor se encuentran más abajo.

## Librerias
 * El paquete **netpbm-progs** (o **netpbm**) para el comando del sistema rawtoppm.
 * El paquete **eog** para visualizar los ficheros .ppm.
 * El paquete **gnuplot** para generar las gráficas de evolución de peces y tiburones.
 * El paquete **gdb** para posibles debuggings.
 * El paquete **binutils** para el comando **gprof**.
 * El paquete **valgrind** para chequear posibles memoria asignada y no liberada.
 * El paquete **ffmpeg** o **ffmpeg-free** para generar video raw que se visualiza con ffplay

## Objetivos
 * Ejemplo secuencial donde el trabajo computacional no se sabe de antemano.
 * Además, existen conflictos:
    - Una celda (pixel) no puede contener dos animales.
    -  Dos tiburones no pueden comerse el mismo pez.
  * Es un código secuencial candidato a necesitar balanceo dinámico de la carga en su paralelización y resolución de conflictos o dependencia de datos.

## Compilación
```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Wa-tor -h
```

## Ejemplo de compilación y ejecuciones
* En el script Run.sh
* A Wa-tor, si se le indica salida gráfica **-o**, realiza una llamada a system para ejecutar **eog** (image viewer) de los movimientos de los animales en el océano. En preferencias de eog, quitar el "antialiasing" al escalar la imagen.
* A Wa-tor, si se le indica **-d**, usa la aplicación **gnuplot** que hay que instalar para visualizar la evolución de los peces y los tiburones que se graba en el fichero "datos.txt" por defecto.
* Wa-tor, si se le indica **-ffmpeg**, usa la aplicación **ffplay** que hay que instalar para visualizar la evolución de los peces en formato video raw.
* Antes de ejecutar Wa-tor hay que ver que no hay otra instancia corriendo:
```console
$ps aux  | grep Wa-tor
$ps aux  | grep eog
$ps aux  | grep gnouplot
$ps aux  | grep ffplay

```
de **Wa-tor** o en su caso **eog**, **gnuplot**, y/o **ffplay**. En tal caso hay que matar el proceso necesario con:
```command 
$ ps aux   (para ver los procesos y sus pid).
$ kill -9 <pid>
```

- - -

# Entrega :

1. **Indica los comandos que has realizado para hacer el profiling**.

Para realizar el archivo hay que usar el flag "-pg" en los comandos de gcc para cuando se ejecute el archivo Wa-tor con "./Wa-tor" se crea un archivo gmon.out. Por último, con el comando "gprof Wa-tor gmon.out > Profiling.txt" para hacer el profiling del programa y poner los resultados en un archivo de texto.

2. **En el fichero Profiling.txt con la salida del gprof del programa, ¿Que rutinas consumen más tiempo?**

3. **Indica los comandos que has realizado para hacer el chequeo de perdida de memoria.**

4. **En el fichero Memprof.txt que has generado con la salida de valgrind, ¿Existen perdidas de memoria?**

5. **¿Existen ejecuciones en las que no se llegan al número establecido de iteraciones? ¿Porqué?**

6.  **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

- - -

# Actividad Wa-tor: reglas.
* Tomadas de [wiki](https://en.wikipedia.org/wiki/Wa-Tor)

## Movimento
* En cada iteración un pez o tiburón puede moverse norte sur este u oeste a un pixel adyacente, si no está ocupado por un miembro de su misma especie.

### Peces
* Si los 4 pixeles adyacentes están ocupados (por peces o tiburones), no se mueve.
* Se selecciona el pixel a moverse aleatoriamente entre los libres.

### Tiburón
* Se elige entre los adyacentes ocupados por peces, uno aleatoriamente y se come el pez.
* Si no hay peces adyacentes, se mueve como el pez, evitando otros tiburones.

## Vida y reproducción:

### Pez
* Si el pez ha sobrevivido un número de iteraciones se reproduce. Esto ocurre cuando además hay un pixel adyacente libre en el cual se crea un nuevo pez. Ambos, padre e hijo ponen sus contadores de iteraciones a 0.

### Tiburon
* En cada iteración se le decrementa en 1 la energía.
* Al alcanzar la energía 0 el tiburón muere.
* Al comerse un pescado, el tiburón gana una cantidad de energía.
* Si el tiburón alcanza un número de iteraciones, se reproduce como el pez. El hijo coje la energia de creación de tiburón. El padre mantiene la suya.

- - - 
### Como ver este .md 
En linux instalar grip:
```console 
$ pip install grip 
```
y ejecutar
```console
$ grip -b README.md
```
- - -
### Markdown cheat sheet
Para añadir información a este README.md:
[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -
&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
