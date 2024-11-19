# Ejercicio: Setup a Cluster MPI. 

## A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : 
 * Nombre y apellidos alumno 2   : 
 * Nombre y apellidos alumno 3   : 
 * Mayoría en GTA1, GTA2 o GTA3  : 
 * Nombre de grupo de actividades: 

## Arquitectura: 
 * PCs:
    + Nombre PC1:
    + IP PC1    : 
    + Nombre PC2:
    + IP PC2    : 
    + Nombre del **mismo usuario** en PC1 y PC2:  
* Conexión (Ehternet 100Mb/1Gb, wifi, VM,..): 


# Instrucciones:

Se seguirá básicamente el tutorial:
  * [Setting up an Ubuntu Linux Cluster](https://www.particleincell.com/2020/ubuntu-linux-cluster/)
  * Si no se dispone de dos PCs, se usarán dos del lab 2.02 durante el Grupo de Trabajo correspondiente.
  * Al usar el mismo usuario en PC1 y PC2 no hay que indicarlo en las conexiones ssh. Se puede usar:
    + `[user@PC1]$ ssh PC2` en vez de
    + `[user@PC1]$ ssh user@PC2`
  * No es necesario poder conectarse sin login ni password desde PC2 a PC1.
  * En los comandos ejemplo hay que cambiar PC1 y PC2 por sus IPs o por sus nombres.
    + Se recomienda usar las las IPs y así hay que hacer menos cosas.  
    + Para que funcionen los nombres deben estar dados de alta en un servidor DNS o en el fichero */etc/hosts*. No confundir */etc/hosts* con el fichero *hosts* que se usará en esta actividad y que se almacena en el directorio local del usuario. Ver [Cambiar el hostname permanentemente](https://www.cyberciti.biz/faq/ubuntu-18-04-lts-change-hostname-permanently/).
  * Pasos:
    - Se recomienda usar dos PCs del lab. 2.02
    - Se puede intentar conectar dos VM mediante NAT ver [Vídeo](https://youtu.be/mOuKtZVMwbw).
    - Se precisa que los hosts se llamen de forma distinta. En el Lab 2.02 ya se llaman de forma distinta.
       * Ver [Cambiar el hostname permanentemente](https://www.cyberciti.biz/faq/ubuntu-18-04-lts-change-hostname-permanently/.
    - Crear claves ssh en PC1 
        + `[user@PC1]$ ssh-keygen -t rsa -b 4096`
    - Copiar la parte publica de la clave de PC1 en el ~/.ssh/Authorized_keys de PC2. 
        + Se puede usar `[user@PC1]$ scp ~/.ssh/id_rsa.pub PC2:.ssh/authorized_keys` 
        + Tener dos shells en PC1, pero el segundo ya conectado con ssh en PC2. Cortar en el shell de PC1 y pegar en un editor como nano/joe/vi en el shell de PC2.
    - Comprobar que se puede hacer ssh entre PC1 y PC2 sin que pida password.
        + `[user@PC1]$ ssh PC2`
        + `[user@PC2]$`
    - En un shell hay que ver primero en qué PC nos encontramos antes de ejecutar comandos.
    - Salida del Shell: CTRL+D
    - Generar un fichero ~/hosts donde aparezcan PC1 y PC2 (Ver el tutorial).
        + ~ indica el directorio HOME del usuario (/home/user).
    - Verificar que se pueden ejecutar comandos en los dos ordenadores a la vez, por ejemplo el comando *hostname* con
      ```console
      [user@PC1]$ mpirun -np 4 -hostfile ~/hosts hostname
      ```
    - El comando `hostname`se ejecutará en ambos PCs por lo que deben aparecer los nombres de los dos PCs.
    - Compilar el programa mpi_hello.c de [Llnc](https://hpc-tutorials.llnl.gov/mpi/exercise_1/) en los dos PCs y mismo directorio.
    - Ejecutar el comando mpi_hello con varios procesos en cada PC según ~/hosts.
        + Deben ejecutarse algunos mpi_hello en PC1 y otros en PC2, tal como se hizo con el comando `hostname`.
        + `[user@PC1 mpirun -np 5 -hostfile ~/hosts -oversubscribe mpi_hello`
        + Deben aparecer 5 hello word, algunos desde PC1 y otros desde PC2. 


### A tener en cuenta:
 * No mostrar información no relevante en los pasos realizados.
 * Se deben mostrar que se están ejecutando los programas en las dos máquinas diferentes. Copiar  `[user@PC1]$`y `[user@PC2]`de los comandos usados.
 * **Todo se realiza en el HOME del usuario.**
 * Solo se necesita sudo si hay que cambiar los nombres de las máquinas para que sean diferentes. No hace falta en el Lab. 2.02.
 * Es conveniente tener el mismo usuario en las dos máquinas.
 * Se aconseja ejecutar el programa mpi con el camino completo donde se encuentra y que sea el mismo en las dos máquinas. Mejor ejecutarloen el $HOME. Por ejemplo:
 
 ```console 
 [atc@PC1 ~]$ mpirun -np 5 -hostfile ~/hosts ~/mpi_hello
 ```

# Objetivos
 * Familiarizar al alumno con 
	- El establecimiento de un cluster de PCs para uso con MPI.
 * Uso de acceso directo mediante compartición de claves públicas ssh.
 * Instalación de MPI.
 * Poder ejecutar varios procesos MPI en varias máquinas, mediante un comando en una de las máquinas. 

- - -
# Entrega:
 Hay que coger los comandos realizados y las salidas importantes, que indiquen que se ha realizado correctamente la práctica. Debido a que se usan dos PCs, hay que mostrar la parte del usuario y nombre del PCs donde se realiza el comando. En los ejemplos anteriores se ha usado  `[user@PC1 ~]$` y`[user@PC2 ~]$`.

## 1. Generación de claves públicas ssh en PC1 e intercambio de claves con PC2.

 ```console 
 < comando > 
 < salida > 
 ```
 
  ```console 
 < comando > 
 < salida > 
 ```
....

## 2. Verificación de conexión ssh dsde PC1 hacia PC2 sin pedir password:
 ```console 
 < comando > 
 < salida > 
 ```
 
  ```console 
 < comando > 
 < salida > 
 ```
....

## 3. Ejecución en cada máquina (independientemente en PC1 y PC2) y en ambas a la vez (comando solo desde PC1) del comando hostname con mpirun. IMPORTANTE: mostrar el fichero ~/hosts.
 ```console 
 < comando > 
 < salida > 
 ```
 
  ```console 
 < comando > 
 < salida > 
 ```
....


## 4. Ejecute el comando `mpirun` en PC1 para ejecutar `mpi_hello` en ambas máquinas (PC1 y PC2) al mismo tiempo (usando la opción `-hostfile ~/hosts`):
 ```console 
 < comando > 
 < salida > 
 ```

- - -

## Librerias
   * Puede necesitarse instalar además el paquete openssh-askpass

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
