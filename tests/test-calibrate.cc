
# include <cstdlib>

# include <tclap/CmdLine.h>

# include <ah-comb.H>
# include <tpl_dynMapTree.H>
# include <ah-dispatcher.H>

# include <metadata/pvt-analyse.H>
# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;

CmdLine cmd = { "test-calibrate", ' ', "0" };

SwitchArg data = { "d", "data", "print data", cmd };

vector<string> properties =
  { "pb", "rs", "rsa", "bob", "boa", "bo", "uob", "uoa", "uo", "uod" };
ValuesConstraint<string> allowed = properties;
ValueArg<string> property =
  { "P", "property", "property", false, "", &allowed, cmd };

ValueArg<double> value = { "v", "value", "value for property", false, 0,
			   "value for the given property", cmd };
  
SwitchArg corr_list = { "l", "list", "list matching correlations", cmd };

SwitchArg corr_all = { "a", "all", "list all associated correlations", cmd };

SwitchArg corr_best = { "b", "best", "list best correlations", cmd };

SwitchArg force_corr =
  { "j", "join", "join by correlation value instead of experimental point",
    cmd };
  
ValueArg<string> below_corr = { "B", "below", "below correlation name", false,
				"", "set below correlation", cmd };

ValueArg<string> above_corr = { "A", "above", "above correlation name", false,
				"", "set above correlation", cmd };

SwitchArg plot = { "p", "plot", "generate plot data", cmd };

vector<string> compute_types = { "single", "calibrated", "both" };
ValuesConstraint<string> allowed_compute = compute_types;
ValueArg<string> compute_type = { "c", "compute-type", "compute type", false,
				 "single", &allowed_compute, cmd };

vector<string> sort_values = { "r2", "sumsq", "mse", "distance", "m", "c" };
ValuesConstraint<string> allow_sort = sort_values;
ValueArg<string> sort_type = { "o", "order", "output order type", false,
			       "sumsq", &allow_sort, cmd };

SwitchArg compute = { "C", "compute", "compute correlation output", cmd };

vector<string> out_type = { "csv", "mat", "json", "R" };
ValuesConstraint<string> allowed_out_type = out_type;
ValueArg<string> output_type = { "t", "output-type", "output type", false,
				 "mat", &allowed_out_type, cmd };

MultiArg<string> corr_names = { "n", "name", "add correlation name", false,
				"add correlation name to computations", cmd };

ValueArg<string> uod_name = { "u", "uod", "uod correlation name", false,
			      "", "uod correlation to be used for computations",
			      cmd };

ValueArg<string> file =
  { "f", "file", "file name", false, "", "file name", cmd };

PvtAnalyzer load_pvt_data(istream & input)
{
  return PvtAnalyzer(input);
}

void list_correlations(const DynList<const Correlation*> & l)
{
  l.for_each([] (auto p) { cout << p->call_string() << endl; });
  exit(0);
}

auto cmp_r2 = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & r2_1 = get<0>(s1);
  const auto & r2_2 = get<0>(s2);
  return r2_1 > r2_2;
};

auto cmp_mse = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & mse1 = get<1>(s1);
  const auto & mse2 = get<1>(s2);
  return mse1 < mse2;
};

auto cmp_dist = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const auto & sigma1 = get<2>(s1);
  const auto & sigma2 = get<2>(s2);
  return sigma1 < sigma2;
};

auto cmp_sumsq = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const CorrStat::LFit & f1 = get<3>(s1);
  const CorrStat::LFit & f2 = get<3>(s2);
  return f1.sumsq < f2.sumsq;
};

auto cmp_m = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const CorrStat::LFit & f1 = get<3>(s1);
  const CorrStat::LFit & f2 = get<3>(s2);
  return fabs(1 - f1.m) < fabs(1 - f2.m);
};

auto cmp_c = [] (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  const CorrStat::Desc & s1 = get<2>(d1);
  const CorrStat::Desc & s2 = get<2>(d2);
  const CorrStat::LFit & f1 = get<3>(s1);
  const CorrStat::LFit & f2 = get<3>(s2);
  return fabs(f1.c) < fabs(f2.c);
};

auto get_cmp(const string & sort_type) -> bool
  (*) (const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  if (sort_type == "r2") return cmp_r2;
  if (sort_type == "sumsq") return cmp_sumsq;
  if (sort_type == "distance") return cmp_dist;
  if (sort_type == "mse") return cmp_mse;
  if (sort_type == "m") return cmp_m;
  if (sort_type == "c") return cmp_c;
  cout << "Invalid sort type " << sort_type << endl;
  abort();
}

enum class OutputType { mat, csv, R, json, undefined };

