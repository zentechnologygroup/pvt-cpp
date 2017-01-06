
# Manufactura de una correlación

El siguiente texto describe el proceso para definir una nueva
correlación.

## Atributos de una correlación

Una correlación tiene los siguientes atributos:

- `type_name`: el tipo de correlación
- `subtype_name`"el subtipo de la correlación
- `name`: el nombre de la correlación
- `unit`: la unidad en que está representado el resultado de la correlación
- `min_val`: el mínimo valor que puede tomar el resultado
- `max_val`: el máximo valor que puede tomar el resultado

Hay tres tipos de correlación: `OilCorrelation`, `GasCorrelation` y
`WaterCorrelation`. 

Si la unidad del resultado no es afín a `unit` entonces se genera la
excepción `domain_error`. 

Si el resultado final del cálculo se sale del rango
`[min_val, max_val]` entonces se genera la excepción `range_error`.

## Subtipos de correlación

Las correlaciones se organizan por subtipos. Por cada subtipo debe
existir un archivo lo más homónimo posible. Por ejemplo, las
correlaciones de punto de burbuja para crudo se ponen en
`bubble-point-pressure.H`, el cual se estructura así:

    # ifndef BUBBLE_POINT_PRESSURE_H
    # define BUBBLE_POINT_PRESSURE_H

    # include <correlation.H>
	
	Declare_Correlation_Subtype(BubblePointPressure, OilCorrelation);
	//        ^
	//        |
	//        --- Este macro define el subtipo de correlación Bubblepointpressure
	
	// ... Definición de la correlación 1
	// ... Definición de la correlación 2
	
	...
	
	// ... Definición de la correlación n
	
	# endif // BUBBLE_POINT_PRESSURE_H
	
Este archivo debe imperativamente residir en el directorio `include`.

Cada vez que se escriba una nueva familia de correlaciones asociada a
un subtipo se debe proceder a efectuar su inclusión en el header
`pvt-correlations.H`. 

El macro `Declare_Correlation_Subtype` define una familia de
correlaciones asociada al tipo `Oilcorrelation`.

Cada correlación se define mediante una clase públicamente derivada
del subtipo definido mediante el macro explicado en el párrafo anterior.

## Definición de una correlación

Una vez seleccionado definido el subtipo, se procede a definir las
correlaciones, las cuales deben residir en el mismo archivo
especificado para el tipo.

Supóngase por ejemplo la correlación de presión de punto de burbuja de
AlMarhoun. El primer paso es definir la clase:

    class PbAlMarhounCorrelation : public BubblePointPressure
    {
      Correlation_Singleton(PbAlMarhounCorrelation);
	  
	  ...
    };
	
`Correlation_Singleton` es un macro que hace que la clase
`PbAlMarhounCorrelation` sea un singleton. Es obligatorio que
parámetro pasado a este macro coincida exactamente con el nombre de la
clase que define la correlación.

el nombre de la correlación, el primer paso es
crear un archivo en el directorio `include`. Es altamente recomendable
que el nombre del archivo esté altamente vinculado con el nombre de la
correlación. 

Supóngase que la correlación se llama `BubblePointPressure`. En este
caso, un nombre de archivo podría ser `bubble-point-pressure.H`. **No
debe olvidarse que este archivo debe estar guardado en el directorio
`include`**.

Asegurarse de poner el guardia del header acorde al nombre del
archivo. Por ejemplo:




