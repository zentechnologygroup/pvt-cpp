#ifndef UNIT_EXCEPTIONS_H
#define UNIT_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(InvalidPhysicalQuantity, "Invalid physical quantity")

DEFINE_ZEN_EXCEPTION(WrongSiblingUnit,
		     "Sibling unit is not physically related");

DEFINE_ZEN_EXCEPTION(WrongUnitRatio, "wrong unit ratio");

DEFINE_ZEN_EXCEPTION(UnitConversionNotFound, "Unit conversion not found");

DEFINE_ZEN_EXCEPTION(DuplicatedUnitConversion, "duplicated unit conversion")

DEFINE_ZEN_EXCEPTION(OutOfUnitRange, "Value is out of unit range");

DEFINE_ZEN_EXCEPTION(DifferentUnits,
		     "Binary Operation between involves different units");

DEFINE_ZEN_EXCEPTION(UnitNotFound, "unit not found");

DEFINE_ZEN_EXCEPTION(CompoundUnitNotFound, "compound unit not found");

DEFINE_ZEN_EXCEPTION(UnitException, "unit exception");

#endif