OutputType get_output_type(const string & type)
{
  if (type == "mat") return OutputType::mat;
  if (type == "csv") return OutputType::csv;
  if (type == "R") return OutputType::R;
  if (type == "json") return OutputType::json;
  error_msg("Invalid output type " + type);
  return OutputType::undefined;
}

json best_corr_to_json(const DynList<string> & row)
{
  json ret;
  auto it = row.get_it();
  ret["correlation"] = it.get_curr(); it.next();
  ret["r2"] = atof(it.get_curr().c_str()); it.next();
  ret["mse"] = atof(it.get_curr().c_str()); it.next();
  ret["distance"] = atof(it.get_curr().c_str()); it.next();
  ret["sumsq"] = atof(it.get_curr().c_str()); it.next();
  ret["c"] = atof(it.get_curr().c_str()); it.next();
  ret["m"] = atof(it.get_curr().c_str());
  return ret;
}

string format_list(const DynList<DynList<string>> & mat, OutputType out_type)
{
  switch (out_type)
    {
    case OutputType::mat: return to_string(format_string(mat));
    case OutputType::csv:
      {
	auto m = mat.maps([] (const auto & l)
	{
	  DynList<string> ret;
	  ret.append("\"" + l.get_first() + "\"");
	  ret.append(l.drop(1));
	  return ret;
	});
	return to_string(format_string_csv(m));
      }
    case OutputType::json:
      {
	DynList<json> jsons =
	  mat.maps<json>([] (const auto & l) { return best_corr_to_json(l); });
	json j = to_vector(jsons);
	return j.dump(2);
      }
    default:
      error_msg("format type " + to_string((long) out_type) +
		" not supported for this combination");
    }
  error_msg("Fatal: format_list has reached an unexpected point");
  return "";
}

void list_correlations(const DynList<PvtAnalyzer::Desc> & l,
		       const string & sort_type, const string & out_type)
{
  auto mat = sort(l, get_cmp(sort_type)).maps<DynList<string>>([] (auto d)
    {
      return DynList<string>({PvtAnalyzer::correlation(d)->call_string(),
	    to_string(PvtAnalyzer::r2(d)), to_string(PvtAnalyzer::mse(d)),
	    to_string(PvtAnalyzer::sigma(d)), to_string(PvtAnalyzer::sumsq(d)),
	    to_string(PvtAnalyzer::c(d)), to_string(PvtAnalyzer::m(d))});
    });
  mat.insert(DynList<string>({"Correlation", "r2", "mse", "distance", "sumsq",
	  "c", "m"}));
  cout << format_list(mat, get_output_type(out_type));
  exit(0);
}

void list_correlations
(const DynList<pair<PvtAnalyzer::Desc, const Correlation*>> & l,
 const string & sort_type, const string & out_type)
{
  auto mat = sort(l, [sort_type] (auto p1, auto p2)
		  {
		    return (*get_cmp(sort_type))(p1.first, p2.first);
		  }).maps<DynList<string>>([] (auto p)
    {
      const auto & d = p.first;
      auto uod_corr = p.second;
      if (uod_corr == nullptr)
	return DynList<string>({PvtAnalyzer::correlation(d)->call_string(),
	      to_string(PvtAnalyzer::r2(d)), to_string(PvtAnalyzer::mse(d)),
	      to_string(PvtAnalyzer::sigma(d)),
	      to_string(PvtAnalyzer::sumsq(d)),
	      to_string(PvtAnalyzer::c(d)), to_string(PvtAnalyzer::m(d))});
      return DynList<string>({PvtAnalyzer::correlation(d)->call_string() +
	    "." + p.second->name,
	    to_string(PvtAnalyzer::r2(d)), to_string(PvtAnalyzer::mse(d)),
	    to_string(PvtAnalyzer::sigma(d)),
	    to_string(PvtAnalyzer::sumsq(d)),
	    to_string(PvtAnalyzer::c(d)), to_string(PvtAnalyzer::m(d))});
    });
  mat.insert(DynList<string>({"Correlation", "r2", "mse", "distance", "sumsq",
	  "c", "m"}));
  cout << format_list(mat, get_output_type(out_type));
  exit(0);
}

enum class EvalType { Single, Calibrated, Both, Undefined };

EvalType get_eval_type(const string & type)
{
  if (type == "single") return EvalType::Single;
  if (type == "calibrated") return EvalType::Calibrated;
  if (type == "both") return EvalType::Both;
  error_msg("Invalid compute type " + type);
  return EvalType::Undefined;
}

string tuned_name(const PvtAnalyzer::Desc & desc)
{
  ostringstream s;
  s << "tuned." << PvtAnalyzer::correlation(desc)->name;
  return s.str();
}

string tuned_name(const PvtAnalyzer::Desc & desc1,
		  const PvtAnalyzer::Desc & desc2)
{
  ostringstream s;
  s << "tuned." << PvtAnalyzer::correlation(desc1)->name << "."
    << PvtAnalyzer::correlation(desc2)->name;
  return s.str();
}

