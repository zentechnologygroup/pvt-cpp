# Units management

This file implements a generic units converter which allows to write
formulas in some units and transparently tries to manage automatic
unit conversion if the units are changed.

## Physical Quantities

Units are associated to a `PhysicalQuantity` class. A "physical
quantity" defines a numeric magnitude associated to a phenomenon. In
order to specify a physical quantity, you should use the macro
`Declare_Physical_Quantity(name, symbol, desc)`. By invoking this
macro you creates a new class directly derived from `PhysicalQuantity`
with a name, a symbol and a description. For example:

    Declare_Physical_Quantity(Length, "L", "Distance between two points");

In this case, a new class name `L` is created with associated Length,
symbol `L` and a description.

Each `PhysicalQuantity` derived class is a singleton. That means that
at any time there is only an instance of the class. As such, you
cannot to instantiate any `PhysicalQuantity` derived class. However,
you can access to the instance through the static method:

    const Length & Length::get_instance()

For the previous example, you could do:

    const Length & length = Length::get_instance();

in order to get a reference to the singleton class `Length`. With this
reference, you could directly access the class attributes `name`,
`symbol` and `description` respectively. The class is instantiated the
first time it is used.

You could also access all the physical quantities through the
static method:

    DynList<const PhysicalQuantity *> unit::quantities()

which returns a list of constant pointers to all the physical
quantities being used.

## Units

Any physical quantity is expressed in units. An unit is a previusly
agreed magnitude of a physical quantity which is used for indicating
values. For example, we could establish that a "light year" is the
distance traversed by the light during a year. So, big distances could
be expressed as quantums of a light year.

An unit is defined through the macro:

    Declare_Unit(name, symbol, desc, physical_quantity_name, min, max)

where `name` is the name of unit, `symbol` is the string used for
symbolizing it, `desc` is a description of the unit,
`physical_quantity_name` is the physical quantity to which the unit
refers to and min and max are the minimum and maximum values
respectively that can have a physical quantity expressed with the
unit. For example, we could define a light year as follows:
    
    Declare_Unit(LightYear, "ly", 
                 "distance traversed by the light during a year",
                 Length, numeric_limits<double>::min(), 
                 numeric_limits<double>::max());

In this case, a new class called `LightYear` is introduced to the
class name system. As with the physical quantities, any unit class is
a singleton that cannot be instantiated, but can be referenced with
the static method:

    const LightYear & LightYear::get_instance()

which returns a constant reference to the singleton `Lightyear`.

Any declared unit class has the following attributes:

. `name`: the name of unit
. `symbol`; the string used as symbol of the unit
. `description`
. `physical_quantity`: constant reference to the physical quantity
  to which the unit relates

These attributes can be directly accessed.

Also, you could know all the used units through the following
static method:

    DynList<const PhysicalQuantity *> Unit::quantities()

### Compound units

Many unities are composed two o more others unities. By example, the
speed, which is the rate of change of position of an object, is
represented by the length of an interval by unit of time. In this
case, the unit is denoted as compound and it is declared through:

    Declare_Compound_Unit(name, symbol, desc, physical_quantity_name, 
                          min, max, list-of-units);

The only difference between `Declare_Unit` and `Declare_Compound_Unit`
lies on the fact that the last takes a list of two or more units. In
the case of the speed, we could define it as follows:

    Declare_Compound_Unit(Km_h, "km/h", "Standard measure of speed", 
                          Speed, 0, numeric_limits<double>::max(), 
                          Kilometer, Hour);

Here we are defining the speed unit `Km_h` (Kilometers by hour)
representing the physical quantity `Speed` between the interval `[0,
max]` (max is the maximum value represented with a `double`) and
composed by the units `Kilometer` and `Hour`.

# Validations

The library performs some validations, concretely those related to the
names and symbols. In that sense, neither the physical quantities nor
the unit cannot have the same names or symbols. 

The exception `domain_error` is thrown if a duplicated name or symbol
is detected. These checking are lazily done in running time, at the
moment of the first use of a physical quantity or a name

## Units conversion

The heart of this conversion approach underlies on the following
generic template function:

    template <class SrcUnit, class TgtUnit> double unit_convert(double val);

The goal of this function is to covert a magnitude `val` expressed in
units of type `SrcUnit` to a magnitude expressed in units of type
`TgtUnit` which is the return value. It is the user's responsibility
to write as many functions conversions as units are going to be used
and invoked.

As example, let us suppose that we have already defined the
`Kilometer` unit. Then, the conversion between `LightYear` and
`Kilometer` could be written thus:

    template <class LightYear, class Kilometer> 
    double unit_convert(double val)
    {
      return 9.4604e15 * val;
    }

So, when a quantity expressed in `LightYear`'s is passed to a function
managing kilometers, the above conversion function is called.

Note that the opposite is not allowed while the function
`unit_convert<Kilometer, LightYear>()` is not still written.

## Quantities

The goal of this library is to manage several units and their
conversions in a type safe way. In order to achieve that, the
numerical computations should be done through objects of type
`Quantity`.

`Quantity<Unit>` is a template class whose parameter type is the unit
used for expressing the quantity. Consider for example the following
code snippet:

    Quantity<LightYear> dist = 1000;

