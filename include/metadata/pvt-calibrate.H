# ifndef PVT_CALIBRATE_H
# define PVT_CALIBRATE_H

# include "correlations/correlation.H"
# include "metadata/empirical-data.H"
# include "metadata/pvt-analyse.H"

struct PvtCalibrator
{
  struct CorrelationDesc
  {
    bool is_set = false;
    string name = "no-name";
    const Correlation * corr_ptr = nullptr;
    bool calibrated = true;
    double c = 0;
    double m = 1;

    json to_json() const
    {
      json j;
      j["is set"] = is_set;
      if (not is_set)
	return j;
      
      j["name"] = name;
      j["correlation"] = corr_ptr->name;
      j["calibrated"] = calibrated;
      j["c"] = c;
      j["m"] = m;
      return j;
    }

    CorrelationDesc() {}
    
    CorrelationDesc(const json & j)
    {
      is_set = j["is set"];
      if (not is_set)
	return;

      name = j["name"];
      corr_ptr = Correlation::search_by_name(j["correlation"]);
      calibrated = j["calibrated"];
      c = j["c"];
      m = j["m"];
    }
  };

  EmpiricalData data;
  CorrelationDesc pb_correlation;
  CorrelationDesc rs_correlation;
  CorrelationDesc bob_correlation;
  CorrelationDesc boa_correlation;
  CorrelationDesc uob_correlation;
  CorrelationDesc uoa_correlation;

  
  
};

# endif //PVT_CALIBRATE_H
