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
	
Todas las invocaciones a la correlación contienen una variante con
prefijo `tuned` la cual recibe como parámetros adicionales los valores
de `c` y `m`. De este modo, el método

	VtlQuantity
	tuned_compute_by_names(const DynList<NamedPar> & pair_list,
		                   double c, double m, bool check = true) const
	
instrumenta la calibración.

##### Invocación pasando listas de parámetros



##### Invocación directa con unidades

##### Invocación directa


### Tipos de correlaciones

### Subtipos de correlaciones

### Definición de correlación

## Definición de una nueva correlación

### Especificación en Ruby

### Generación de archivos

### Escritura de la implementación

## Utilización
