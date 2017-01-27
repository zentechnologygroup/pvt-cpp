# ifndef METADATA_EXCEPTIONS_H
# define METADATA_EXCEPTIONS_H

# include <zen-exceptions.H>

DEFINE_ZEN_EXCEPTION(VarNameNotFound, "variable not found in empirical set");

DEFINE_ZEN_EXCEPTION(DuplicatedVarName, "Duplicated var name");

DEFINE_ZEN_EXCEPTION(DuplicatedConstName, "Duplicated const name");

DEFINE_ZEN_EXCEPTION(ConstNameNotFound, "constant not found in empirical set");

DEFINE_ZEN_EXCEPTION(ConstOutOfRangeUnit, "constant out of unit range");

DEFINE_ZEN_EXCEPTION(InvalidConst, "invalid const");

DEFINE_ZEN_EXCEPTION(EmptyVarSet, "variable set is empty")

DEFINE_ZEN_EXCEPTION(ColumnIndexNameMismatch,
		     "mismatch between column name and index")



DEFINE_ZEN_EXCEPTION(SamplesSetAlreadySet, "samples set is already set");
DEFINE_ZEN_EXCEPTION(SamplesSetEmpty, "samples set is empty");
DEFINE_ZEN_EXCEPTION(BadSamplesHeader, "Bad samples header");
DEFINE_ZEN_EXCEPTION(SampleIncompleteColumnNumber, "incomplete column number");
DEFINE_ZEN_EXCEPTION(SampleIncompleteRow, "incomplete sample row");
DEFINE_ZEN_EXCEPTION(SampleRowOverflow, "sample row overflow");
DEFINE_ZEN_EXCEPTION(SampleValueIsNotNumber, "sample value is not a number");
DEFINE_ZEN_EXCEPTION(SampleVarNotFound, "value not found");
DEFINE_ZEN_EXCEPTION(SampleDoesNotFit,
		     "sample data does not fit with correlation range");
DEFINE_ZEN_EXCEPTION(SampleOutOfRange, "sample is out of any correlation range");
DEFINE_ZEN_EXCEPTION(SampleContainsTargetName, "sample contains target name");


# endif
