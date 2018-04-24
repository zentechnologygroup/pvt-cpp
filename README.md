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
* Command line interaction for generating high-resolution, high-precision petroleum fluid datasets by applying model calibration to Constant
* Composition Expansion (CCE) and Differential Liberation (DL) fluid experimental data at one or various temperatures. Several tuning modes are available depending on integrity of experimental data.



## Dependencies 

| Package |  Download |
| ------------- | ------------- |
| clang | official repositories |
| clang-format-3.X | official repositorie |
| gcc |  official repositories |
| libx11-dev  | official repositories |
| libasprintf-dev |  official repositories |
| libgettextpo-dev  | official repositories |
| libgsl0-dev  | official repositories |
| libmpfr-dev  | official repositories |
| make | official repositories |
| ruby | https://www.ruby-lang.org/es/downloads/ |
| tclap | official repositories |
| xclip | official repositories |
| xutils-dev  | official repositories |
| calculus | gem install |
| bibtex-ruby | gem install |

### Aleph-w

Download the latest version of Aleph-w, available at: https://github.com/lrleon/Aleph-w

Download it in the directory of your preference. It is not necessary to compile this library. 

The makefiles will search for an environment variable called ALEPHW, which could be defined as follows:
```
	ALEPHW = directory-where-alephw-is
	export $ALEPHW
```
  
These lines can be placed in the `.bashrc` file, so that each time a console is opened the variable is already defined.


### Imake

For a distribution based on Debian it can be installed by executing:
```
	sudo apt-get install xutils-dev
```
	
For other distributions, follow the instructions available at: http://www.snake.net/software/imake-stuff/imake-faq.html#where-to-get


## Generation of `makefiles`

The first time you download the sources, execute:

```
	$xmkmf
	$make Makefiles
	$make depend
```

The command `make depend` generates compilation "dependencies", which allow you to detect when the sources have to be recompiled.

Each time you add a new header file to the `include` directory, execute `make depend`.

## Generation of executables

To generate all the executables, run:


```
  $make all
```


## Authors

####  Software Architect and Lead Developer
* PhD. Leandro Rabindranath Leon - (leandro.r.leon@gmail.com)

#### Domain Driven Analyst and  Designer
* MSc. Alberto Valderrama - (alberto.valderrama@zentech.group)

#### Developer and Tester
* Ing. Ixhel Mejías -  (ixhel.mejias@zentech.group)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