//                     name    tuned    c       m
using CorrDesc = tuple<string, bool, double, double>;

// TODO: hacer la misma versión pero para DefinedCorrelation

pair<DynList<CorrDesc>, DynList<DynList<double>>>
eval_correlations(const DynList<PvtAnalyzer::Desc> & l,
		  const string & set_name,
		  const string & col_name,
		  const PvtAnalyzer & pvt,
		  EvalType eval_type = EvalType::Single)
{
  if (l.is_empty())
    error_msg("Correlation list is empty");

  auto first = l.get_first();
  auto target_name = PvtAnalyzer::correlation(first)->target_name();

  auto pressure = pvt.get_data().values(set_name, "p").first;
  auto lab = pvt.get_data().values(set_name, col_name).first;

  DynList<DynList<double>> ret;
  for (auto it = get_zip_it(pressure, lab); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ret.append(DynList<double>( { get<0>(t), get<1>(t) } ));
    }

  DynList<CorrDesc> header = { make_tuple("p", false, 0, 0),
			       make_tuple(target_name, false, 0, 0) };

  switch (eval_type)
    {
    case EvalType::Single:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  const auto & values = PvtAnalyzer::values(desc);
	  auto it = values.get_it();
	  ret.mutable_for_each([&it] (auto & row)
            {
	      row.append(it.get_curr());
	      it.next();
	    });
	  header.append(make_tuple(PvtAnalyzer::correlation(desc)->name, false,
				   0, 0));
	}
	break;
    case EvalType::Calibrated:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  auto corr_ptr = PvtAnalyzer::correlation(desc);
	  auto values = pvt.get_data().tuned_compute(set_name, corr_ptr,
						     PvtAnalyzer::c(desc),
						     PvtAnalyzer::m(desc));
	  auto it = values.get_it();
	  ret.mutable_for_each([&it] (auto & row)
            {
	      row.append(it.get_curr());
	      it.next();
	    });
	  header.append(make_tuple(tuned_name(desc), true,
				   PvtAnalyzer::c(desc), PvtAnalyzer::m(desc)));
	}
      break;
    case EvalType::Both:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  auto corr_ptr = PvtAnalyzer::correlation(desc);
	  const auto & values = PvtAnalyzer::values(desc);
	  auto tuned_values =
	    pvt.get_data().tuned_compute(set_name, corr_ptr,
					 PvtAnalyzer::c(desc),
					 PvtAnalyzer::m(desc));
	  auto vit = values.get_it();
	  auto tit = tuned_values.get_it();
	  ret.mutable_for_each([&vit, &tit] (auto & row)
			       {
				 row.append(vit.get_curr());
				 row.append(tit.get_curr());
				 vit.next();
				 tit.next();
			       });
	  header.append(make_tuple(PvtAnalyzer::correlation(desc)->name,
				   false, 0, 0));
	  header.append(make_tuple(tuned_name(desc), true,
				   PvtAnalyzer::c(desc),
				   PvtAnalyzer::m(desc)));
	}
      break;
    default:
      error_msg("Invalid evaluation type");
    }

  return make_pair(header, ret);
}

//                     name    tuned    c below   m below c above  m above
using MixedCorrDesc = tuple<string, bool, double, double, double, double>;

CorrDesc to_CorrDesc(const MixedCorrDesc & desc)
{
  return make_tuple(get<0>(desc), get<1>(desc), get<2>(desc), get<3>(desc));
}

