
#ifndef COMPRESSIBILITY_UNIT_H
#define COMPRESSIBILITY_UNIT_H 1

# include "units.H"
# include "pressure-unit.H"

Declare_Physical_Quantity(Compressibility, "Compressibility",
			  "Inverse of a pressure that force is distributed");

Declare_Unit(psia_1, "psia^-1", "Inverse of psia", Compressibility, 1e-6, 1e-3);

Declare_Unit(mPa_1, "MPa^-1", "Inverse of mPascal", Compressibility, 1.45e-4, 1.45e-1);

Declare_Conversion(psia_1, mPa_1, v)
{
  double v_psia = 1/v; // get value in psia
  // double v_mPa = unit_convert<psia, mPascal>(v_psia);
  double v_mPa = 6.894757293178308e-3 * v_psia;
  return 1/v_mPa;
}

Declare_Conversion(mPa_1, psia_1, v)
{
  double v_mPa = 1/v; // get value in mPascal
  // double v_psia = unit_convert<mPascal, psia>(v_mPa);
  double v_psia = v_mPa/6.894757293178308e-3;
  return 1/v_psia;
}

#endif // COMPRESSIBILITY_UNIT_H
