
# include <tclap/CmdLine.h>

# include <ah-comb.H>
# include <tpl_dynMapTree.H>

# include <metadata/pvt-analyse.H>
# include <metadata/pvt-calibrate.H>

using namespace std;
using namespace TCLAP;

CmdLine cmd = { "test-calibrate", ' ', "0" };

vector<string> properties =
  { "pb", "rs", "rsa", "bob", "boa", "bo", "uob", "uoa", "uo", "uod" };
ValuesConstraint<string> allowed = properties;
ValueArg<string> property =
  { "P", "property", "property", true, "", &allowed, cmd };

ValueArg<double> value = { "v", "value", "value for property", false, 0,
			   "value for the given property", cmd };
  
SwitchArg corr_list = { "l", "list", "list matching correlations", cmd };

SwitchArg corr_all = { "a", "all", "list all associated correlations", cmd };

SwitchArg corr_best = { "b", "best", "list best correlations", cmd };
  
SwitchArg set_val = { "s", "set-value", "puts given value in data set", cmd };

ValueArg<string> set_corr = { "S", "set-correlation", "set correlation",
			      false, "",
			      "set correlation for the given property", cmd };

SwitchArg plot = { "p", "plot", "generate plot data", cmd };

vector<string> compute_types = { "single", "calibrated", "both" };
ValuesConstraint<string> allowed_compute = compute_types;
ValueArg<string> compute_type = { "c", "compute-type", "compute type", false,
				 "single", &allowed_compute, cmd };

SwitchArg r = { "R", "R", "generate R script", cmd };

ValueArg<string> unit = { "u", "unit", "unit", false, "",
			  "unit for the given value", cmd };

vector<string> sort_values = { "r2", "sumsq", "mse", "distance" };
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

ValueArg<string> file =
  { "f", "file", "file name", false, "", "file name", cmd };

ValueArg<double> uod =
  { "", "uod", "uod value", false, 0, "uod value", cmd };

PvtAnalyzer load_pvt_data(istream & input)
{
  return PvtAnalyzer(input);
}

void list_correlations(const DynList<const Correlation*> & l)
{
  l.for_each([] (auto p) { cout << p->call_string() << endl; });
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

auto get_cmp(const string & sort_type)
  -> bool (*)(const PvtAnalyzer::Desc & d1, const PvtAnalyzer::Desc & d2)
{
  if (sort_type == "r2") return cmp_r2;
  if (sort_type == "sumsq") return cmp_sumsq;
  if (sort_type == "distance") return cmp_dist;
  if (sort_type == "mse") return cmp_mse;
  cout << "Invalid sort type " << sort_type << endl;
  abort();
}

DynList<double> extract_col(const PvtAnalyzer & pvt,
			    const string & set_name, const string col_name)
{
  return to_dynlist(pvt.get_data().values(set_name, col_name));
}

DynList<double> p_all(const PvtAnalyzer & pvt)
{
  
}

void error_msg(const string & msg = "Not yet implemented")
{
  cout << msg << endl;
  abort();
}

void list_correlations(const DynList<PvtAnalyzer::Desc> & l,
		       const string & sort_type)
{
  auto f = sort(l, get_cmp(sort_type)).maps<DynList<string>>([] (auto d)
    {
      return DynList<string>({PvtAnalyzer::correlation(d)->call_string(),
	    to_string(PvtAnalyzer::r2(d)), to_string(PvtAnalyzer::mse(d)),
	    to_string(PvtAnalyzer::sigma(d)), to_string(PvtAnalyzer::sumsq(d))});
    });
  f.insert(DynList<string>({"Correlation", "r2", "mse", "distance", "sumsq"}));
}

enum class EvalType { Single, Calibrated, Both };

EvalType get_eval_type(const string & type)
{
  if (type == "single") return EvalType::Single;
  if (type == "calibrated") return EvalType::Calibrated;
  if (type == "both") return EvalType::Both;
  error_msg("Invalid compute type " + type);
}

enum class OutputType { mat, csv, R, json };

OutputType get_output_type(const string & type)
{
  if (type == "mat") return OutputType::mat;
  if (type == "csv") return OutputType::csv;
  if (type == "R") return OutputType::R;
  if (type == "json") return OutputType::json;
  error_msg("Invalid output type " + type);
}
    
string tuned_name(const PvtAnalyzer::Desc & desc)
{
  ostringstream s;
  s << "tuned_" << PvtAnalyzer::c(desc) << "_"
    << PvtAnalyzer::m(desc) << "_"
    << PvtAnalyzer::correlation(desc)->name;
  return s.str();
}

// TODO: hacer la misma versión pero para DefinedCorrelation
pair<DynList<string>, DynList<DynList<double>>>
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

  auto pressure = pvt.get_data().values(set_name, "p");
  auto lab = pvt.get_data().values(set_name, col_name);

  DynList<DynList<double>> ret;
  for (auto it = get_zip_it(pressure, lab); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      ret.append(DynList<double>( { get<0>(t), get<1>(t) } ));
    }

  DynList<string> header = { "p", target_name };

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
	  header.append(PvtAnalyzer::correlation(desc)->name);
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
	  header.append(tuned_name(desc));
	}
      break;
    case EvalType::Both:
      for (auto itl = l.get_it(); itl.has_curr(); itl.next())
	{
	  const auto & desc = itl.get_curr();
	  auto corr_ptr = PvtAnalyzer::correlation(desc);
	  const auto & values = PvtAnalyzer::values(desc);
	  auto tuned_values = pvt.get_data().tuned_compute(set_name, corr_ptr,
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
	  header.append(PvtAnalyzer::correlation(desc)->name);
	  header.append(tuned_name(desc));
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
    {
      list_correlations(pvt.rs_correlations());
      exit(0);
    }

  if (corr_list.isSet())
    {
      list_correlations(pvt.rs_valid_correlations());
      exit(0);
    }

  if (corr_best.isSet())
    {
      double pb = pvt.get_pb();
      auto l = pvt.pb_best_correlations().maps<DynList<string>>([pb] (auto t)
        {
	  int per = round(get<2>(t)/get<1>(t) * 100);
	  return DynList<string>( { get<0>(t)->call_string(),
		to_string(get<1>(t)), to_string(get<2>(t)), to_string(per) });
	});
      l.insert({"Correlation", "Value",
	    "Error (pb = " + to_string(pvt.get_pb()) + ")", "%"});
      cout << to_string(format_string(l)) << endl;
      exit(0);
    }
}

string mat_format(const pair<DynList<string>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first);

  ostringstream s;
  s << to_string(format_string(mat));
  return s.str();
}