pair<DynList<MixedCorrDesc>, DynList<DynList<double>>>
eval_correlations(const DynList<PvtAnalyzer::Desc> & lb, // below pb
		  const DynList<PvtAnalyzer::Desc> & la, // above pb
		  size_t col_idx, // respect to below set
		  PvtAnalyzer & pvt,
		  EvalType eval_type = EvalType::Single)
{
  if (lb.is_empty())
    error_msg("Below pb correlation list is empty");

  if (la.is_empty())
    error_msg("Above pb correlation list is empty");

  auto first = lb.get_first();
  auto target_name = PvtAnalyzer::correlation(first)->target_name();

  const auto & data = pvt.get_data();
  const auto & above_set = data.var_sets(1);

  auto pressure = data.values(0, "p").first;
  auto lab = data.values(0, col_idx).first;
  const auto & n_above = above_set.samples.size();
  for (size_t i = 1; i < n_above; ++i) // omite 1ra fila
    {
      pressure.append(above_set.samples(i)(0));
      lab.append(above_set.samples(i)(col_idx));
    }

  DynList<DynList<double>> ret;
  for (auto it = get_zip_it(pressure, lab); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ret.append(DynList<double>( { get<0>(t), get<1>(t) } ));
    }

  DynList<MixedCorrDesc> header =
    { make_tuple("p", false, 0, 1, 0, 1),
      make_tuple(target_name, false, 0, 1, 0, 1) };

  switch (eval_type)
    {
    case EvalType::Single:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  const auto & below_values = PvtAnalyzer::values(below_desc);
	  const auto & above_values = PvtAnalyzer::values(above_desc).drop(1);

	  auto ret_it = ret.get_it();

	  for (auto it = below_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());
	      
	  for (auto it = above_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());

	  header.append(make_tuple(PvtAnalyzer::correlation(below_desc)->name
				   + "_" +
				   PvtAnalyzer::correlation(above_desc)->name,
				   false, 0, 1, 0 ,1));
	}
	break;
    case EvalType::Calibrated:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  auto below_corr = PvtAnalyzer::correlation(below_desc);
	  auto above_corr = PvtAnalyzer::correlation(above_desc);

	  auto below_values =
	    pvt.get_data().tuned_compute("Below Pb", below_corr,
					 PvtAnalyzer::c(below_desc),
					 PvtAnalyzer::m(below_desc));

	  auto above_values =
	    pvt.get_data().tuned_compute("Above Pb", above_corr,
					 PvtAnalyzer::c(above_desc),
					 PvtAnalyzer::m(above_desc)).drop(1);

	  auto ret_it = ret.get_it();

	  for (auto it = below_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());
	      
	  for (auto it = above_values.get_it(); it.has_curr();
	       it.next(), ret_it.next())
	    ret_it.get_curr().append(it.get_curr());

	  header.append(make_tuple(tuned_name(below_desc, above_desc), true,
				   PvtAnalyzer::c(below_desc),
				   PvtAnalyzer::m(below_desc),
				   PvtAnalyzer::c(above_desc),
				   PvtAnalyzer::m(above_desc)));
	}
      break;
    case EvalType::Both:
      for (auto itt = get_zip_it(lb, la); itt.has_curr(); itt.next())
	{
	  auto t = itt.get_curr();
	  const auto & below_desc = get<0>(t);
	  const auto & above_desc = get<1>(t);
	  auto below_corr = PvtAnalyzer::correlation(below_desc);
	  auto above_corr = PvtAnalyzer::correlation(above_desc);
	  const auto & below_values = PvtAnalyzer::values(below_desc);
	  const auto & above_values = PvtAnalyzer::values(above_desc).drop(1);
	  auto below_tuned_values =
	    pvt.get_data().tuned_compute(0, below_corr,
					 PvtAnalyzer::c(below_desc),
					 PvtAnalyzer::m(below_desc));
	  auto above_tuned_values =
	    pvt.get_data().tuned_compute(1, above_corr,
					 PvtAnalyzer::c(above_desc),
					 PvtAnalyzer::m(above_desc)).drop(1);

	  auto ret_it = ret.get_it();

	  for (auto it = get_zip_it(below_values, below_tuned_values);
	       it.has_curr(); it.next(), ret_it.next())
	    {
	      auto t = it.get_curr();
	      ret_it.get_curr().append(get<0>(t));
	      ret_it.get_curr().append(get<1>(t));
	    }
	      
	  for (auto it = get_zip_it(above_values, above_tuned_values);
	       it.has_curr(); it.next(), ret_it.next())
	    {
	      auto t = it.get_curr();
	      ret_it.get_curr().append(get<0>(t));
	      ret_it.get_curr().append(get<1>(t));
	    }	      

	  header.append(make_tuple(PvtAnalyzer::correlation(below_desc)->name
				   + "_" +
				   PvtAnalyzer::correlation(above_desc)->name,
				   false, 0, 0, 0 ,0));
	  header.append(make_tuple(tuned_name(below_desc, above_desc), true,
				   PvtAnalyzer::c(below_desc),
				   PvtAnalyzer::m(below_desc),
				   PvtAnalyzer::c(above_desc),
				   PvtAnalyzer::m(above_desc)));
	}
      break;
    default:
      error_msg("Invalid evaluation type");
    }

  return make_pair(header, ret);
}

