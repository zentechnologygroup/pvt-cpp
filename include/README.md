# Headers relacionados a PVT

Este directorio contiene los headers relacionados con la biblioteca PVT

## Definición de una nueva magnitud física y sus unidades

En primer lugar, cree un archivo con sufijo `-unit.H` en el cual se
especificaría la nueva magnitud física y sus unidades. Por ejemplo, si
la magnitud fuese la velocidad, entonces un buen nombre sería
`speed-unit.H`.

Para mitigar el riesgo de duplicidad de símbolos debido a múltiples
inclusiones, envuelva todo lo que ponga en el archivo así:

	# ifndef SPEED_UNIT_H
	# define SPEED_UNIT_H

	... aquí ponen todas las definiciones pertinentes a la magnitud
    física
	
	... etc, etc, etc
	
	# endif // SPEED_UNIT_H
	
Luego de definida la unidad, edite el archivo `pvt-units.H` y añada el
archivo correspondiente a la nueva unidad. Por ejemplo:

    # ifndef PVT_UNITS_H
    # define PVT_UNITS_H

    # include <temperature-unit.H>
    # include <pressure-unit.H>
	# include <speed-unit.H> // <-- Este es el nuevo archivo

    # endif // PVT_UNITS_H

Luego haga 

	cd ../tests
	make depend
	make all
	
Si todo sale bien los tests generados incluirán la(s) nueva(s)
unidad(es) 

## Definición de nueva correlación

PENDIENTE
