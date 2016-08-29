
# ifndef SOLUTION_GAS_OIL_RELATION_IMPL_H
# define SOLUTION_GAS_OIL_RELATION_IMPL_H

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


inline Quantity<SCF_STB> RsVelarde::impl(const Quantity<Sgg> & yg,
					 const Quantity<psig> & pb,
					 const Quantity<psig> & p,
					 const Quantity<Api> & api,
					 const Quantity<Fahrenheit> & t,
					 const Quantity<SCF_STB> & rsb) const
{
  const double raw_pb = pb.get_value();
  const double raw_p  = p.get_value();
  if (raw_p >= raw_pb)
    return rsb;

  constexpr double a0 = 9.73e-07;
  constexpr double a1 = 1.672608;
  constexpr double a2 = 0.92987;
  constexpr double a3 = 0.247235;
  constexpr double a4 = 1.056052;
        
  constexpr double b0 = 0.022339;
  constexpr double b1 = -1.00475;
  constexpr double b2 = 0.337711;
  constexpr double b3 = 0.132795;
  constexpr double b4 = 0.302065;
        
  constexpr double c0 = 0.725167;
  constexpr double c1 = -1.48548;
  constexpr double c2 = -0.164741;
  constexpr double c3 = -0.09133;
  constexpr double c4 = 0.047094;

  const double raw_yg = yg.get_value();
  const double raw_api = api.get_value();
  const double raw_t = t.get_value();

  const double s1 =
    a0 * pow(raw_yg, a1) * pow(raw_api, a2) * pow(raw_t, a3) * pow(raw_pb, a4);

  const double s2 =
    b0 * pow(raw_yg, b1) * pow(raw_api, b2) * pow(raw_t, b3) * pow(raw_pb, b4);

  const double s3 =
    c0 * pow(raw_yg, c1) * pow(raw_api, c2) * pow(raw_t, c3) * pow(raw_pb, c4);
        
  const double pr = raw_p / raw_pb;
        
  const double rsr = s1*pow(pr, s2) + (1 - s1)*pow(pr, s3);
        
  const double rs = rsr*rsb.get_value();

  return Quantity<SCF_STB>(rs);
}

inline Quantity<SCF_STB> RsCegarra::impl(const Quantity<Sgg> & yg,
					 const Quantity<psia> & pb,
					 const Quantity<psia> & p,
					 const Quantity<Api> & api,
					 const Quantity<Fahrenheit> & t,
					 const Quantity<SCF_STB> & rsb) const
{
  const double raw_pb = pb.get_value();
  const double raw_p  = p.get_value();
  if (raw_p >= raw_pb)
    return rsb;

  double l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
  const double raw_api = api.get_value();
  if (raw_api < 29.9)
    {
      l1 = 154.158;
      l2 = 0.4577;
      l3 = 0.0006680;
      l4 = 0.000514;
      l5 = 4.70257;
    }
  else 
    {
      l1 = 809.238;
      l2 = 0.32;
      l3 = 0.00061;
      l4 = 0.011;
      l5 = 1.1142;
    }

  const double raw_yg = yg.get_value();
  const double raw_t = t.get_value();

  const double rs =
    raw_yg * pow( (raw_p/l1 + l5) * pow(10, l4*raw_api - l3*raw_t), 1/l2);

  return Quantity<SCF_STB>(rs); 
}

inline Quantity<SCF_STB> RsPerezML::impl(const Quantity<Sgg> &,
					 const Quantity<psia> & pb,
					 const Quantity<psia> & p,
					 const Quantity<Api> &,
					 const Quantity<Fahrenheit> &,
					 const Quantity<SCF_STB> & rsb) const
{
  const double raw_pb = pb.get_value();
  const double raw_p  = p.get_value();
  if (raw_p >= raw_pb)
    return rsb;

  const double rs = (rsb.get_value()/1.031) * pow((raw_p/raw_pb), 0.83);

  return Quantity<SCF_STB>(rs);
}

inline Quantity<SCF_STB>
RsMillanArcia::impl(const Quantity<Sgg> &,
		    const Quantity<psia> & pb,
		    const Quantity<psia> & p,
		    const Quantity<Api> &,
		    const Quantity<Fahrenheit> &,
		    const Quantity<SCF_STB> & rsb) const
{
  const double raw_pb = pb.get_value();
  const double raw_p  = p.get_value();
  if (raw_p >= raw_pb)
    return rsb;

  const double rs = (rsb.get_value()/1.031) * pow(raw_p/raw_pb, 0.83);

  return Quantity<SCF_STB>(rs);
}

inline Quantity<SCF_STB>
RsManucciRosales::impl(const Quantity<Sgg> & yg,
		       const Quantity<psia> & pb,
		       const Quantity<psia> & p,
		       const Quantity<Api> & api,
		       const Quantity<Fahrenheit> & t,
		       const Quantity<SCF_STB> & rsb) const
{
  const double raw_pb = pb.get_value();
  const double raw_p  = p.get_value();
  if (raw_p >= raw_pb)
    return rsb;

  const double x = 0.000922*t.get_value() - 0.0072*api.get_value();
            
  const double rs = yg.get_value() * pow(raw_p/(84.88 * pow(10, x) ), 1/0.53);

  return Quantity<SCF_STB>(rs);
}

# endif // SOLUTION_GAS_OIL_RELATION_IMPL_H