void process_pb(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.pb_correlations());

  if (corr_list.isSet())
    list_correlations(pvt.pb_valid_correlations());

  if (not corr_best.isSet())
    error_msg("missing combined option -a, -l, or -b with -P pb option");

  double pb = pvt.get_pb();
  auto l = pvt.pb_best_correlations().maps<DynList<string>>([pb] (auto t)
        {
	  int per = round(get<2>(t)/get<1>(t) * 100);
	  return DynList<string>( { get<0>(t)->call_string(),
		to_string(get<1>(t)), to_string(-get<2>(t)), to_string(per) });
	});
  l.insert({"Correlation", "Value", "c", "%"});
  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << to_string(format_string(l)) << endl;
      break;
    case OutputType::csv:
      {
	auto m = l.maps([] (const auto & l)
	{
	  DynList<string> ret;
	  ret.append("\"" + l.get_first() + "\"");
	  ret.append(l.drop(1));
	  return ret;
	});
	cout << to_string(format_string_csv(m)) << endl;
      }
      break;
    case OutputType::json:
      {
	DynList<json> jsons = l.maps<json>([] (const auto & row)
          {
	    json j;
	    auto it = row.get_it();
	    j["correlation"] = it.get_curr(); it.next();
	    j["value"] = atof(it.get_curr().c_str()); it.next();
	    j["c"] = atof(it.get_curr().c_str()); it.next();
	    j["%"] = atof(it.get_curr().c_str());
	    return j;
	  });
	cout << json(to_vector(jsons)).dump(2) << endl;
      }
      break;
    default:
      error_msg("process_pb() option " + output_type.getValue() +
		" is not allowed with -P pb combined with -l, -a or -b");
      break;
    }
  exit(0);
}

DynList<DynList<string>>
string_mat(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first.maps<string>([] (auto t) { return get<0>(t); }));

  return mat;
}

DynList<DynList<string>>
string_mat(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first.maps<string>([] (auto t) { return get<0>(t); }));

  return mat;
}

string
mat_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;
  s << to_string(format_string(string_mat(dmat)));
  return s.str();
}

string
csv_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;
  s << to_string(format_string_csv(string_mat(dmat)));
  return s.str();
}

string
r_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;

  double ymin = numeric_limits<double>::max();
  double ymax = numeric_limits<double>::min();
  auto values = transpose(dmat.second);
  for (auto it = get_zip_it(dmat.first, values); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      const string & name = get<0>(get<0>(t));
      const DynList<double> & y = get<1>(t);
      s << Rvector(name, y) << endl;
      if (name != "p")
	{
	  ymin = min(ymin, y.fold(ymin, [] (const auto & a, const auto & v)
	    {
	      return min(a, v);
	    }));
	  ymax = max(ymax, y.fold(ymax, [] (const auto & a, const auto & v)
	    {
	      return max(a, v);
	    }));
	}
    }

  auto second_name = get<0>(dmat.first.nth(1));

  s << "plot(p, " << second_name << ",ylim=c(" << ymin << "," << ymax << "))"
  << endl;

  size_t col = 1;
  DynList<string> colnames;
  DynList<string> cols;
  for (auto it = dmat.first.get_it(2); it.has_curr(); it.next(), col++)
    {
      auto desc = it.get_curr();
      const auto & name = get<0>(desc);
      colnames.append("\"" + name + "\"");
      cols.append(to_string(col));
      s << "lines(p, " << name << ", col=" << col << ")" << endl;
    }

  s << Rvector("cnames", colnames) << endl
    << Rvector("cols", cols) << endl
    <<  "legend(\"topleft\", legend=cnames, lty=1, col=cols)" << endl;

  cout << s.str() << endl;
  return s.str();
}

json to_json(const pair<CorrDesc, DynList<double>> & sample)
{
  json j;
  j["name"] = get<0>(sample.first);
  j["calibrated"] = get<1>(sample.first);
  j["c"] = get<2>(sample.first);
  j["m"] = get<3>(sample.first);
  j["values"] = to_vector(sample.second);
  return j;
}

json to_json(const pair<MixedCorrDesc, DynList<double>> & sample)
{
  json j;
  j["name"] = get<0>(sample.first);
  j["calibrated"] = get<1>(sample.first);
  j["c below"] = get<2>(sample.first);
  j["m below"] = get<3>(sample.first);
  j["c above"] = get<4>(sample.first);
  j["m above"] = get<5>(sample.first);
  j["values"] = to_vector(sample.second);
  return j;
}

string
json_format(const pair<DynList<CorrDesc>, DynList<DynList<double>>> & dmat)
{
  DynList<pair<CorrDesc, DynList<double>>> samples;
  auto values = transpose(dmat.second);
  for (auto it = get_zip_it(dmat.first, values); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      samples.append(make_pair(get<0>(t), get<1>(t)));
    }

  json j;
  j["data sets"] = to_vector(samples.maps<json>([] (auto & s) { return to_json(s); }));
  
  return j.dump(2);
}

using FormatFct =
  string (*)(const pair<DynList<CorrDesc>, DynList<DynList<double>>>&);

AHDispatcher<string, FormatFct> format_dispatcher("csv", csv_format,
						  "mat", mat_format,
						  "json", json_format,
						  "R", r_format);

string mixed_mat_format
(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>> & dmat)
{
  return mat_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
                                 { return to_CorrDesc(desc); }),
			      dmat.second));
}

