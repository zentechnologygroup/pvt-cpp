
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
  j["Result maximum value"] = c.max_val;
  j["Result minimum value"] = c.min_val;
  j["Result unit"] = c.unit.symbol;
  j["refs"] = to_vector(c.refs.map<string>([] (const auto & r)
    { return r->to_string(); }));
  j["notes"] = to_vector(c.notes);
  j["data bank"] = to_vector(c.db);
  j["title"] = c.title;
  j["author"] = c.author;
  j["subtype"] = c.subtype_name;
  j["type"] = c.type_name;
  j["name"] = c.name;
  j["hidden"] = c.hidden;

  auto jpars = c.get_preconditions().map<json>([] (const auto & par)
					       { return ::to_json(par); });
  auto pars = to_vector(jpars);
  j["parameters"] = pars;
  j["number of parameters"] = pars.size();  

  return j;
}

string Correlation::to_json()
{
  DynMapTree<string, DynMapTree<string, DynList<const Correlation * const>>> tree;

  for (auto it = tbl.get_it(); it.has_curr(); it.next())
    {
      auto corr_ptr = it.get_curr().second;
      auto & subtree = tree[corr_ptr->type_name];
      auto & l = subtree[corr_ptr->subtype_name];
      l.append(corr_ptr);
    }

  DynList<json> jl;
  for (auto it = tree.get_it(); it.has_curr(); it.next())
    {
      auto p = it.get_curr();
      auto type_name = p.first;

      json j;
      j["Relation type"] = type_name;
      vector<json> properties;

      auto subtree = p.second;
      for (auto i = subtree.get_it(); i.has_curr(); i.next())
	{
	  auto p = i.get_curr();
	  auto l = p.second;

	  json physical_property;
	  physical_property["name"] = p.first;

	  vector<json> corrs;

	  for (auto it = l.get_it(); it.has_curr(); it.next())
	    {
	      auto corr_ptr = it.get_curr();
	      corrs.push_back(::to_json(*corr_ptr));
	    }

	  physical_property["number of relations"] = corrs.size();
	  physical_property["Relations"] = corrs;
	  properties.push_back(physical_property);
	}

      j["number of properties"] = properties.size();
      j["Physical property"] = properties;
      jl.append(j);
    }
	
  json j;
  j["Physical properties"] = to_vector(jl);

  return j.dump(2);
}

