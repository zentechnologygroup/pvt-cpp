
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

Si el resultado final del cálculo se sale del rango
`[min_val, max_val]` entonces la e

## Nombre de la correlación y del archivo que la contiene

Una vez seleccionado el nombre de la correlación, el primer paso es
crear un archivo en el directorio `include`. Es altamente recomendable
que el nombre del archivo esté altamente vinculado con el nombre de la
correlación. 

Supóngase que la correlación se llama `BubblePointPressure`. En este
caso, un nombre de archivo podría ser `bubble-point-pressure.H`. **No
debe olvidarse que este archivo debe estar guardado en el directorio
`include`**.

### Estructura del archivo

Siguiendo el ejemplo anterior 
