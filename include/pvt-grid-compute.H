# ifndef PVTGRID_COMPUTE_H
# define PVTGRID_COMPUTE_H

# include <parse-csv.H>
# include <tpl_array.H>
# include <units.H>

class PvtGrid
{
  Array<string> header;
  Array<const Unit*> units;
  Array<Array<double>> vals;



public:

  double compute(size_t i, double x) const
  {
    return 0;
  }
    
  double compute(const string & name, double x) const
  {
    return 0;
  }

};

# endif
