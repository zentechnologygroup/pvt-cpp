## PVT
PVT is a C++ library for reservoir/production fluids characterization through the Black Oil Method.

## Features
* Features
* Runtime entirely written in C++14.
* Tested on gcc, clang and Intel (icc), compilable on any platform that supports any of these.
* The library is compilable for Windows, OSX, Linux, and Solaris operating systems.
* Fully reentrant code, which makes it multithreaded.
* Chainable to any system.
* Characterization of the following reservoir fluids: black oils, dry gases and wet gases. 
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
| clang-format-3.X | official repositories |
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

### Imake

For a distribution based on Debian it can be installed by executing:
```
	sudo apt-get install xutils-dev
```
	
For other distributions, follow the instructions available at: http://www.snake.net/software/imake-stuff/imake-faq.html#where-to-get


### Aleph-w

Download the latest version of Aleph-w, available at: https://github.com/lrleon/Aleph-w

Download it in the directory of your preference. It is not necessary to compile this library. 

The makefiles will search for an environment variable called ALEPHW, which could be defined as follows:
```
	ALEPHW = directory-where-alephw-is
	export $ALEPHW
```
  
These lines can be placed in the `.bashrc` file, so that each time a console is opened the variable is already defined.

### libzen

Download the repository LibZen, available at: https://github.com/zentechnologygroup/libzen

Download it in the directory of your preference.

The makefiles will search for an environment variable called ZEN, which could be defined as follows:

```
	ZEN = directory-where-libzen-is
	export $ZEN
```
  
These lines can be placed in the `.bashrc` file, so that each time a console is opened the variable is already defined.


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


## Tests

After compiling, in the `backend` directory you can find the binary file `backend-cplot`, which allows the generation of a basic PVT properties grid. For this, the specification of the fluid type and field measurables is required, as well as the establishment of a set of correlations and the temperature-pressure analysis framework. 


For more details consult the help system by executing the command:
```
$./backend/backend-cplot --help
```

Example,
```
$./backend/backend-cplot --rsb 80 --unit "rsb SCF_STB" --api 8.3 --unit "api Api" --yg 0.608 --unit "yg Sgg" --nacl 0 --unit "nacl Dissolved_Salt_PPM" --h2s 0 --unit "h2s VolumeFraction" --n2  0.19 --unit "n2 MolePercent" --co2 0.86 --unit "co2 MolePercent" --tsep 100 --unit "tsep  Fahrenheit" --psep 100 --unit "psep psia" --t  "80 300 10" --unit "t Fahrenheit" --p  "981 7461 40" --unit "p psia" --pb  PbAlMarhoun  --rs  RsAlShammasi  --bob BobAlShammasi  --boa BoaMcCain  --uod UodBeal  --uob UobSalazar  --uoa UoaPerezML  --cob CobMcCainEtAl --coa CoaDeGhetto  --sgo SgoBakerSwerdloff --ppchc PpchcSutton --tpchc TpchcSutton --zfactor ZfactorDranchukAK --cg CgHallYarborough --ug UgCarrKB --bwb BwbMcCain --bwa BwaMcCain --uw  UwMaoDuan --pw  PwMcCain --rsw RswSpiveyMN --cwb CwbMcCain --cwa CwaSpiveyMN --sgw SgwJenningsNewman --grid blackoil   
```
PVT Properties Grid,

```
pb_row Sm3_Sm3,pw Lb_ft3,uw Paxs,bw RB_STB,pg Lb_ft3,ug CP,bg RCF_SCF,cg psia_1,zfactor Zfactor,po Lb_ft3,uo CP,bo RB_STB,co psia_1,rs SCF_STB,p psia,uod CP,pb psia,t Fahrenheit
"false","false",65.451412,15.568571,0.000016,1.741039,62.153208,0.000857,1.003456,3.412440,0.012560,0.013604,0.001142,0.874466,61.680502,9614.734060,1.033326,0.000123,77.969343,981.000000,1132853.078396,1375.228097,80.000000
"false","false",64.784330,13.488716,0.000014,1.958768,62.163794,0.000857,1.003285,4.074542,0.013143,0.011394,0.000987,0.856410,61.658756,8108.359538,1.033962,0.000098,80.000000,1147.153846,1132853.078396,1375.228097,80.000000
"false","false",64.129691,11.686715,0.000012,2.162387,62.175206,0.000857,1.003101,4.755778,0.013737,0.009761,0.000867,0.840009,61.658756,6867.098854,1.033962,0.000081,80.000000,1313.307692,1132853.078396,1375.228097,80.000000
"true","false",63.888909,11.078560,0.000011,2.235061,62.179670,0.000857,1.003029,5.013585,0.013961,0.009260,0.000829,0.834383,61.658756,6454.782447,1.033962,0.000076,80.000000,1375.228097,1132853.078396,1375.228097,80.000000
"true","false",63.888909,11.078560,0.000003,2.235061,62.179670,0.000857,1.003029,,,,,,61.658756,6454.782447,1.033962,0.000000,80.000000,1375.228097,1132853.078396,1375.228097,80.000000
"false","false",63.487493,10.125450,0.000003,2.353805,62.199317,0.000857,1.002712,,,,,,61.658756,6668.801460,1.033962,0.000000,80.000000,1479.461538,1132853.078396,1375.228097,80.000000
"false","false",62.857737,8.772758,0.000003,2.534536,62.230479,0.000857,1.002210,,,,,,61.658756,7009.959552,1.033962,0.000000,80.000000,1645.615385,1132853.078396,1375.228097,80.000000
"false","false",62.240423,7.600777,0.000003,2.705810,62.261450,0.000857,1.001711,,,,,,61.658756,7351.117644,1.033962,0.000000,80.000000,1811.769231,1132853.078396,1375.228097,80.000000
"false","false",61.635551,6.585365,0.000003,2.868638,62.292232,0.000857,1.001216,,,,,,61.658756,7692.275737,1.033962,0.000000,80.000000,1977.923077,1132853.078396,1375.228097,80.000000
"false","false",61.043121,5.705605,0.000003,3.023867,62.322829,0.000857,1.000725,,,,,,61.658756,8033.433829,1.033962,0.000000,80.000000,2144.076923,1132853.078396,1375.228097,80.000000
"false","false",60.463133,4.943375,0.000003,3.172212,62.353243,0.000857,1.000237,,,,,,61.658756,8374.591921,1.033962,0.000000,80.000000,2310.230769,1132853.078396,1375.228097,80.000000
"false","false",59.895587,4.282974,0.000003,3.314287,62.383475,0.000857,0.999752,,,,,,61.658756,8715.750014,1.033962,0.000000,80.000000,2476.384615,1132853.078396,1375.228097,80.000000
"false","false",59.340483,3.710798,0.000003,3.450620,62.413529,0.000857,0.999271,,,,,,61.658756,9056.908106,1.033962,0.000000,80.000000,2642.538462,1132853.078396,1375.228097,80.000000
"false","false",58.797820,3.215060,0.000003,3.581672,62.443406,0.000857,0.998792,,,,,,61.658756,9398.066198,1.033962,0.000000,80.000000,2808.692308,1132853.078396,1375.228097,80.000000
"false","false",58.267600,2.785550,0.000003,3.707846,62.473109,0.000857,0.998318,,,,,,61.658756,9739.224290,1.033962,0.000000,80.000000,2974.846154,1132853.078396,1375.228097,80.000000
"false","false",57.749822,2.413420,0.000003,3.829500,62.502639,0.000857,0.997846,,,,,,61.658756,10080.382383,1.033962,0.000000,80.000000,3141.000000,1132853.078396,1375.228097,80.000000
"false","false",57.244485,2.091003,0.000003,3.946949,62.531997,0.000857,0.997377,,,,,,61.658756,10421.540475,1.033962,0.000000,80.000000,3307.153846,1132853.078396,1375.228097,80.000000
"false","false",56.751591,1.811659,0.000003,4.060476,62.561187,0.000857,0.996912,,,,,,61.658756,10762.698567,1.033962,0.000000,80.000000,3473.307692,1132853.078396,1375.228097,80.000000
"false","false",56.271138,1.569634,0.000003,4.170335,62.590209,0.000857,0.996450,,,,,,61.658756,11103.856660,1.033962,0.000000,80.000000,3639.461538,1132853.078396,1375.228097,80.000000
"false","false",55.803127,1.359941,0.000003,4.276753,62.619066,0.000857,0.995991,,,,,,61.658756,11445.014752,1.033962,0.000000,80.000000,3805.615385,1132853.078396,1375.228097,80.000000
"false","false",55.347558,1.178263,0.000003,4.379939,62.647758,0.000857,0.995534,,,,,,61.658756,11786.172844,1.033962,0.000000,80.000000,3971.769231,1132853.078396,1375.228097,80.000000
"false","false",54.904432,1.020855,0.000003,4.480079,62.676288,0.000857,0.995081,,,,,,61.658756,12127.330937,1.033962,0.000000,80.000000,4137.923077,1132853.078396,1375.228097,80.000000
"false","false",54.473747,0.884475,0.000003,4.577344,62.704657,0.000857,0.994631,,,,,,61.658756,12468.489029,1.033962,0.000000,80.000000,4304.076923,1132853.078396,1375.228097,80.000000
.
.
.
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
