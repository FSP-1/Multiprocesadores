# Ejercicio: Bandera PTh

## A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : David Matarin Guil
 * Nombre y apellidos alumno 2   : Franco Sergio Pereyra
 * Nombre y apellidos alumno 3   : Rosa María López García
 * Mayoría en GTA1, GTA2 o GTA3  : GTA2
 * Nombre de grupo de actividades: GTA2-2 y GTA3-2

## Arquitectura: 
  * Microprocesador: Intel core I5-10500 CPU @ 3.10GHz
  * Número de núcleos: 6
  * Cantidad de subprocesos por nucleo: 12
  * Tiene hyperthreading (SMT) activado en BIOS:Si
  * HDD/SDD: 500G
  * RAM:16G
  * Se usa máquina virtual: NO
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

## Instrucciones:
  * A diferencia de Bandera OMP, ahora la version secuencial y paralela se encuentran en códigos fuente distintos:
    + Bandera.c: para obtener tiempos secuenciales.
    + Bandera-PTh: para obtener tiempos paralelos. 
  * En el ejemplo de Bandeta-PTh hay que rellenar la estructura de los parámetros de la rutina que realizan las hebras y hay que determinar que hace cada hebra en esa rutina.
  * Los parámetros de anchura, altura y número de hebras se introducen por la línea de comandos. Ver el Run.sh. En el Run.sh se han usado tamaños de 1024 y 2048, pero el 2048 hay que cambiarlo para que el programa secuencial dure varios segundos, tal como se hizo en Bandera OMP.
  * La salida gráfica (-o España) se usará solo para verificar que se está haciendo correctamente el proceso. No se usará para medir tiempos.
  * Verifica que el algoritmo paralelo funciona para distinto número de filas y columnas, es decir, las filas se reparten correctamente entre las hebras.
  * La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.
  * Se pedirá al alumno que genere de forma dinámica el número de hebras y colores ppRed, ppGren y ppBlue de cada pixel para que puedan calcularse en paralelo en las hebras. 
  * Los parámetros de la función que realizan las hebras se cogen en un vector que se generó de forma dinámica donde cada componente es una estructura con los parámetros para cada hebra. 
  * Se valorará positivamente que: 
   	- Se tenga en cuenta si hay hebras que no deben procesar datos.
  	- Que el número de pixeles por hebra no difiera mucho.
  * En el makefile ya se ha puesto la librería *-lpthread* y esta preparado para compilar la versión serie y la paralela.
  * Hay que compilar sin optimizaciones del compilador: FLAGS   = -Wall
  * Hay que comparar los tiempos obtenidos con la versión secuencial (no con la versión paralela con una hebra) y los obtenidos con la versión paralela con un número distinto de hebras.
  * Busca que hacer en los TODO. Hay que realizarlos **todos**:
```console
$ grep TODO *
```

## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
  * Familiarizar al alumno con 
	- El uso de Pthreads. 
  * Evitar false sharing, teniendo en cuenta como se reservó la memoria. 
  * Conocer el funcionamiento de las rutinas PThreads y la librería a incluir. Hay que usar:
```console 
$ man 3 <rutina>  
```
```console 
$ man 7 pthreads 
```
muestra todas las rutinas en la librería pthread.

## Compilación

```console 
$ make Bandera
$ make Bandera-PTh
$ make all 
```

## Ayuda parámetros 
```console
$ ,/Bandera -h
$ ./Bandera-PTh -h
```

## Ejemplo de compilacion, establecer parámetros  y ejecución
 * En el script Run.sh
 * El valor 2048 habrá que cambiarlo.

- - - 
# Entrega

## Speed-up teórico

* Al igual que en Bandera-OMP, hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n*1204, n>1) que haga que el programa secuencial (Bandera.c) tarde varios segundos y que no consuma toda la RAM. Se puede usar el mismo valor que se usó en Bandera-OMP, si no se ha cambiado la arquitectura. 

1. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?** 
 * El valor escogido fue de 10240. La imagen creada con ese valor es de 22525 bytes.

2. **Usa la versión secuencial *Bandera.c* para rellenar la siguiente tabla.**
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time Bandera -r <Rows> -c <Cols>
```

| Ejecución   | -r 1024 -c 1024 | -r Rows -c Cols |
| ----------- | --------------- | --------------- |
|T.Sec        |  0.009          |  0.303          |
|T.CsPar      |  0.002279       |  0.205566       |
|SpA(2)       |  1.145          |  1.513          |
|SpA(4)       |  1.234          |  2.036          |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Bandera ... 
 * T.CsPar: El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue. 
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.

## Resultados del algoritmo paralelo
* Para asignar pixeles a las hebras, estás deben:
    + Hacer un trabajo similar
    + Ningún pixel debe quedarse sin hacer. Probar con distintos valores de Rows y Cols.
    + Que un pixel no se haga más de una vez.

3. **Usa la versión paralela *Bandera-PTh.c* para rellenar la siguiente tabla.**
* Puedes usar la rutina DivideVector.c que se encuentra en el fichero Rutines-PTh.tgz en la parte de Teoría del BB Learn.
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time Bandera-PTh -r <Rows> -c <Cols> -nt <p>
```

| Ejecución   |-r 1024 -r 1024 |-r Rows -c Cols  | 
| ----------- | -------------- | --------------- |
|T(1)         | 0.074          |  0.305          |
|T(2)         | 0.075          |  0.201          |
|T(4)         | 0.075          |  0.151          |
|Sp(2)        | 0.12           |  1.507          |
|Sp(4)        | 0.12           |  2.007          |

Donde 
* T(p): El tiempo total (parte real de la salida $time Bandera-PTh ...) del algoritmo paralelo con p hebras.
* Sp(p): Speed-up real con p hebras paralelas.


4. **¿Que distribución estática de datos has usado? (Mira la introducción a las arquitecturas paralelas)?** 
    - (bloque,*), 
    - (*,bloque), 
    - (bloque,bloque), 
    - (ciclica,*), 
    - (*,ciclica), 
    - (ciclica,ciclica).
    
* Se ha usado la distribución (*, bloque) al hacer que los hilos procesen x cantidad de filas.

5. **¿Son el tiempo secuencial y el paralelo con una hebra diferentes? ¿Porqué?**
* Si, eso es debido a que en el paralelo con una hebra hay un retardo al crear y manejar el hilo.
6. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**
Si
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

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.
