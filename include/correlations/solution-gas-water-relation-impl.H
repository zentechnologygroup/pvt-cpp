# ifndef SOLUTION_GAS_WATER_RELATION_IMPL_H
# define SOLUTION_GAS_WATER_RELATION_IMPL_H

# include <correlations/solution-gas-water-relation.H>
# include <correlations/water-density.H>

inline Quantity<Sm3_Sm3>
RswSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Molality_NaCl> & nacl_concentration) const
{
  // Transformation from °C to °K
  const double tk = Quantity<Kelvin>(t).raw();
        
  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  constexpr double tc = 647.096; // °K
  constexpr double pc = 22.064; // MPa

  const double v = 1 - (tk/tc);
  const double v2 = v*v;
  const double v3 = v2*v;
  const double v4 = v3*v;
        
  const double lnpv = ((tc/tk) * (-7.85951783*v + 1.84408259*pow(v, 1.5) +
				  -11.7866497*v3 + 22.6807411*pow(v, 3.5) +
				  -15.9618719*v4 + 1.80122502 * pow(v, 7.5))) +
    				log(pc);

  const double pv = exp(lnpv);
  if (p < pv)
    {
      ostringstream s;
      s << "RswSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapour pressure " << pv << " "
	<< p.unit.name;
      throw domain_error(s.str());
    }

  const double t100 = t.raw()/100;
  const double t100_2 = t100*t100;
  const double log_p_pv = log(p.raw() - pv);
  const double p2 = pow2(p);
  
  // Solubility of methane in pure water
  const double a = -0.007751*t100_2 + 0.013624*t100 + -0.0781;
  const double b = 0.01193*t100_2 + 0.0851*t100 + 1.02766;
  const double c = (1.8316*t100_2 + -7.8119*t100 + -3.6231) /
    (-0.10733*t100_2 + 1.09192*t100 + 1);
  const double m = nacl_concentration.raw();
  const double m2 = m*m;

  const double mch4pw = exp(a*log_p_pv*log_p_pv + b*log_p_pv + c);
            
  // Solubility of methane in brine
  const double c1 = 7.015e-2 + 1.074e-4*tk + 2.260e-1*p.raw()/tk + -1.227e-3*p2/tk;
  constexpr double c2 = -6.28e-3;

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] AT THE
  // TEMPERATURE AND PRESSURE OF EVALUATION 
  const double mch4w = mch4pw * exp(-2*c1*m - c2*m2);

  // VMCH4gstd: MOLAR VOLUME OF METHANE IN THE GAS PHASE AT STANDARD CONDITIONS [cm³/gmol] 
  // FROM THE REAL GAS LAW
  constexpr double R = 8.314467; // Universal gas constant [MPa
				 // cm³/gmol K] (McCain et al., 2011) 
  const Quantity<Celsius> Tstd = 15.555556; // °C (60 °F)  
  constexpr double Pstd = 0.101352; // MPa (14.7 psia)

  const double VMCH4gstd = R*Quantity<Kelvin>(Tstd).raw()/Pstd; // Zstd = 1.0
            
  // vgfwstd: SPECIFIC VOLUME OF METHANE-FREE BRINE
            
  // Density of methane-free brine [g/cm³] at standard temperature and pressure
  const double pgfwstd =
    PwSpiveyMNGasFree::get_instance().impl(Tstd, Pstd, m).raw(); 

  // Specific volume of methane-free brine [cm³/g]
  const double vgfwstd = 1.0/pgfwstd;

  // CALCULATION OF THE SOLUTION GAS-WATER RATIO [std cm³/ST cm³]
  // Molecular weight MNaCl: 58.4428 g/gmol
  const double rsw = (mch4w * VMCH4gstd)/((1000 + m * 58.4428) * vgfwstd);

  return Quantity<Sm3_Sm3>(rsw);
}


inline Quantity<SCF_STB>
RswCulbersonMcKetta::impl(const Quantity<Fahrenheit> & t,
			const Quantity<psia> & p,
			const Quantity<Dissolved_Salt_Percent> & nacl_concentration) const
{
  const double raw_t = t.raw();
  const double t2 = raw_t*raw_t;
  const double t3 = t2*raw_t;
  const double t4 = t3*raw_t;

  const double raw_p = p.raw();
  const double p2 = raw_p*raw_p;

  const double a = 8.15839 - 6.12265e-2*raw_t + 1.91663e-4*t2 - 2.1654e-7*t3;
  const double b = 1.01021e-2 - 7.44241e-5*raw_t + 3.05553e-7*t2 - 2.94883e-10*t3;
  const double c = (-9.02505 + 0.130237*raw_t - 8.53425e-4*t2 + 2.34122e-6*t3 - 2.37049e-9*t4) * 1e-7;
        
  // Solubility of gas in pure water
  const double rspw = a + b*raw_p + c*p2; 
   
  // Solubility of gas in brine  
  const double d =  -0.0840655*nacl_concentration.raw()*pow(raw_t, -0.285854);  
  const double rsw = rspw * pow(10, d); 
                        
  return Quantity<SCF_STB>(rsw);
}

inline Quantity<SCF_STB>
RswMcCoy::impl(const Quantity<Fahrenheit> & t,
			const Quantity<psia> & p,
			const Quantity<Dissolved_Salt_Percent> & nacl_concentration) const
{
  const double raw_t = t.raw();
  const double t2 = raw_t*raw_t;

  const double raw_p = p.raw();
  const double p2 = raw_p*raw_p;
        
  const double a = 2.12 + 3.45e-3*raw_t - 3.59e-5*t2;
  const double b = 0.0107 - 5.26e-5*raw_t + 1.48e-7*t2;
  const double c = -8.75e-7 + 3.9e-9*raw_t - 1.02e-11*t2;

  // Solubility of gas in pure water
  const double rspw = a + b*raw_p + c*p2; 
   
  // Solubility of gas in brine    
  const double rsw = rspw * (1 - (0.0753 - 1.73e-4*raw_t)*nacl_concentration.raw());
                        
  return Quantity<SCF_STB>(rsw);
}

# endif // SOLUTION_GAS_WATER_RELATION_IMPL_H