string mixed_csv_format(const pair<DynList<MixedCorrDesc>,
			DynList<DynList<double>>> & dmat)
{
  return csv_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
					          { return to_CorrDesc(desc); }),
			      dmat.second));
}

string mixed_r_format(const pair<DynList<MixedCorrDesc>,
		      DynList<DynList<double>>> & dmat)
{
  return r_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
				          { return to_CorrDesc(desc); }),
			    dmat.second));
}

string
mixed_json_format(const pair<DynList<MixedCorrDesc>,
		  DynList<DynList<double>>> & dmat)
{
  return json_format(make_pair(dmat.first.maps<CorrDesc>([] (const auto & desc)
					          { return to_CorrDesc(desc); }),
			      dmat.second));
}

using MixedFormatFct =
  string (*)(const pair<DynList<MixedCorrDesc>, DynList<DynList<double>>>&);

AHDispatcher<string, MixedFormatFct> mixed_dispatcher("csv", mixed_csv_format,
						      "mat", mixed_mat_format,
						      "json", mixed_json_format,
						      "R", mixed_r_format);

DynList<const Correlation*>
read_correlation_from_command_line(MultiArg<string> & cnames,
				   PvtAnalyzer & pvt,
				   const string & target_name,
				   const string & set_name)
{
  auto valid_correlations = pvt.valid_correlations(target_name, set_name);

  DynList<const Correlation*> corr_list;
  for (auto corr_name : cnames.getValue())
    {
      auto p = Correlation::search_by_name(corr_name);
      if (p == nullptr)
	error_msg("Correlation name " + corr_name + " not found");
      if (p->target_name() != target_name)
	error_msg("Correlation name " + corr_name + " is not for " +
		  target_name + " property");
      if (not valid_correlations.exists([&corr_name] (auto p)
					{ return p->name == corr_name; }))
	error_msg("Correlation name " + corr_name + " development range does "
		  "not fit the data associated to the given fluid");

      corr_list.append(p);
    }
  return corr_list;
}

DynList<const Correlation*>
read_uob_correlations_from_command_line(MultiArg<string> & cnames,
					PvtAnalyzer & pvt)
{
  auto valid_correlations = pvt.uob_correlations_lfits().
    maps<const Correlation*>([] (auto p) { return get<0>(p.first); });

  DynList<const Correlation*> corr_list;
  for (auto corr_name : cnames.getValue())
    {
      auto p = Correlation::search_by_name(corr_name);
      if (p == nullptr)
	error_msg("Correlation name " + corr_name + " not found");
      if (p->target_name() != "uob")
	error_msg("Correlation name " + corr_name + " is not for uob property");
      if (not valid_correlations.exists([&corr_name] (auto p)
					{ return p->name == corr_name; }))
	error_msg("Correlation name " + corr_name + " development range does "
		  "not fit the data associated to the given fluid");

      corr_list.append(p);
    }
  return corr_list;
}

void process_rs(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.rs_correlations());

  if (corr_list.isSet())
    list_correlations(pvt.rs_valid_correlations());

  if (corr_best.isSet())
    list_correlations(pvt.rs_best_correlations(), sort_type.getValue(),
		      output_type.getValue());

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(corr_names, pvt, "rs", "Below Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 0),
				"Below Pb", "rs", pvt,
				get_eval_type(compute_type.getValue()));

  cout << format_dispatcher.run(output_type.getValue(), dmat) << endl;
}

void process_rsa(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    error_msg("-" + corr_all.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_list.isSet())
    error_msg("-" + corr_list.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_best.isSet())
    error_msg("-" + corr_best.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> below_corr_list =
    read_correlation_from_command_line(corr_names, pvt, "rs", "Below Pb");

  auto above_corr_list = below_corr_list.maps<const Correlation*>([] (auto)
    {
      return &RsAbovePb::get_instance();
    });

  auto dmat = eval_correlations(pvt.correlations_stats(below_corr_list, 0),
				pvt.correlations_stats(above_corr_list, 1),
				pvt.get_data().name_index("Below Pb", "rs"), pvt,
				get_eval_type(compute_type.getValue()));

  mixed_dispatcher.run(output_type.getValue(), dmat);
}

void process_bob(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.bob_correlations());

  if (corr_list.isSet())
    list_correlations(pvt.bob_valid_correlations());

  if (corr_best.isSet())
    list_correlations(pvt.bob_best_correlations(), sort_type.getValue(),
		      output_type.getValue());

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(corr_names, pvt, "bob", "Below Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 0),
				"Below Pb", "bob", pvt,
				get_eval_type(compute_type.getValue()));

  cout << format_dispatcher.run(output_type.getValue(), dmat) << endl;
}