string csv_format(const pair<DynList<string>, DynList<DynList<double>>> & dmat)
{
  auto mat = dmat.second.maps<DynList<string>>([] (const auto & row)
    {
      return row.template maps<string>([] (auto v) { return to_string(v); });
    });
  mat.insert(dmat.first);

  ostringstream s;
  s << to_string(format_string_csv(mat));
  return s.str();
}

string r_format(const pair<DynList<string>, DynList<DynList<double>>> & dmat)
{
  ostringstream s;

  auto values = transpose(dmat.second);
  for (auto it = get_zip_it(dmat.first, values); it.has_curr(); it.next())
    {
      auto t = it.get_curr();
      s << Rvector(get<0>(t), get<1>(t)) << endl;
    }

  return s.str();
}

void process_rs(PvtAnalyzer & pvt)
{
  if (corr_all.isSet())
    {
      list_correlations(pvt.rs_correlations());
      exit(0);
    }

  if (corr_list.isSet())
    {
      list_correlations(pvt.rs_valid_correlations());
      exit(0);
    }

  if (corr_best.isSet())
    {
      list_correlations(pvt.rs_best_correlations(), sort_type.getValue());
      exit(0);
    }

  if (not corr_names.isSet())
    return;

  if (not plot.isSet())
    return;

  if (not corr_names.isSet())
    error_msg("Correlations list is empty");

  DynList<const Correlation*> corr_list;
  for (auto corr_name : corr_names.getValue())
    {
      auto p = Correlation::search_by_name(corr_name);
      if (p == nullptr)
	error_msg("Correlation name " + corr_name + " not found");
      if (p->target_name() != "rs")
	error_msg("Correlation name " + corr_name + " is not for rs property");
      if (not pvt.rs_valid_correlations().exists([&corr_name] (auto p)
					     { return p->name == corr_name; }))
	error_msg("Correlation name " + corr_name + " development range does "
		  "not fit the data associated to the given fluid");

      corr_list.append(p);
    }

  auto dmat = eval_correlations(pvt.correlations_stats(corr_list, 0),
				"Below Pb", "rs", pvt,
				get_eval_type(compute_type.getValue()));

  switch (get_output_type(output_type.getValue()))
    {
    case OutputType::mat:
      cout << mat_format(dmat);
      break;
    case OutputType::csv:
      cout << csv_format(dmat);
      break;
    case OutputType::R:
      cout << r_format(dmat);
      break;
    case OutputType::json:
    default:
      error_msg("Invalid outout type value");
    }

  // TODO< revisar bandera de formateo y actuar en consecuencia
}

void process_rsa(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_bob(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_boa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_bo(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uob(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uoa(PvtAnalyzer & pvt)
{
  error_msg();
}
void process_uo(PvtAnalyzer & pvt)
{
  error_msg();
}

void process_uod(PvtAnalyzer & pvt)
{
  error_msg();
}

using OptionPtr = void (*)(PvtAnalyzer&);

DynMapTree<string, OptionPtr> dispatch_tbl;

void init_dispatcher()
{
  dispatch_tbl.insert("pb", process_pb);
  dispatch_tbl.insert("rs", process_rs);
  dispatch_tbl.insert("rsa", process_rsa);
  dispatch_tbl.insert("bob", process_bob);
  dispatch_tbl.insert("boa", process_boa);
  dispatch_tbl.insert("bo", process_bo);
  dispatch_tbl.insert("uob", process_uob);
  dispatch_tbl.insert("uoa", process_uoa);
  dispatch_tbl.insert("uo", process_uo);
  dispatch_tbl.insert("uod", process_uod);
}

void dispatch_option(const string & op, PvtAnalyzer & pvt)
{
  auto command = dispatch_tbl.search(op);
  if (command == nullptr)
    error_msg("Option " + op + " not registered");
  (*command->second)(pvt);
}

int main(int argc, char *argv[])
{
  init_dispatcher();

  //cmd.xorAdd(corr_list, corr_best);
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

  dispatch_option(property.getValue(), pvt);

  return 0;
}
