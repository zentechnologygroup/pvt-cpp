
# include <istream>

# include <tclap/CmdLine.h>
# include <correlations/correlation.H>

using namespace TCLAP;

const Unit * search_unit(const string & name)
{
  auto unit_ptr = Unit::search_by_name(name);
  if (unit_ptr == nullptr)
    {
      unit_ptr = Unit::search_by_symbol(name);
      if (unit_ptr == nullptr)
	{
	  cout << "Unit " << name << " not found" << endl;
	  abort();
	}
    }
  return unit_ptr;
}

void convert(const Unit * src_unit, const Unit * tgt_unit, istream & in)
{
  double val;
  while (in >> val)
    cout << unit_convert(*src_unit, val, *tgt_unit) << " ";
  cout << endl;
}

void list_all_units()
{
  DynList<DynList<string>> rows;
  PhysicalQuantity::quantities().for_each([&rows] (auto & pq)
    {
      rows.append(pq->units().template maps<DynList<string>>([&pq] (auto p)
        {
	  return DynList<string>({ pq->name, pq->symbol, pq->latex_symbol,
		p->name, p->symbol, p->latex_symbol });
	}));
    });
  rows.insert({"Physical-Quantity", "symbol", "LaTeX symbol", "Unit name",
	"Unit symbol", "LaTeX symbol"});
  cout << to_string(format_string(rows)) << endl;
  exit(0);
}

void ruby_hash()
{
  cout << "{" << endl;
  Unit::units().for_each([] (const Unit * ptr)
    {
      cout << "  '" << ptr->name << "' => '" << ptr->symbol << "'," << endl;
    });
  cout << "}" << endl;
  exit(0);
}

void list_sibling_units(const Unit & unit)
{
  DynList<DynList<string>> rows = unit.family_units().maps<DynList<string>>
    ([] (auto ptr)
     {
       return DynList<string>({ptr->name, ptr->symbol, ptr->latex_symbol});
     });
  rows.insert({"name", "symbol", "LaTeX symbol"});
  cout << to_string(format_string(rows)) << endl;
  exit(0);
}

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  vector<string> units =
    to_vector(Unit::units().maps<string>([] (auto u) { return u->symbol; }));
  ValuesConstraint<string> allowed(units);
  ValueArg<string> unit = { "u", "unit-symbol", "symbol of unit",
			    false, "", &allowed, cmd };

  MultiArg<string> unit_desc = { "U", "Unit-symbol", "describe unit",
				 false, &allowed, cmd };

  vector<string> pqs =
    to_vector(PhysicalQuantity::quantities().maps<string>([] (auto p)
    { return p->name; }));
  ValuesConstraint<string> pq_allowed(pqs);
  ValueArg<string> unit_list = { "L", "list-units",
				 "list units associated to a physical quantity",
				 false, "", &pq_allowed, cmd };

  ValueArg<double> sample = {"s", "sample", "sample", false, 0, "sample", cmd};

  ValueArg<string> source = {"S", "source-unit", "source unit", false,
			     "", "source unit", cmd};
  ValueArg<string> target = {"T", "target-unit", "target unit", false,
			     "", "target unit", cmd};

  SwitchArg l = { "l", "list", "list all units", cmd, false };

  SwitchArg v("v", "verbose", "verbose mode", cmd, false);

  SwitchArg json("j", "json", "json list of units", cmd, false);

  SwitchArg ruby("r", "ruby", "ruby list of units", cmd, false);

  ValueArg<string> file = { "f", "file", "input file name", false, "",
			    "input file name", cmd };
  SwitchArg pipe = { "p", "pipe", "input by cin", cmd };

  cmd.parse(argc, argv);

  if (l.getValue())
    if (not unit.isSet())
      list_all_units();
    else
      list_sibling_units(*search_unit(unit.getValue()));

  if (ruby.getValue())
    ruby_hash();

  if (unit_list.isSet())
    {
      auto pq_name = unit_list.getValue();
      auto pq = PhysicalQuantity::search(pq_name);
      if (pq == nullptr)
	{
	  cout << "Physical quantity " << pq_name << " not found" << endl;
	  abort();
	}
      pq->units().for_each([] (auto uptr) { cout << uptr->name << endl; });
      exit(0);
    }

  if (json.isSet())
    {
      cout << units_json() << endl;
      exit(0);
    }

  auto verbose = v.getValue();

  if (unit_desc.isSet())
    {
      for (const auto & s : unit_desc.getValue())
	cout << Unit::search_by_symbol(s)->to_string(50, 2) << endl
	     << endl;
      exit(0);
    }

  if (source.isSet() and target.isSet())
    {
      auto src_ptr = search_unit(source.getValue());
      auto tgt_ptr = search_unit(target.getValue());

      if (file.isSet() or pipe.getValue())
	{
	  if (pipe.isSet())
	    convert(src_ptr, tgt_ptr, cin);
	  else
	    {
	      ifstream in(file.getValue());
	      if (not in.good())
		{
		  cout << "Cannot open " << file.getValue() << endl;
		  abort();
		}
	      convert(src_ptr, tgt_ptr, in);
	    }
	  exit(0);
	}

      VtlQuantity val(*src_ptr, sample.getValue());
      cout << VtlQuantity(*tgt_ptr, val).raw() << endl;
      exit(0);
    }

  if (not unit.isSet() and not sample.isSet())
    {
      cout << "Flags -u and -s must be set" << endl;
      abort();
    }

  auto unit_ptr = search_unit(unit.getValue());
  if (unit_ptr == nullptr)
    {
      cout << "Unit symbol " << unit.getValue() << " not found" << endl;
      abort();
    }

  cout << *unit_ptr << endl
       << endl
       << "Conversions:" << endl
       << endl;

  VtlQuantity val(*unit_ptr, sample.getValue());

  for (auto u : Unit::units(unit_ptr->physical_quantity))
    {
      if (verbose)
	cout << "    " << val << " to " << u->symbol << endl;
      if (exist_conversion(*unit_ptr, *u))
	cout << "    " << val << " = " << VtlQuantity(*u, val) << endl;
      else
	cout << "    "  << val << " = "
	     << "WARNING: there is no conversion from " << unit_ptr->symbol
	     << " to " << u->symbol << endl;
    }
  cout << endl;
}

int main(int argc, char *argv[])
{
  test(argc, argv);
  return 0;
}
