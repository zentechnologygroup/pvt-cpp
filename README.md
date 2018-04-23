## PVT
PVT is a C++ library for reservoir/production fluids characterization through the Black Oil Method.

## Features
* Features
* Runtime entirely written in C++14.
* Tested on gcc, clang and Intel (icc), compilable on any platform that supports any of these.
* The library is compilable for Windows, OSX, Linux, and Solaris operating systems.
* Fully reentrant code, which makes it multithreaded.
* Chainable to any system.
* Twenty-six PVT properties of crude oil, natural gas and formation brines.
* Exceptions system powered by developmental ranges of correlations which aids in a proper use of models.
* Comprehensive set of correlations, methods and standard equations, 186 in current version.
* Exceptions system powered by developmental ranges of correlations which aids in a proper use of models.
* Command line interaction for generating high-resolution, high-precision petroleum fluid datasets of twenty-six PVT properties of crude oil, natural gas and formation brines through high-performance processing.
* Command line for generating high-resolution, high-precision petroleum fluid datasets by applying model calibration to Constant * Composition Expansion (CCE) and Differential Liberation (DL) fluid experimental data at one or various temperatures. Several tuning modes are available depending on integrity of experimental data.



## Dependencias

### GSL - GNU Scientific Library

Sobre una distribución basada en debian puede isstalarse con

	sudo apt-get install libgsl-dev
	
En otra distro siga las instrucciones de la
[página web de gsl](https://www.gnu.org/software/gsl/)

### The GNU MPFR Library

Sobre una distribución basada en debian puede isstalarse con

	sudo apt-get install libmpfr-dev libmpfrc++-dev
	
En otra distro siga las instrucciones de la
[página web de MPFR](http://www.mpfr.org/)

### Imake

Sobre una distribución basada en debian puede isstalarse con

	sudo apt-get install xutils-dev
	
En otra distro tendrá que seguir
[estas instrucciones](http://www.snake.net/software/imake-stuff/imake-faq.html#where-to-get)

### Aleph-w

Descargue la última versión de Aleph-w en [sourceorge](https://sourceforge.net/projects/aleph-w/)

Explótela en el directorio de su preferencia.

No es necesario compilar esta biblioteca. 

Los makefile buscarán una variable de ambiente llamada `ALEPHW`, la
cual podría definirse del siguiente modo:
	
	ALEPHW = directorio-donde-esta-alephw
	export $ALEPHW
	
Estas líneas pueden ponerse en el archivo `.bashrc`, de modo que cada
vez que se abra una consola la variable ya esté definida. Para evitar
reiniciar una nueva consola, haga

	source ~/.bashrc
	
luego de haber editado el archivo `.bashrc`

## Directorios

Al presente, los directorios de trabajo son:

1. `include`: declaraciones de la biblioteca
2. 'units`: módulo de manejo de unidades
3. `tests`: pruebas

Cada uno de estos directorios contiene su `README` explicativo de su contenido.

## Generación de `makefiles`

La 1ra vez que descargue los fuentes ejecute

	xmkmf
	make Makefiles
	make depend
	
El comando `make depend` genera "dependencias" de compilación, las
cuales permiten detectar cuando hay que recompilar fuentes.

Cada vez que añada un nuevo archivo header al directorio `include`
ejecute `make depend`.

## Generación de ejecutables (por ahora los tests)

Ejecute

	make all
	
Para generar todos los ejecutables. 

Alternativamente se puede ejecuar

	make all -n num
	
donde `num` sería el número de procesadores del computador. Esto
permite compilar más rápido

## Authors

####  Software Architect and Lead Developer
* PhD. Leandro Rabindranath Leon - (leandro.r.leon@gmail.com)

#### Domain Driven Analyst and  Designer
* MSc. Alberto Valderrama - (alberto.valderrama@zentech.group)

#### Developer and Tester
* Ing. Ixhel Mejías -  (ixhel.mejias@zentech.group)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
