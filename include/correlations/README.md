# Correlaciones

Este documento explica los conceptos necesarios para usar
correlaciones en la biblioteca Zen.

El centro del sistema de cálculo es la idea de "correlación". Por lo
general, pero no siempre, una correlación es una fórmula que aproxima
el comportamiento de una propiedad física sin que se haya establecido
una relación causal. Se trata de una "aproximación", a menudo basada
en estudios estadísticos, cuya calidad depende de cuan tanto se
aproxime el objeto físico, en este caso un fluido, a las condiciones
de desarrollo de la correlación.

## Jerarquía de clases

Hay cuatro niveles de clases de objetos: `Correlation`, tipo, subtipo
y la correlación misma. Por ejemplo, la correlación de punto de
burbuja de AlMarhoun se define del siguiente modo:

	Correlation
	    ^
		|
	OilCorrelation
		^
		|
	BubblePointPressure
		^
		|
	PbAlMarhoun

La correlación en cuestión se define en la clase `PbAlMarhoun`, con
subtipo `BubblePointPressure`, tipo `OilCorrelation` y que es una
correlación a ser derivada de `Correlation`. 

### La clase `Correlation`

El primer llamado `Correlation`, es una clase abstracta contentiva de
toda la funcionalidad común a toda correlación (pase de parámetros,
invocación, validación, etc).

Esta clase es abstracta, lo que significa que no puede ser
instanciada. Para poder usarla se requiere una correlación
específica; en el ejemplo de la sección anterior una referencia o
puntero a `PbAlMarhoun`.

#### Parámetros de una correlación

Hay una clase auxiliar llamada `CorrelationPar` empleada para definir
los parámetros de una correlación. Todo el manejo en esta clase se
efectúa mediante acceso directo a sus atributos. De estos los
principales son:

- `name`: nombre del parámetro.
- `description`: texto descriptivo
- `unit`: referencia a la unidad en que se expresa el resultado del
  cálculo. 
- `min_val`: objeto `VtlQuantity` que expresa el valor mínimo del
  resultado junto con su unidad.
- `max_val`: objeto `VtlQuantity` que expresa el valor máximo del
  resultado junto con su unidad.
- `latex_symbol`: string que denota el símbolo LaTeX

Los atributos son constantes; es decir, no se pueden modificar.

Dada una correlación, el método `get_preconditions()` proporciona una
referencia a una lista de objetos `CorrelationPar` correspondiente a
los parámetros.

##### Convención sobre los nombres de parámetros

Los nombres de los parámetros son significativos y no pueden
escogerse ni seleccionarse arbitrariamente. **Es muy importante respetar
esta convención**. Por ejemplo, la presión se nota con el nombre `p`, el factor
volumétrico `bo`, la viscosidad por debajo del punto de burbuja como
`uob`, etc. Una lista, sin descripción, de todos los nombres de
parámetros empleados en todas las correlaciones puede obtenerse
mediante el método estático:

	Correlation::all_parameter_names()

#### Sinónimos o alias de parámetros

Un parámetro puede tener un sinónimo; es decir un nombre y unidad
alternativo. Un ejemplo notable es parametro `api` expresaso en unidad
`Api` para el cual muchas veces se define el sinónimo `yo` expresado
en la unidad `Sg_do`. 

#### Atributos de `Correlation`

La clase `Correlation` tiene algunos atributos constante de acceso
público, entre los cuales vale la pena mencionar:

- `type_name`: nombre del tipo de correlación
- `subtype_name`: nombre del subtipo de correlación
- `name`: nombre de la correlación
- `latex_symbol`
- `author`: autor(es) de la correlación
- `unit`: referencia a la unidad en que se expresa el resultado
- `min_val`: mínimo valor de resultado 
- `max_val`: máximo valor de resultado 
- `title`: título de la correlación
- `db`: lista de `string` que conforman la descripción de la correlación
- `notes`: lista de `string` que conforman notas
- `DynList<const BibEntry*> refs`: lista de referencias 
- `hidden`: indica si la correlación es o no oculta; es decir, si
  puede o no ser públicamente usada
- `id`: identificador entero de la correlación

#### Convención sobre el nombre de la correlación

Los primeros símbolos alfabéticos del nombre de la correlación deben
contener, imperativamente, el nombre genérico de la propiedad física
hasta la siguiente letra mayúscula. Por ejemplo, `PbAlMarhoun`
caracteriza el nombre `pb`, cual es el nombre empleado en la biblioteca
para referir al punto de burbuja.

