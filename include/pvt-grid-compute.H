# ifndef PVT_GRID_COMPUTE_H
# define PVT_GRID_COMPUTE_H

# include <parse-csv.H>
# include <tpl_array.H>
# include <tpl_dynMapTree.H>
# include <units.H>

DEFINE_ZEN_EXCEPTION(MismatchInPressureValues, "pressure values does not match");
DEFINE_ZEN_EXCEPTION(UnsortedPressureValues, "pressure values are not sorted");

class PvtGrid
{
  const Unit * tunit_ptr = nullptr;
  const Unit * punit_ptr = nullptr;

  // names and units alphabetically ordered by name as read in the
  // csv's header except t and p
  Array<pair<string, const Unit*>> var_names; 

  struct Desc
  {
    Array<double> p;
    Array<Array<double>> vals;
  };

  Array<double> p; 

  //          t,          matrix of values associated to temperature t
  // columns in second are ordered by name in var_names array
  Array<pair<double, Array<Array<double>>>> temps; 

  void process_row(const Array<string> & row,
		   DynMapTree<double, Desc> & tmap,
		   size_t row_idx, size_t ncol, size_t tidx, size_t pidx)
  {
    if (row.size() != ncol)
      ZENTHROW(InvalidCsvRow, "invalid size of " + to_string(row_idx) +
	       "-th row");
    if (not row.all([] (auto &s) { return is_double(s); }))
      ZENTHROW(InvalidConversion, "a value in row " + to_string(row_idx) +
	       " cannot be converted to double");

    double t = atof(row(tidx));
    Desc & desc = tmap[t];
    Array<double> vals;
    for (size_t i = 0; i < ncol; ++i)
      {
	double val = atof(row(i));
	if (i == pidx)
	  desc.p.append(val);
	else if (i != tidx)
	  vals.append(val);
      }
    assert(vals.size() == ncol - 2);
    desc.vals.append(move(vals));
  }

public:

  PvtGrid(istream & in)
  {
    Array<string> header = csv_read_row(in);
    size_t i = 0, tidx = 0, pidx = 0;
    for (auto it = header.get_it(); it.has_curr(); it.next(), ++i)
      {
	auto & s = it.get_curr();
	auto parts = split(s, ' ');
	if (parts.size() != 2)
	  ZENTHROW(InvalidCsvHeader, "Invalid format in column " + to_string(i));
	const string & var_name = parts[0];
	const string & unit_name = parts[1];
	const Unit * unit_ptr = Unit::search(unit_name);
	if (unit_ptr == nullptr)
	  ZENTHROW(UnitNotFound, "unit " + unit_name + " not found");
	if (var_name == "t")
	  {
	    tunit_ptr = unit_ptr;
	    tidx = i;
	  }
	else if (var_name == "p")
	  {
	    punit_ptr = unit_ptr;
	    pidx = i;
	  }
	else
	  var_names.append(make_pair(var_name, unit_ptr));
      }

    const size_t & ncol = header.size();
    DynMapTree<double, Desc> tmap;
    for (size_t i = 1; in.good(); ++i)
      {
	Array<string> row = csv_read_row(in);
	if (row.size() == 0)
	  break;
	process_row(row, tmap, i, ncol, tidx, pidx);
      }

    auto & fst = tmap.get_first();
    Desc & desc = const_cast<Desc&>(fst.second);
    p = move(desc.p);
    temps.append(pair<double,
		 Aleph::Array<Aleph::Array<double>>>(fst.first,
						     move(desc.vals)));

    // TODO: falta ordenar columnas

    if (not is_sorted(p))
      ZENTHROW(UnsortedPressureValues, "pressure values associated to temp " +
	       to_string(fst.first) + " are not sorted");

    for (auto it = tmap.get_it(1); it.has_curr(); it.next())
      {
	auto & curr = it.get_curr();
	const double & t = curr.first;
	Desc & desc = curr.second;

	if (not zip_all([] (auto t) { return get<0>(t) == get<1>(t); },
			p, desc.p))
	  ZENTHROW(MismatchInPressureValues,
		   "Found inconsistency en pressure values associated to temp" +
		   to_string(t));

	temps.append(pair<double, Array<Array<double>>>(t, move(desc.vals)));
      }
  }

  friend ostream & operator << (ostream & out, const PvtGrid & grid)
  {

    return out;
  }

  bool check(istream & in) const
  {

  }

  double compute(double t, double p, const string & name) const
  {
    
  }

};

# endif
