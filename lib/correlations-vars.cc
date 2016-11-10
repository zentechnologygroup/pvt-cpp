
# include <ahSort.H>
# include <ah-stl-utils.H>
# include <correlations/pvt-correlations.H>

# include <json.hpp>

using namespace Aleph;
using json = nlohmann::json;
  
size_t Correlation::counter = 0;
DynMapTree<string, const Correlation *> Correlation::tbl;
Array<const Correlation *> Correlation::correlations_tbl;

CorrelationInstantiater __correlations; 


static json to_json(const CorrelationPar & p) 
{
  json j;
  j["name"] = p.name;
  j["description"] = p.description;
  j["unit"] = p.unit.symbol;
  j["physical quantity"] = p.unit.physical_quantity.name;
  j["minimum value"] = p.min_val.get_value();
  j["minimum value unit"] = p.min_val.unit.symbol;
  j["maximum value"] = p.max_val.get_value();
  j["maximum value unit"] = p.min_val.unit.symbol;
  j["min from author"] = p.min_from_author;
  j["max from author"] = p.max_from_author;
  return j;
}


static json to_json(const Correlation & c) 
{
  json j;
  j["Result maximum value"] = c.max_val;
  j["Result minimum value"] = c.min_val;
  j["Result unit"] = c.unit.symbol;
  j["refs"] = to_vector(c.refs.maps<string>([] (const auto & r)
    { return r->to_string(); }));
  j["notes"] = to_vector(c.notes);
  j["data bank"] = to_vector(c.db);
  j["title"] = c.title;
  j["author"] = c.author;
  j["subtype"] = c.subtype_name;
  j["type"] = c.type_name;
  j["name"] = c.name;
  j["hidden"] = c.hidden;
  j["id"] = c.id;

  auto jpars = c.get_preconditions().maps<json>([] (const auto & par)
					       { return ::to_json(par); });
  auto pars = to_vector(jpars);
  j["parameters"] = pars;
  j["number of parameters"] = pars.size();  

  return j;
}

string Correlation::to_json()
{
  DynMapTree<string, DynMapTree<string, DynList<const Correlation * const>>>
    tree;

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

  auto properties = to_vector(jl);
  json j;
  j["Physical properties"] = properties;
  j["total physical properties"] = properties.size();

  return j.dump(2);
}

long open_correlation_call(long id, char buf[], size_t sz)
{
  try
    {
      return (long) new CorrelationInvoker(id, buf, sz);
    }
  catch (...)
    {
      return 0;
    }
}

long push_correlation_call(long proxy_id, double val)
{
  try
    {
      CorrelationInvoker * ptr = (CorrelationInvoker*) proxy_id;
      ptr->push(val);
      return true;
    }
  catch (...)
    {
      return false;
    }
}

long CorrelationInvoker::call()
{
  bool status = true;
  json j;
  try
    {
      double result = correlation_ptr->compute(pars);
      j["status"] = "success";
      j["result"] = result;
      j["msg"] = "";
    }
  catch (exception & e)
    {
      j["status"] = "failure";
      j["result"] = 0;
      j["msg"] = e.what();
      status = false;
    }

  string str = j.dump();

  strncpy(json_buffer, str.data(), buf_sz);

  return status;
}

long exec_correlation_call(long proxy_id)
{
  CorrelationInvoker * ptr = (CorrelationInvoker*) proxy_id;
  return ptr->call();
}

long free_correlation_call(long proxy_id)
{
  delete (CorrelationInvoker*) proxy_id;
  return true;
}