El método `target_name()` de `Correlation` retorna el nombre de la
propiedad física. **Séase extremadamente cuidadoso de respetar esta
convención, así como también la convención sobre los nombres de los
parámetros, pues toda la automatización subyace sobre este acuerdo.**.

#### Firma de la correlación

La firma de una correlación es el nombre de la propiedad que
caracteriza más los nombres de sus parámetros. Por ejemplo, la firma
de `PbAlMarhoun` es una lista conformada por: `pb`, `yg`, `yo`, `rsb`,
`t`. Nótese que `pb` es la propiedad y que ésta se obtiene del nombre
de la correlación.

La firma se obtiene a través del método `DynList<string> names()`.

Puesto que los parámetros pueden tener sinónimos, una correlación
podría tener varias firmas. Para acceder a las distintas firmas, se
puede usar el método `DynList<DynList<string>> names_and_synonyms()`,
el cual retorna una lista de listas de nombres. En el caso de
`PbAlMarhoun`, el resultado es el siguiente:

	`yg`, `yo`, `rsb`, `t`
	`yg`, `api`, `rsb`, `t`
	
Pues esta correlación define el sinónimo `api` para el parámetro
`yo`. Note que este método no retorna el nombre de la propiedad.

#### Invocación de la correlación

Hay varias maneras y modos de invocar la correlación: con parámetros por
nombre, por listas de parámetros, con validación de rangos y llamada
directa a la correlación. La elección es un compromiso entre la
facilidad, validación y performance.

##### Obtención de referencia o puntero a una correlación

Las correlaciones son clases **singleton**. Esto quiere decir que en
cualquier momento de la ejecución sólo puede existir una sola
instancia de clase. Hay varias maneras de obtener una referencia a una
correlación. 

La primera de ellas es mediante el método estático
`get_intance()`. Por ejemplo: 

	const auto & ref = PbAlMarhoun::get_instance();
	
retorna una referencia a la correlación `PbAlMarhoun`. Este es el
mecanismo de más alto desempeño. Se garantiza que la referencia es
obtenida en tiempo de compilación.

En algunos casos no es posible conocer la correlación hasta que no se
inicie la ejecución (una aplicación iterativa, por ejemplo). Para
estas situaciones se puede usar el método estático:

	static const Correlation * search_by_name(const string & name)
	
el cual en tiempo logarítmico retorna un puntero a la correlación con
nombre `name`.

##### Invocación pasando parámetros por nombre

El siguiente método efectúa una llamada a la correlación pasando
parámetros por nombre:

    VtlQuantity compute_by_names(const DynList<NamedPar> & pair_list,
                                 bool check = true) const
								 
El tipo `NamedPar` es una tupla de cuatro elementos definida como
`tuple<bool, string, double, const Unit*>`. El primer campo indica si
el parámetro es válido o no. El segundo indica el nombre del
parámetro. El tercero el valor del parámetro. Finalmente, el cuarto
indica la unidad en que está pasado el parámetro. 

Suponga una función `NPAR(name, value, unit_symbol)` que crea una
tupla válida y una referencia `corr` a la correlación `PbAlMarhoun`. Entonces
una posible invocación podría ser:

	corr.compute_by_names({NPAR("t", 180, "degC"), 
	                       NPAR("yg", 0.848, "Sgg"), 
						   NPAR("api", 27.3, "Api"), 
						   NPAR("rsb", 807, "scf/STB")});
						  
La ventajas de este enfoque es que los parámetros pueden estar en
cualquier orden y las conversiones de unidades y validaciones se efectúan
transparentemente. Su desventaja es el impacto en el performance;
entre 60 y 75 % del coste total.

La mayoría de las llamadas contienen como último parámetro un valor
lógico llamado `check`, el cual es por omisión es `true` e indica si
los rangos de desarrollo de la correlación deben ser o no validados.

##### Aplicación de parámetros de calibración

A veces es deseable aplicar una transformación lineal sobre el
resultado de la correlación. Si `C(x1, x2, ... xn)` es una
correlación, entonces la calibración consistiría en:

	c + m C(x1, x2, ... xn)
	
Para todos métodos de invocación a una correlación, existe una
variante con prefijo `tuned` la cual recibe como parámetros
adicionales los valores de `c` y `m`. De este modo, el método

	VtlQuantity
	tuned_compute_by_names(const DynList<NamedPar> & pair_list,
		                   double c, double m, bool check = true) const
	
es la versión que instrumenta la calibración del método descrito en la
sección anterior.

