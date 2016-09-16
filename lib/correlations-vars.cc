
# include <json.hpp>
# include <ahSort.H>
# include <pvt-correlations.H>

using json = nlohmann::json;

DynMapTree<string, const Correlation * const> Correlation::tbl;

CorrelationInstantiater __correlations; 


string CorrelationPar::to_json() const
{
  json j;
  j["name"] = name;
  j["unit"] = unit.symbol;
  j["minimum value"] = min_val.get_value();
  j["minimum value unit"] = min_val.unit.symbol;
  j["maximum value"] = max_val.get_value();
  j["maximum value unit"] = min_val.unit.symbol;
  return j.dump();
}


string Correlation::to_json() const
{
  json j;
  j["Result unit"] = unit.symbol;
  j["Result minumum value"] = min_val;
  j["Result maximum value"] = max_val;

  return j.dump(4);
}

string Correlation::to_jsons()
{
  DynList<const Correlation* const> l =
    sort(list(), [] (const auto p1, const auto p2)
	 { return p1->type_name < p2->type_name; });

  l.for_each([] (auto ptr)
	     {
	       cout << ptr->subtype_name << ", " << ptr->type_name << ", "
		    << ptr->name << endl
		    << ptr->to_json() << endl;
	     });
	

  string ret;

  return ret;
}

