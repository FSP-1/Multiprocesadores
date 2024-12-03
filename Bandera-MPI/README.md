# Ejercicio: Bandera OMP.

# A rellenar por el alumno/grupo
## Nombre y apellidos alumno 1   : < Franco Sergio Pereyra >
## Nombre y apellidos alumno 2   : < Rosa Maria Lopez Garcia>
## Nombre y apellidos alumno 3   : < David Matarin Guill>
## Mayoría en GTA1, GTA2 o GTA3  : < GT2 >
## Nombre de grupo de actividades: < GT2-2 y GT3-2 >


# Descripción de la arquitectura utilizada:
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

# Instrucciones:
  * Ya se asignan pixeles a las tareas, de forma que:
    + Hacen un trabajo similar
    + Ningún pixel debe quedarse sin hacer.
    + Que un pixel no se haga más de una vez.
  * El makefile y el  Run.sh ya se encuentran preparados para las ejecuciones secuencial y paralelas. 
    + El valor **2048** hay que cambiarlo para que el secuencial dure al menos varios segundos sin consumir la RAM. 
  * La salida gráfica se usará solo para verificar que se está haciendo correctamente el proceso. No se usará para medir tiempos.
  * La memoria consumida dependerá del tamaño de la imagen y el tipo de datos a almacenar. Hay que calcularlo. La memoria consumida no es la misma en la tarea 0 que en las demás tareas.
  * Debido a que MPI envía secuencias de datos contiguas y la imagen es 2D, se han cogido vectores de tamaño ROWs*COLs para los colores RGB.
  * Se hace una distribución estática, donde cada tarea (incluyendo la 0) calcula la posición y el  número de pixeles que debe procesar.
  * Cada tarea calcula los valores R y G  de los pixeles que le han tocado y los manda a la tarea 0 que los recoge y opcionalmente los escribe en un fichero para que se pueda visualizar la bandera. 
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
	- El uso de la librería MPI. 
  * Usar MPI_Probe cuando no se sabe de quien se recibe el mensaje.
  * Usar MPI_Get_count cuando no se sabe el tamaño que hay que poner en el receive.
  * Entender la memoria consumida por las tareas.

### A tener en cuenta:
  * Para saber el funcionamiento de las funciones de la librería, una vez cargada con  (en Ubuntu no hay que cargarla):

```console 
$ module load < libreria mpi >  
```
hágase

```console 
$ man <rutina>  
```
Por ejemplo, 
```console
$ man MPI_Probe
$ man MPI_Scatterv
```

## Compilación

```console 
$ make Bandera
$ make Bandera-MPI
$ make all 
```

## Ayuda parámetros 
```console
$ ,/Bandera -h
$ ./Bandera-MPI -h
```

## Ejemplo de compilacion, establecer parámetros  y ejecución
 * En el script Run.sh
 * El valor 2048 habrá que cambiarlo.

# Entrega

**Se tiene que entregar una de las siguientes opciones**:

### Opcion 1: Realizar los //TODO:
  * En el ejemplo hay que realizar lo que se encuentra en **//TODO**
  * Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.

### Opcion 2: Usar MPI_Scatterv y MPI_Gatherv
* Se usan las versiones "v" porque a cada tarea le pueden corresponder un número de pixeles distinto ya que el número de pixeles puede no ser divisible por el número de tareas. Puede forzarse a que el uso de las versiones v (Scatterv) o (Gatherv) no sea necesario, en tal caso razonar la solución propuesta y se tiene menos nota.

 
## Speed-up teórico

* Al igual que en ejercicios anteriores, hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n*1204, n>1) que haga que el programa secuencial (Bandera.c) tarde varios segundos y que no consuma toda la RAM. Se puede usar el mismo valor que se usó en los ejercicios anteriores, si no se ha cambiado la arquitectura.
* Para la **Opción 2** hay que usar un valor que no sea divisible entre el número de hebras. Habrá que sumarle algo a Rows=Cols del punto anterior para que no sea múltiplo de 2.

1. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?** 

2. **Usa la versión secuencial *Bandera.c* para rellenar la siguiente tabla.**
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time ./Bandera -r <Rows> -c <Cols>
```

| Ejecución   | -r 1024 -c 1024 | -r Rows -c Cols |
| ----------- | --------------- | --------------- |
|T.Sec        |  0.004          |  0.305          |
|T.CsPar      |  0.002          |  0.205          |
|SpA(2)       |  1.429          |  1.506          |
|SpA(4)       |  1.812          |  2.016          |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Bandera ... 
 * T.CsPar: El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue. 
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.

## Resultados del algoritmo paralelo

3. **¿Qué opción se ha seleccionado?**
 *option 1

4. **Usa la versión paralela *Bandera-MPI.c* para rellenar la siguiente tabla.**
 * Ejemplo de ejecución sin salida gráfica:
```console 
$ time mpirun -np <p> ./Bandera-MPI -r <Rows> -c <Cols>
```
ime mpirun -np 2 -host 192.168.6.51:1 -host 192.168.6.52:1 ./Bandera-MPI -r 7680 -c 10240 -o prueba

| Ejecución       | -r 1024 -c 1024| -r Rows -c Cols |
| --------------- | -------------- | --------------- | 
|T.Sec            |        0.004   |        0.305    |
|T(1)             |      0.00392   |     0.368658    |
| Mem. Task=0(Gb) |      1048576   |     10485760    |
| --------------- | -------------- | --------------- |
|T(2)             |        0.002   |     0.206996    |
| Mem. Task>0(Gb) |      1048576   |     10485760    |
| --------------- | -------------- | --------------- |
|T(4)             |     0.001975   |     0.151784    |
| Mem. Task>0(Gb) |      1048576   |     10485760    |
| --------------- | -------------- | --------------- |
|Sp(1)            |        1.024   |  0.827          |
|Sp(2)            |  	   1.867   |  1.473          |
|Sp(4)            |        2.025   |  2.009          |


Donde 
* T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$ time ./Bandera ...` 
* T(p): El tiempo total (parte real de la salida `$ time ./Bandera-MPI ...`) del algoritmo paralelo con p tareas.
* Mem. Task=0(Gb): Memoria consumida por la tarea cero.
* Mem. Task>0(Gb): Memoria consumida por las tareas distintas de la cero. 
* Sp(p): Speed-up real con p tareas paralelas.


5. **¿Es Sp(p)> SpA(p)? ¿Cuales pueden ser los motivos?**
* Sí, Sp(p), que es la aceleración paralela con carga total, suele ser mayor que SpA(p), que es la aceleración paralela considerando solo las tareas activas. Esto ocurre porque Sp(p) incluye el impacto de las tareas que no están procesando datos activamente (procesos inactivos que manejan 0 píxeles), mientras que SpA(p) considera únicamente los procesos que realizan algún trabajo.
6. **Explica como has calculado las cantidades de memoria consumidas por la tarea 0 y las otras tareas.**
* La memoria consumida se calcula considerando las asignaciones dinámicas y estáticas en el código. Para la tarea 0, se manejan los arreglos completos para las tres matrices RGB (ppRed, ppGreen, ppBlue), cada una de tamaño Rows×Cols.
Memorias:
Memoria tarea 0=3×Rows×Cols×sizeof(char)

Para las demás tareas, cada una procesa NPixels píxeles, calculado en DivideMBetweenNForMe. Además, solo se asignan matrices locales (ppRed y ppGreen) del tamaño NPixels.
Memoria total por tarea:
Memoria tarea i=2×NPixels×sizeof(char)

Nota:

    Para tareas inactivas (NPixels=0NPixels=0), el consumo de memoria dinámica es cero.
    El uso de funciones como GetMem asegura que las matrices estén inicializadas a 0.
7. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

 Sí.   
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
