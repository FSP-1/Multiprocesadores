# Ejercicio: Mandelbrot MPI.

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
 * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

## Instrucciones:

* En el Run.sh se tienen los comandos a ejecutar para -mi 1e4 y -mi 1e5 para el algoritmo secuencial (Mandel) y el paralelo (Mandel-MPI) con distinto número de tareas.
* La opción **-o** genera la imagen. La imagen se puede usar para verificar que el código es correcto, al visualizarse con el comando *eog*.

## Librerías
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
 * Familiarizar al alumno con 
	- El uso de la librería MPI.
 	- Uso de una estrategia maestro-trabajador donde la tarea maestra (0) reparte trabajo entre las tareas trabajadoras (>0). Este esquema se usa para realizar balanceo dinámico de la carga, cuando la carga computacional por unidad a calcular no es homogénea. Usar Scatter y Gather no sería lo más eficiente para este tipo de cómputo.
 	- En la Opción 2: cómo chequear sin bloqueo si hay o no mensajes para la tarea maestra, de forma que si no los hay la tarea maestra también haga trabajo. 

## Compilación

```console 
$ make -j
$ make -j Mandel
$ make -j Mandel-MPI
$ make -j all
```

## Ayuda parámetros 
```console
$ ./Mandel -h
$ ./Mandel-MPI -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:


## Speed-up teórico

1. **Rellena la siguiente tabla para la versión secuencial, Mandel.c.**


| Ejecución   | -mi 1e4         | -mi 1e5           |
| ----------- | --------------- | ----------------- |
|T.Sec        |  5.15           |  49.556           |
|T.CsPar      |  5.041          |  49.447           |
|SpA(2)       |  1.958          |  1.99             |
|SpA(4)       |  3.761          |  3.97             |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Mandel ... 
 * T.CsPar: El tiempo de la parte del código secuencial que será paralelizado: doble bucle.
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.



**Se tiene que entregar una de las siguientes opciones**:

## Opción 1: La tarea maestra (0) no realiza cómputo.
 * Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.
 * Se requiere completar el Mandel-MPI. Ver **//TODO** en el código. Solo se requieren modificaciones en las comunicaciones de las tareas trabajadoras (>0) que deben casar con las que realiza la tarea maestra (0).
 ```console
 $ grep -n TODO Mandel-MPI.c
 ```
* Las tareas trabajadoras en Mandel-MPI trabajan por filas completas y en una fila cada vez. Cada fila tiene tres vectores de `char` para RGB.
* Las tareas trabajadoras terminan cuando reciben un número de fila a procesar igual al número de filas. Las filas  van de 0 a Rows-1.


2. **Rellene la siguiente tabla para Mandel-MPI.c:**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |   5.15          |   49.556        | 
|T(1)         |   N/A           |   N/A           | 
|T(2)         |   5.471         |   50.511        | 
|T(4)         |   2.081         |   17.332        | 
|Sp(1)        |   N/A           |   N/A           | 
|Sp(2)        |   0.94          |   0.981         | 
|Sp(4)        |   3.081         |   2.85          |

3. **Indica al número de filas que realiza cada hebra para una ejecución con p=4 de la tabla anterior.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**
    
     * Sí, los números de filas pueden diferir ligeramente entre las hebras en una misma ejecución debido a:

    	* Desbalance en la distribución del trabajo:
     
       		* El método de distribución de filas podría ser dinámico, dependiendo de la velocidad de procesamiento de cada hebra y las características del sistema.
           
 	*   Sincronización de las hebras:

  		*  Si se utiliza una estrategia de "work stealing" o distribución dinámica, las hebras más rápidas toman más trabajo cuando otras tardan más en completarlo.
 
    
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**

      * En general, el número de filas realizado por las hebras es similar pero no exactamente igual. Esto ocurre porque:
        
  	*  El tamaño total de filas es divisible por el número de hebras (1024 ÷ 4 = 256), pero los bordes pueden no ser equitativos si el proceso es dinámico.
  	*  Las hebras suelen trabajar en bloques distribuidos uniformemente, pero pequeñas diferencias surgen debido a:
   		*   Carga dinámica del sistema.
     		*   Procesamiento de filas que requieren más iteraciones (pixeles complejos): Algunas filas pueden necesitar más tiempo para calcular valores del conjunto de Mandelbrot.


4. **¿Porqué el tiempo paralelo con dos tareas (-np 2) es siempre peor que el secuencial?**
   * El tiempo paralelo con dos tareas puede ser peor que el secuencial debido a:
   
   	* Sobrecarga de Comunicación En la versión paralela, las tareas deben coordinarse a través del sistema MPI, lo que introduce tiempo extra para enviar, recibir y sincronizar datos.
   
   	* Desbalance de Carga: Si las filas distribuidas entre las dos tareas no están perfectamente equilibradas, una tarea podría terminar antes y quedarse esperando que la otra complete su parte.
   
   	* Limitación del Escalado: Con dos tareas, la capacidad de dividir eficientemente el trabajo se reduce.
 
 
6. **¿Tiene ventajas su paralelización?**
	* La paralelización tiene ventajas claras, especialmente en problemas de gran escala, pero para aprovecharla al máximo es necesario:

   		* Minimizar la sobrecarga de comunicación.
     		* Balancear eficientemente la carga de trabajo.
   		* Escoger problemas que requieran suficiente computación para superar el costo de la comunicación.

	* En casos pequeños o para un bajo número de iteraciones, la paralelización puede no ser tan efectiva o incluso contraproducente. Sin embargo, para configuraciones complejas o en sistemas con muchos núcleos, es altamente más ventajosa.
	
7. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

 	* si 

## Opción 2: La tarea maestra también realiza cómputo.
* Se necesita cambiar más el código, pero se opta a la máxima nota.
* La tarea maestra (0) debe chequear de forma no bloqueante si le ha llegado un mensaje de una tarea trabajadora. 
    + Si le ha llegado un mensaje, debe recoger los resultados y enviarle la nueva fila a procesar, tal como está actualmente el código. 
    + **Si no le ha llegado un mensaje** (MPI_IProbe) la hebra maestra realiza el cómputo de la fila actual en esta iteración del bucle.

7. **Rellene la siguiente tabla para Mandel-MPI.c:**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |                 |                 | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 |

8. **Indica al número de filas que realiza cada hebra para una ejecución con p=2,4 de la tabla anterior.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**


9. **¿Tiene ventajas su paralelización?**

10. **¿Cuando sería mejor que la hebra maestra haga trabajo?**
    - Cuando el número de tareas trabajadoras es bajo, o
    - Cuando el número de tareas trabajadoras el alto.

11. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

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
