
# include <ahSort.H>
# include <ah-stl-utils.H>
# include <pvt-correlations.H>

# include <json.hpp>

using namespace Aleph;
using json = nlohmann::json;
  

DynMapTree<string, const Correlation * const> Correlation::tbl;

CorrelationInstantiater __correlations; 


static json to_json(const CorrelationPar & p) 
{
  json j;
  j["name"] = p.name;
  j["unit"] = p.unit.symbol;
  j["minimum value"] = p.min_val.get_value();
  j["minimum value unit"] = p.min_val.unit.symbol;
  j["maximum value"] = p.max_val.get_value();
  j["maximum value unit"] = p.min_val.unit.symbol;
  return j;
}


static json to_json(const Correlation & c) 
{
  json j;
  j["type"] = c.type_name;
  j["subtype"] = c.subtype_name;
  j["name"] = c.name;
  j["author"] = c.author;
  j["title"] = c.title;
  j["data bank"] = to_vector(c.db);
  j["notes"] = to_vector(c.notes);
  j["refs"] = to_vector(c.refs.map<string>([] (const auto & r)
    { return r->to_string(); }));
  j["Result unit"] = c.unit.symbol;
  j["Result minumum value"] = c.min_val;
  j["Result maximum value"] = c.max_val;

  return j;
}

string Correlation::to_json()
{
  DynList<const Correlation* const> l =
    sort(list(), [] (const auto p1, const auto p2)
	 { return p1->type_name < p2->type_name; });

  l.for_each([] (auto ptr)
	     {
	       cout << ptr->subtype_name << ", " << ptr->type_name << ", "
		    << ptr->name << endl
		 ;//<< ptr->to_json() << endl;
	     });
	

  string ret;

  return ret;
}

