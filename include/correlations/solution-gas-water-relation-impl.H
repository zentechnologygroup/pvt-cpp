# ifndef SOLUTION_GAS_WATER_RELATION_IMPL_H
# define SOLUTION_GAS_WATER_RELATION_IMPL_H

# include <correlations/solution-gas-water-relation.H>
# include <correlations/water-density.H>
# include <correlations/spivey-dry.H>

inline Quantity<Sm3_Sm3>
RswSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<Molality_NaCl> & nacl_concentration) const
{
  const double m = nacl_concentration.raw();

  // Transformation from °C to °K
  const double tk = Quantity<Kelvin>(t).raw();

  // Vapor pressure of pure water, calculated from the IAWPS-95 formulation
  const double pv = PvSpiveyMN::get_instance().impl(tk).raw();

  if (p < pv)
    {
      ostringstream s;
      s << "RswSpiveyMN::impl: received pressure " << p
	<< " is less than computed vapor pressure " << pv << " "
	<< p.unit.symbol;
      throw domain_error(s.str());
    }

  // mCH4w: SOLUBILITY OF METHANE IN BRINE [gmol NaCl/kgH2O] 
  // AT THE TEMPERATURE AND PRESSURE OF EVALUATION
  const double mch4w = MwSpiveyMN::get_instance().impl(t, p, pv, m).raw();

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