defines a variable `dist` containing a `double` whose value is 1000
lightyears.

### Conversions

Conversion are automatically done through the constructors,
assignments, arithmetical operations and comparisons.

A conversion between two unities is valid if and only if they relate
the same physical quantity. By example, meters could be converted to
kilometers because they relate the physical quantity `Length`. At the
contrary, an conversion between meters and seconds will be not valid.

For this version, the library performs this checking at running time,
while a conversion is being performed. A `domain_error` exception will
be thrown if a conversion between physically unrelated unities is
done.

Casting between `Quantity` objects is not allowed. Perform valid
conversion through the constructor.

#### Constructor and assignment

Given a quantity in a unit `U1`, you can get its equivalent in another
unit `U2` by simply instantiating the `Quantity<U2>` copy
constructor. For example:

    Quantity<Meter> dist_mt = 1000;
    Quantity<Kilometer> dist_km = dist_mt; // valid and converted to 1 km

You can also convert via the assignment. For example:

    dist_mt = 10000; // here we are assigning a scalar
    dist_km = dist_mt; // it is converted to 10 km

As suggested in the previous example, you can perfectly construct and
assign `Quantity` objects directly passing them a scalar. Of course,
in this case no conversion is done.

#### Arithmetical operations

The library exports the fundamental arithmetical operations: addition,
subtraction, multiplication and division.

Any arithmetical operation can be done between a `Quantity` object and
a scalar. the scalar can be a `int`, `long`, `float` or `double`
constant or a variable. In this case, no conversion is performed.

Addition and subtraction between quantities expressed in different
units can be done as long as the involved units relate to the same
physical quantity. For example:

    Quantity<Mile> dist_mi = 100;
    dist_mi += dist_km + dist_mt; // + performs conversion from mt to km

In this case, the binary arithmetical operator `+` converts the
second operand to kilometers and then perform the addition. The result
is in kilometers. Afterward, for `+=` operator the kilometers are
converted to miles and added to `dist_mi`.

For `+` and `-` binary operators, the second operand is converted to
the unit of first one. A `domain_error` exception would be thrown if
the units do not relate to the same physical quantity. Addition and
subtraction between quantities relating to different physical
magnitudes are not allowed.

For the `*` and `/` binary operators, no conversion is done because
these operations physically would mean another physical quantity with
units that are function of the operands. For example, an unit of speed
could be `km/h`.

In order to the result of `*` and `/` to be valid, there must exist a
unit linking the units of the two operands. Basically, this is the
goal of a compound unit. So, when a `*` or `/` are invoked, the
library searches the compound unit linking the unities of their
operands. The code should not compile if there is no exist such
compound unit. For example, you could compute the speed as follows:

    Quantity<Hour> time_h = 1;
    auto speed = dist_km/time_h; // valid because Km_h unit was defined

In this case, the division is valid because the speed unit `Km_h` was
defined as a compound unit involving the `Kilometer` and `Hour` units.

If a binary `*` or `/`  is written and the needed compound unit does
not exist, then the construct will not compile

The `*=` and `/=` operators are not defined.

#### Arithmetical expression and compound units

You could extend the syntactic validity of `*` and `/` operator by
specifying compound units. 

For example, this operation:

    auto dist_mt_square = dist_mt*dist_mt;

will not compile because `*` on both operands `Quantity<Meter>` is not
defined. You could however become valid the above expression by doing:

    Declare_Compound_Unit(Mt2, "mt2", "", Distance, 0,
                          numeric_limits<double>::max(), Meter, Meter);

In the same way, you could define

    Declare_Compound_Unit(Mt3, "mt3", "", Distance, 0,
                          numeric_limits<double>::max(), Mt2, Meter);

which would allow

    auto s = dist_meter*dist_meter*dist_meter;

According to the parsing rules for arithmetical expressions, the
compiler processes form left to right. So, the first
`dist_meter*dist_meter` is done which produces a `mt2` unit. Then this
result is multiplied as `(dist_meter*dist_meter)*dist_meter` which
matches with the `mt3` declared unit. Note however that the expression

    auto s = dist_meter*(dist_meter*dist_meter);

will not compile because a compound unit involving meter and square
meter does not exist (although it exists square meter with meter). In
many cases is not worth declaring a compound unit only for satisfying
the previous expression, because you could solve the matter by writing
the expression in an appropriated way. In addition, it is ambiguous and
potentially dangerous to have several units expressing the same thing,

#### Accessing the scalar value

In some cases, especially those involving complicated arithmetical
transformations, it is very probable that you get compiler syntactic
errors without chance of defining a compound unit. A way for
by-passing this is to get the scalar value of the quantity and then
perform all the operations with scalar values. In order to do that,
you could use the `get_value()` method.

As recommendation, encapsulate these scalar value on a function. For
example:

    Quantity<Mt3> volume(const Quantity<Meter> & length,
                         const Quantity<Meter> & width,
                         const Quantity<Meter> & height)
    {
      double l = length.get_value();
      double w = width.get_value();
      double h = height.get_value();
      return Quantity<Mt3>(l*w*h);
   }

In this way, other length units could be used and their conversions
transparently done when the `volume()` function is called.