void process_boa(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.boa_correlations());

  if (corr_list.isSet())
    list_correlations(pvt.boa_valid_correlations());

  if (corr_best.isSet())
    list_correlations(pvt.boa_best_correlations(), sort_type.getValue(),
		      output_type.getValue());

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(corr_names, pvt, "boa", "Above Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 1),
				"Above Pb", "boa", pvt,
				get_eval_type(compute_type.getValue()));

  cout << format_dispatcher.run(output_type.getValue(), dmat) << endl;
}

void process_bo(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    error_msg("-" + corr_all.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_list.isSet())
    error_msg("-" + corr_list.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_best.isSet())
    error_msg("-" + corr_best.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (corr_names.isSet())
    error_msg("option -p is not allowed with -P bo");

  if (not below_corr.isSet())
    error_msg("Below correlation has not been set (-B option)");

  if (not above_corr.isSet())
    error_msg("Above correlation has not been set (-A option)");

  auto below_corr_ptr = Correlation::search_by_name(below_corr.getValue());
  if (below_corr_ptr == nullptr)
    error_msg("Below correlation " + below_corr.getValue() + " not found");
  if (below_corr_ptr->target_name() != "bob")
    error_msg("Below correlation " + below_corr.getValue() +
	      " is not for bob");
  {
    if (not pvt.valid_correlations("bob", "Below Pb").
	exists([below_corr_ptr] (auto p) { return p == below_corr_ptr; }))
      error_msg("Below correlation " + below_corr.getValue() +
		" is not inside the development ranges");
  }

  auto above_corr_ptr = Correlation::search_by_name(above_corr.getValue());
  if (above_corr_ptr == nullptr)
    error_msg("Above correlation " + above_corr.getValue() + " not found");
  if (above_corr_ptr->target_name() != "boa")
    error_msg("Above correlation " + above_corr.getValue() +
	      " is not for boa");
  {
    if (not pvt.valid_correlations("boa", "Above Pb").
	exists([above_corr_ptr] (auto p) { return p == above_corr_ptr; }))
      error_msg("Above correlation " + above_corr.getValue() +
		" is not inside the development ranges");
  }

  DynList<const Correlation*> below_corr_list = { below_corr_ptr };
  DynList<const Correlation*> above_corr_list = { above_corr_ptr };

  auto below_stats = pvt.correlations_stats(below_corr_list, 0);
  const auto & below_desc = below_stats.get_first();

  if (force_corr.getValue())
    {
      auto & data = pvt.get_data();
      auto row = data.var_sets(0).samples.size() - 1;
      const double bobp = data.tuned_compute(0, row, below_corr_ptr,
					     PvtAnalyzer::c(below_desc),
					     PvtAnalyzer::m(below_desc));
      data.var_sets(1).samples(0)(2) = bobp;
    }

  auto above_stats = pvt.correlations_stats(above_corr_list, 1);
	      
  auto dmat =
    eval_correlations(below_stats, above_stats, 
		      pvt.get_data().name_index("Below Pb", "bob"), pvt,
		      get_eval_type(compute_type.getValue()));

  mixed_dispatcher.run(output_type.getValue(), dmat);
}

void process_uob(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.uob_correlations());

  if (corr_list.isSet())
      list_correlations(pvt.uob_valid_correlations());

  if (corr_best.isSet())
    list_correlations(pvt.uob_correlations_lfits(), sort_type.getValue(),
		      output_type.getValue());

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  auto & pvtdata = pvt.get_data();

  const Correlation * uod_corr = nullptr;
  if (uod_name.isSet())
    {
      if (get<0>(pvtdata.search_const("uod")))
	error_msg("-u option (for uod) is not allowed if data "
		  "set already contains uod");
      uod_corr = Correlation::search_by_name(uod_name.getValue());
      if (uod_corr == nullptr)
	error_msg("Correlation " + uod_name.getValue() +  " not found");
      if (uod_corr->target_name() != "uod")
	error_msg("Correlation " + uod_name.getValue() +  " is not for uod");
      pvtdata.def_const("uod", pvtdata.compute(uod_corr), &uod_corr->unit);
    }

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(corr_names, pvt, "uob", "Below Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 0),
				"Below Pb", "uob", pvt,
				get_eval_type(compute_type.getValue()));

  if (uod_corr) // if uod was computed ==> delete from data set
    pvtdata.remove_last_const("uod");

  cout << format_dispatcher.run(output_type.getValue(), dmat) << endl;
}

void process_uoa(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    list_correlations(pvt.uoa_correlations());

  if (corr_list.isSet())
    list_correlations(pvt.uoa_valid_correlations());

  if (corr_best.isSet())
    list_correlations(pvt.uoa_best_correlations(), sort_type.getValue(),
		      output_type.getValue());

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (not corr_names.isSet())
    error_msg("specific correlations have not been defined (option -n)");

  DynList<const Correlation*> corr_list =
    read_correlation_from_command_line(corr_names, pvt, "uoa", "Above Pb");

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 1),
				"Above Pb", "uoa", pvt,
				get_eval_type(compute_type.getValue()));

  cout << format_dispatcher.run(output_type.getValue(), dmat) << endl;
}