##### Invocación pasando listas de parámetros

El siguiente nivel de invocación es mediante una lista de 
parámetros ordenada según el orden de especificación de los
parámetros de la correlación. Este enfoque es más rápido que el
anterior y efectúa las conversiones y validaciones necesarias.

El método se define del siguiente modo:

	VtlQuantity compute(const DynList<VtlQuantity> &, bool check = true)
	
Al tratarse de una lista de objetos de tipo `VtlQuantity`, ésta puede
recibir parámetros de tipo `VtlQuantity`, `Quantity` o  directos de
tipo `double`, pero en el último caso se asume que el valor está
expresado en las unidad conque se especificó el parámetro. El ejemplo
base podría expresarse del siguiente modo:

	ret = corr.compute({Quantty<Sgg>(0.848), 
	                    Quantity<Api>(27.3), // Aquí ocurre conversión
		                807, // al ser double scf/STB como unidad
						Quantity<Celsius>(180)}); // aquí ocurre conversión

El objeto resultado es de tipo `VtlQuantity`. Nótese que la lista está
expresada entre llaves `{}`.

Si todos los elementos de la lista son de tipo `double`, entonces no
ocurre ninguna conversión y el resultado es de tipo `double`.

Una manera más concisa y ligeramente más rápida es sin construir la
lista intermedia. Para ello se usa el mismo método `compute` pero que
recibe como primer primer parámetro `check`. Así, la llamada anterior
sería:

	ret = corr.compute(false, 
                       Quantty<Sgg>(0.848), 
	                   Quantity<Api>(27.3), // Aquí ocurre conversión
		               807, // al ser double scf/STB como unidad
					   Quantity<Celsius>(180)); // aquí ocurre conversión

##### Racional sobre el pase de parámetros por listas

Hasta ahora los métodos de invocación reciben listas, lo cual brinda
mucha mayor flexibilidad al momento de construir aplicaciones
iterativas. Considere por ejemplo una gui en la cual se seleccionen
las correlaciones por widgets tipos combobox. Puesto que las
correlaciones varían en cantidad y tipos de parámetros, la construcción
de un combobox con los parámetros para todas la correlaciones sería
muy tediosa si no se tuviesen listas; probablemente se tendría que
construir una tabla de correlaciones o apelar a multiples
`if-else-if`. Con el pase por listas, aunado a que cada correlación
refleja toda la información necesaria para invocarla, la
instrumentación del combobox se hace en unas tres líneas de código. 

El pase de parámetros por nombre también brinda mucha flexibilidad,
pues no es necesario conocer o mantener el orden. La generación de las
tablas PVT para cualquier conjunto de correlaciones hace uso de este
mecanismo.

Sin embargo, este enfoque es más lento que el pase directo de
parámetros y podría ser un problema si la correlación se invoca
muchísimas veces, como es el caso de algunos escenarios de
simulación. Para paliar estas situaciones se posibilita el pase
directo de parámetros.

##### Invocación directa con unidades

Un primer sabor es usando el operador `()` y pasándole a la
correlación valores y unidades; es decir, objetos de tipo
`VtlQuantity` o `Quantity`. En el caso anterior, la correlación
`PbAlMarhoun` podría invocarse así:

	PbAlMarhoun::get_instance().(Quantty<Sgg>(0.848), 
	                             Quantity<Api>(27.3), // Aquí ocurre conversión
		                         807, // al ser double scf/STB como unidad
					             Quantity<Celsius>(180)); // aquí ocurre conversión

En este caso, las conversiones serán efectuadas y las precondiciones
que se hayan especificado para la correlación serán verificadas.

Si se tiene el conocimiento de que las precondiciones no serán
violadas, entonces se puede omitir su verificación, y por consiguiente
calcular más rápido, usando el método `call`:

	PbAlMarhoun::get_instance().call(Quantty<Sgg>(0.848), 
                                     Quantity<Api>(27.3), // Aquí ocurre conversión
                                     807, // al ser double scf/STB como unidad
                                     Quantity<Celsius>(180)); // aquí ocurre conversión

Las dos versiones anteriores requieren que se tenga una referencia a
la correlación. Sin embargo, a veces esto no es trivial y lo que se
tiene es un referencia o puntero a un objeto `Correlation`. En este
caso, la llamada a `call()` se puede instrumentar mediante una la función
`direct_call()`. En nuestro ejemplo sería así

	direct_call(ref_corr, Quantty<Sgg>(0.848), 
	            Quantity<Api>(27.3), // Aquí ocurre conversión
                807, // al ser double scf/STB como unidad
                Quantity<Celsius>(180)); // aquí ocurre conversión

