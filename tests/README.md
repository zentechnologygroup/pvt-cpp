# Tests

Este directorio contiene los tests de los objetos pvt

## `test-conversion`

	USAGE: 

	./test-conversion  -s <sample> -u <degK|degC|degF|degR|bar|Pa|kPa|psia
	|psig|atm> [--] [--version] [-h]


	Where: 

	-s <sample>,  --sample <sample> (required)  sample

	-u <degK|degC|degF|degR|bar|Pa|kPa|psia|psig|atm>,  --unit-symbol <degK
      |degC|degF|degR|bar|Pa|kPa|psia|psig|atm>
     (required)  symbol of unit

	--,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

	--version
     Displays version information and exits.

	-h,  --help
     Displays usage information and exits.


## `test-all-units-1`

	USAGE: 

	./test-all-units-1  -P <Pressure|Temperature> [-E <precision threshold>]
	[-c] [-s <random seed>] [-m <maximum value of
                       physical magnitude>] [-n <number of samples>] [--]
                       [--version] [-h]

	Where: 

	-P <Pressure|Temperature>,  --physical-quantity <Pressure|Temperature>
     (required)  name of physical quantity

	-E <precision threshold>,  --epsilon <precision threshold>
     precision threshold for numeric comparaisons

	-c,  --csv
     output in csv format

	-s <random seed>,  --seed <random seed>
     seed for random number generator

	-m <maximum value of physical magnitude>,  --max <maximum value of
      physical magnitude> maximum value

	-n <number of samples>,  --num-samples <number of samples>
	 number of random samples

	--,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

	--version
     Displays version information and exits.

	-h,  --help
     Displays usage information and exits.
