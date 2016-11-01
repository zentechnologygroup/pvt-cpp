
# include <tclap/CmdLine.h>
# include <correlations/correlation.H>

using namespace TCLAP;

void test(int argc, char *argv[])
{
  CmdLine cmd(argv[0], ' ', "0");

  vector<string> units;
  Unit::units().for_each([&units] (auto u) { units.push_back(u->symbol); });
  ValuesConstraint<string> allowed(units);
  ValueArg<string> unit = { "u", "unit-symbol", "symbol of unit",
			    false, "", &allowed, cmd };

  MultiArg<string> unit_desc = { "U", "Unit-symbol", "describe unit",
				 false, &allowed, cmd };

  vector<string> pqs =
    to_vector(PhysicalQuantity::quantities().maps<string>([] (auto p)
    { return p->name; }));
  ValuesConstraint<string> pq_allowed(pqs);
  ValueArg<string> list = { "L", "list-units",
			    "list units associated to a physical quantity",
			    false, "", &pq_allowed, cmd };

  ValueArg<double> sample = {"s", "sample", "sample", false, 0, "sample", cmd};

  ValueArg<string> source = {"S", "source-unit", "source unit", false,
			     "", "source unit", cmd};
  ValueArg<string> target = {"T", "target-unit", "target unit", false,
			     "", "target unit", cmd};

  SwitchArg v("v", "verbose", "verbose mode", cmd, false);

  SwitchArg json("j", "json", "json list of units", cmd, false);

  cmd.parse(argc, argv);

  if (list.isSet())
    {
      auto pq_name = list.getValue();
      auto pq = PhysicalQuantity::search(pq_name);
      if (pq == nullptr)
	{
	  cout << "Physical quantity " << pq_name << " not found" << endl;
	  abort();
	}
      pq->units().for_each([] (auto uptr) { cout << uptr->symbol << endl; });
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
      auto src_ptr = Unit::search_by_symbol(source.getValue());
      if (src_ptr == nullptr)
	{
	  cout << "Source unit symbol " << source.getValue()
	       << " not found" << endl;
	  abort();
	}
      auto tgt_ptr = Unit::search_by_symbol(target.getValue());
      if (tgt_ptr == nullptr)
	{
	  cout << "Target unit symbol " << target.getValue()
	       << " not found" << endl;
	  abort();
	}
      // if (not exist_conversion(*src_ptr, *tgt_ptr))
      // 	{
      // 	  cout << "Conversion from " << source.getValue() << " to "
      // 	       << target.getValue() << " not found" << endl;
      // 	  abort();
      // 	}

      VtlQuantity val(*src_ptr, sample.getValue());
      cout << VtlQuantity(*tgt_ptr, val).raw() << endl;
      exit(0);
    }

  if (not unit.isSet() and not sample.isSet())
    {
      cout << "Flags -u and -s must be set" << endl;
      abort();
    }

  auto unit_ptr = Unit::search_by_symbol(unit.getValue());
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