En este caso, `ref_corr` es una referencia de tipo `Correlation` a la
correlación `PbAlMarhoun`.

##### Invocación directa

El último nivel de invocación es mediante llamada directa a la
implementación de la correlación. En este caso el coste de ejecución
es mínimo y completamente equivalente a una implementación `C++` o
`C`.

La llamada a la implementación se efectúa mediante el método `impl()`,
el cual recibe objetos de tipo `double` y deben estar en el orden
exacto de declaración. En la línea de nuestro ejemplo sería así:

	PbAlMarhoun::get_instance().impl(0.848, 0.891058, 807, 815.67);

Es extremadamente importante considerar que no hay conversión de
unidades. Se asume que los valores están expresados en la unidad
conque se especificó el parámetro.

La llamada directa usando una referencia de tipo `Correlation` se
efectúa mediante:

	direct_impl(ref_corr, 0.848, 0.891058, 807, 815.67);
	
De nuevo, `ref_corr` es una referencia de tipo `Correlation` a la
correlación `PbAlMarhoun`.

## Definición e implementación de una correlación

Para poder especificar una nueva correlación es necesario conocer el
tipo y el subtipo. Como ya se ha dicho, en nuestro ejemplo el tipo es
`OilRelation` y el subtipo `BubblePointPressure`. 

La idea del tipo y subtipo es definir una especie de taxonomía que
catalogue las correlaciones en conjuntos jerárquicos según propiedades
físicas y uso. Esencialmente existen los siguientes tipos:

- `TmpFunction`: una función temporal, no destinada a uso público si a
ser empleada por otras correlaciones.
- `OilFunction`: una función causal asociada al petróleo.
- `OilCorrelation`: una correlación asociada al petróleo.
- `GasCorrelation`: una correlación asociada al gas.
- `GasFunction`: una función asociada al gas.
- `WaterCorrelation`: una correlación asociada al agua

### Definición de un tipo de correlación

Para definir un nuevo tipo se emplea el macro
`Declare_Correlation_Type`. Por ejemplo, si se desea crear una familia
llamada `WaterFunction`, entonces basta con

	Declare_Correlation_Type(WaterFunction);

Si el tipo no es destinado a uso público, entonces puede indicarse que
el tipo es **oculto**. En este caso la declaración sería así

	Declare_Hidden_Correlation_Type(WaterFunction);

### Definición de un subtipo de correlación

Para definir un nuevo subtipo se emplea el macro
`Declare_Correlation_Subtype`. Para el caso de `BubblePointPressure`
la llamada es así:

	Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation, "P_b");

En primer parámetro es el nombre del subtipo, el segundo es el nombre
del tipo, el cual ya debe estar definido y el último es el símbolo
LaTeX asociado a la propiedad física del subtipo.

## Definición de una nueva correlación

Las correlaciones se definen en un metalenguaje basado en Ruby. 

Para cada subtipo se debe escribir un archivo Ruby que agrupe todas
las correlaciones asociadas al subtipo. El archivo debe
imperativamente tener la extensión `.rb` y residir en el directorio
`$(TOP)/include/correlations`. En este directorio no pueden residir
otros archivos con extensión `.rb` que no se correspondan con la
definición de las correlaciones asociadas a un tipo.

El `Makefile` en `$(TOP)/include/correlations` está configurado para
generar los archivos de las correlaciones.

Hay cuatro archivos asociados a un subtipo de correlación. Por
ejemplo, para el caso de `BubblePointPressure` están los siguientes:

- `bubble-point-pressure.rb`: aquí residen las especificaciones Ruby
de cada una de las correlaciones asociadas a
`BubblePointPressure`. 
- `bubble-point-pressure.H`: Aquí residen las especificaciones `C++`
de las correlaciones asociadas a `BubblePointPressure`. Este archivo
es generado automáticamente.
- `bubble-point-pressure.cc`: Aquí residen las especificaciones `C++`
de para llamadas externas a las correlaciones asociadas a
`BubblePointPressure`. Este archivo es generado automáticamente. 
- `bubble-point-pressure-impl.H`: aquí residen las implementaciones
de las correlaciones asociadas a `BubblePointPressure`. 

Así pues, la definición de una correlación involucra dos pasos:

1. Especificar la correlación en el archivo Ruby
2. Escribir la implementación C++ en el archivo con extensión `-impl.H`.

### Especificación en Ruby


### Escritura de la implementación




