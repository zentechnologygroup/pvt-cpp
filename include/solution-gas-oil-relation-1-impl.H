
# include <solution-gas-oil-relation-1.H>

inline Quantity<SCF_STB> RsStanding::impl(const Quantity<Sgg> & yg,
					  const Quantity<psia> & pb,
					  const Quantity<psia> & p,
					  const Quantity<Api> & api,
					  const Quantity<Fahrenheit> & t,
					  const Quantity<SCF_STB> & rsb) const
{
  const double raw_p = p.get_value();
  if (raw_p >= pb)
    return rsb;

  const double raw_yg = yg.get_value();
  const double raw_api = api.get_value();
  const double raw_t = t.get_value();
   
  const double rs = raw_yg *
    pow((raw_p/18.2 + 1.4)*pow(10, 0.0125*raw_api - 0.00091*raw_t), 1.2048);

  return Quantity<SCF_STB>(rs);
}

inline Quantity<SCF_STB> RsTotalCFP::impl(const Quantity<Sgg> & yg,
					  const Quantity<psia> & pb,
					  const Quantity<psia> & p,
					  const Quantity<Api> & api,
					  const Quantity<Fahrenheit> & t,
					  const Quantity<SCF_STB> & rsb) const
{
  const double raw_p = p.get_value();
  if (raw_p >= pb)
    return rsb;

  double c1 = 0, c2 = 0, c3 = 0, c4 = 0;
  const double raw_api = api.get_value();
  if (raw_api <= 10)
    {
      c1 = 12.2651;
      c2 = 0.030405;
      c4 = 0.9669;
    }
  else if (raw_api <= 35)
    {
      c1 = 15.0057;
      c2 = 0.0152;
      c3 = 0.0004484;
      c4 = 1.095;
    }
  else // API <= 45:
    {
      c1 = 112.925;
      c2 = 0.0248;
      c3 = -0.001469;
      c4 = 1.129;
    }
        
  const double raw_yg = yg.get_value();
  const double raw_t = t.get_value();

  const double rs = raw_yg *
    pow((raw_p/c1) * pow(10, c2*raw_api - c3*raw_t), c4);

  return Quantity<SCF_STB>(rs);
    
}

inline Quantity<SCF_STB>
RsVasquezBeggs::impl(const Quantity<Sgg> & yg,
		     const Quantity<psia> & pb,
		     const Quantity<psia> & p,
		     const Quantity<Api> & api,
		     const Quantity<Fahrenheit> & t,
		     const Quantity<Fahrenheit> & tsep,
		     const Quantity<psia> & psep,
		     const Quantity<SCF_STB> & rsb) const
{
  const double raw_p = p.get_value();
  if (raw_p >= pb)
    return rsb;

  const double raw_api = api.get_value();
  double c1 = 0, c2 = 0, c3 = 0;
  if (raw_api <= 30)
    {
      c1 = 0.0362;
      c2 = 1.0937;
      c3 = 25.724;
    }
  else
    {
      c1 = 0.0178;
      c2 = 1.187;
      c3 = 23.931;
    }

  const double raw_yg = yg.get_value();
  const double ygs = raw_yg *
    (1 + 0.00005912*raw_api*tsep.get_value()*log10(psep/114.7));

  const double rs = c1 * ygs * pow(raw_p, c2) *
    exp((c3*raw_api) / (t.get_value() + 460));

  return Quantity<SCF_STB>(rs > 0 ? rs : 0);
}

