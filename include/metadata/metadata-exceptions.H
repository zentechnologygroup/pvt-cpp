# ifndef METADATA_EXCEPTIONS_H
# define METADATA_EXCEPTIONS_H

# include <aleph-exceptions.H>

DEFINE_ALEPH_EXCEPTION(MismatchAuthors,
		     "Pair of correlations have not the same author");
DEFINE_ALEPH_EXCEPTION(CorrelationNotApplicable, "Correlation is not applicable");
DEFINE_ALEPH_EXCEPTION(VarNameNotFound, "variable not found in empirical set");
DEFINE_ALEPH_EXCEPTION(PressureMismatch, "Pressure values mismatch");
DEFINE_ALEPH_EXCEPTION(AmbiguousDeadViscosity, "Ambiguous uod values"); 
DEFINE_ALEPH_EXCEPTION(DuplicatedVarName, "Duplicated var name");
DEFINE_ALEPH_EXCEPTION(DuplicatedConstName, "Duplicated const name");
DEFINE_ALEPH_EXCEPTION(ConstNameNotFound, "constant not found in empirical set");
DEFINE_ALEPH_EXCEPTION(ConstOutOfRangeUnit, "constant out of unit range");
DEFINE_ALEPH_EXCEPTION(InvalidConst, "invalid const");
DEFINE_ALEPH_EXCEPTION(EmptyVarSet, "variable set is empty")
DEFINE_ALEPH_EXCEPTION(ColumnIndexNameMismatch,
		     "mismatch between column name and index")
DEFINE_ALEPH_EXCEPTION(InvalidProperty, "Invalid property name")
DEFINE_ALEPH_EXCEPTION(InvalidPressureUnit, "Invalid unit for pressure")
DEFINE_ALEPH_EXCEPTION(InvalidTargetUnit, "Invalid target unit");
DEFINE_ALEPH_EXCEPTION(SamplesUnsorted, "samples are not sorted")
DEFINE_ALEPH_EXCEPTION(SamplesSetAlreadySet, "samples set is already set");
DEFINE_ALEPH_EXCEPTION(SamplesSetEmpty, "samples set is empty");
DEFINE_ALEPH_EXCEPTION(BadSamplesHeader, "Bad samples header");
DEFINE_ALEPH_EXCEPTION(SampleIncompleteColumnNumber, "incomplete column number");
DEFINE_ALEPH_EXCEPTION(SampleIncompleteRow, "incomplete sample row");
DEFINE_ALEPH_EXCEPTION(SampleRowOverflow, "sample row overflow");
DEFINE_ALEPH_EXCEPTION(SampleValueIsNotNumber, "sample value is not a number");
DEFINE_ALEPH_EXCEPTION(SampleVarNotFound, "value not found");
DEFINE_ALEPH_EXCEPTION(SampleDoesNotFit,
		     "sample data does not fit with correlation range");
DEFINE_ALEPH_EXCEPTION(LengthMismatch, "invalid sequence length");
DEFINE_ALEPH_EXCEPTION(SampleOutOfRange, "sample is out of any correlation range");
DEFINE_ALEPH_EXCEPTION(SampleContainsTargetName, "sample contains target name");
DEFINE_ALEPH_EXCEPTION(SampleInvalid, "invalid sample value");
DEFINE_ALEPH_EXCEPTION(InvalidTargetName, "invalid correlation target")
DEFINE_ALEPH_EXCEPTION(InvalidJson, "Invalid json");
DEFINE_ALEPH_EXCEPTION(CannotApply, "cannot apply");
DEFINE_ALEPH_EXCEPTION(MetadataException, "metadata data exception");

# endif