void process_uo(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    error_msg("-" + corr_all.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_list.isSet())
    error_msg("-" + corr_list.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (corr_best.isSet())
    error_msg("-" + corr_best.getFlag() + " option is not allowed with -" +
	      property.getFlag() + " " + property.getValue() + " option");

  if (not plot.isSet())
    error_msg("Not plot option (-p) has not been set");

  if (corr_names.isSet())
    error_msg("option -p is not allowed with -P bo");

  if (not below_corr.isSet())
    error_msg("Below correlation has not been set (-B option)");

  if (not above_corr.isSet())
    error_msg("Above correlation has not been set (-A option)");

  auto & pvtdata = pvt.get_data();

  const Correlation * uod_corr = nullptr;
  if (uod_name.isSet())
    {
      if (get<0>(pvtdata.search_const("uod")))
	error_msg("-u option (for uod) is not allowed if data "
		  "set already contains uod");
      uod_corr = Correlation::search_by_name(uod_name.getValue());
      if (uod_corr == nullptr)
	error_msg("Correlation " + uod_name.getValue() +  " not found");
      if (uod_corr->target_name() != "uod")
	error_msg("Correlation " + uod_name.getValue() +  " is not for uod");
      pvtdata.def_const("uod", pvtdata.compute(uod_corr), &uod_corr->unit);
    }

  auto below_corr_ptr = Correlation::search_by_name(below_corr.getValue());
  if (below_corr_ptr == nullptr)
    error_msg("Below correlation " + below_corr.getValue() + " not found");
  if (below_corr_ptr->target_name() != "uob")
    error_msg("Below correlation " + below_corr.getValue() +
	      " is not for uob");
  {
    if (not pvt.valid_correlations("uob", "Below Pb").
	exists([below_corr_ptr] (auto p) { return p == below_corr_ptr; }))
      error_msg("Below correlation " + below_corr.getValue() +
		" is not inside the development ranges");
  }

  auto above_corr_ptr = Correlation::search_by_name(above_corr.getValue());
  if (above_corr_ptr == nullptr)
    error_msg("Above correlation " + above_corr.getValue() + " not found");
  if (above_corr_ptr->target_name() != "uoa")
    error_msg("Above correlation " + above_corr.getValue() +
	      " is not for uoa");
  {
    if (not pvt.valid_correlations("uoa", "Above Pb").
	exists([above_corr_ptr] (auto p) { return p == above_corr_ptr; }))
      error_msg("Above correlation " + above_corr.getValue() +
		" is not inside the development ranges");
  }

  DynList<const Correlation*> below_corr_list = { below_corr_ptr };
  DynList<const Correlation*> above_corr_list = { above_corr_ptr };

  auto below_stats = pvt.correlations_stats(below_corr_list, 0);
  const auto & below_desc = below_stats.get_first();

  if (force_corr.getValue())
    {
      auto & data = pvt.get_data();
      auto row = data.var_sets(0).samples.size() - 1;
      const double uobp = data.tuned_compute(0, row, below_corr_ptr,
					     PvtAnalyzer::c(below_desc),
					     PvtAnalyzer::m(below_desc));
      data.var_sets(1).samples(0)(3) = uobp;
    }

  auto dmat = eval_correlations(pvt.correlations_stats(below_corr_list, 0),
				pvt.correlations_stats(above_corr_list, 1),
				pvt.get_data().name_index("Below Pb", "uob"),
				pvt,
				get_eval_type(compute_type.getValue()));

  if (uod_corr) // if uod was computed ==> delete from data set
    pvtdata.remove_last_const("uod");

  mixed_dispatcher.run(output_type.getValue(), dmat);
}

AHDispatcher<string, void (*)(PvtAnalyzer&)> property_dispatcher("pb", process_pb,
								 "rs", process_rs,
								 "rsa", process_rsa,
								 "bob", process_bob,
								 "boa", process_boa,
								 "bo", process_bo,
								 "uob", process_uob,
								 "uoa", process_uoa,
								 "uo", process_uo);
int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);

  PvtAnalyzer pvt;
  if (file.isSet())
    {
      ifstream input(file.getValue());
      if (not input)
	error_msg("cannot open " + file.getValue() + " file");
      pvt = load_pvt_data(input);
    }
  else
    pvt = load_pvt_data(cin);

  if (data.getValue())
    {
      cout << pvt.get_data().to_string() << endl;
      exit(0);
    }

  if (not property.isSet())
    {
      cout << "Required argument missing: property" << endl;
      exit(0);
    }

  property_dispatcher.run(property.getValue(), pvt);

  return 0;
}
