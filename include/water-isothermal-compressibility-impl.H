# ifndef WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
# define WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H

# include <water-isothermal-compressibility.H>


inline Quantity<mPa_1>
CwSpiveyMN::impl(const Quantity<Celsius> & t,
		 const Quantity<mPascal> & p,
		 const Quantity<mPascal> & pb,
		 const Quantity<Disolved_Salt_Percent> & s,
		 const Quantity<ZFactor> & z) const
{
  const double t100 = t.raw()/100;
  const double t100_2 = t100*100;

  const double epw = (4.221*t100_2 + -3.478*t100 + 6.221) /
    (0.5182*t100_2 + -0.4405*t100 + 1);

  const double fpw = (-11.403*t100_2 + 29.932*t100 + 27.952) /
    (0.20684*t100_2 + 0.3768*t100 + 1);

  const double s100 = s.raw()/100;

  // concentracion molal de NaCl [gmol/kgH2O] a partir de S
  const double m = (1000 * s100) / (58.4428 * (1 - s100));
        
  // Coeficientes de compresibilidad de la salmuera libre de gas
  const double e = 0.1249;
  const double f1 = (-0.617*t100_2 + -0.747*t100 + -0.4339) / (10.26*t100 + 1);
  const double f2 = (9.917*t100 + 5.1128) / (3.892*t100 + 1);
  const double f3 = 0.0365*t100_2 + -0.0369*t100;
        
  const double ew = epw + e*m;
  Fw = Fpw + (F1 * (m ** (3/2))) + (F2 * (m)) + (F3 * (m ** (1/2)))
        
        # Compresibilidad de la salmuera libre de gas
        Cgfw = (1/70.) * (1/((Ew * (P/70.)) + Fw))
        
        #TERMINO2 vgfw: VOLUMEN ESPECIFICO DE LA SALMUERA LIBRE DE GAS A CONDICIONES T Y P DE EVALUACION
        # Densidad del agua [g/cm³] pura a la presion de referencia 70Mpa y T de evaluacion
        ppwr = (((-0.127213) * t100_2) + ((0.645486) * t100) + (1.03265))/(((-0.070291) * t100_2) + ((0.639589) * t100) + 1)
        
        # Coeficientes de compresibilidad del agua pura 
        Epw = (((4.221) * t100_2) + ((-3.478) * t100) + (6.221))/(((0.5182) * t100_2) + ((-0.4405) * t100) + 1)
        Fpw = (((-11.403) * t100_2) + ((29.932) * t100) + (27.952))/(((0.20684) * t100_2) + ((0.3768) * t100) + 1)
        
        # Concentracion molal de NaCl [gmol/kgH2O] a partir de S
        m = (1000 * s100)/(58.4428 * (1 - s100))
        
        # Coeficientes para la densidad de la salmuera libre de gas a la presion de referencia 70Mpa
        D1 = (((-1.1149e-4) * t100_2) + ((1.7105e-4) * t100) + (-4.3766e-4))/(((0) * t100_2) + ((0) * t100) + 1)
        D2 = (((-8.878e-4) * t100_2) + ((-1.388e-4) * t100) + (-2.96318e-3))/(((0) * t100_2) + ((0.51103) * t100) + 1)
        D3 = (((2.1466e-3) * t100_2) + ((1.2427e-2) * t100) + (4.2648e-2))/(((-8.1009e-2) * t100_2) + ((0.525417) * t100) + 1)
        D4 = (((2.356e-4) * t100_2) + ((-3.636e-4) * t100) + (-2.278e-4))/(((0) * t100_2) + ((0) * t100) + 1)
        
        pgfwr = ppwr + (D1 * (m ** 2)) + (D2 * (m ** (3/2))) + (D3 * m) + (D4 * (m ** (1/2)))
        
        # Coeficientes de compresibilidad de la salmuera libre de gas
        E = (((0) * t100_2) + ((0) * t100) + (0.1249))/(((0) * t100_2) + ((0) * t100) + 1)
        F1 = (((-0.617) * t100_2) + ((-0.747) * t100) + (-0.4339))/(((0) * t100_2) + ((10.26) * t100) + 1)
        F2 = (((0) * t100_2) + ((9.917) * t100) + (5.1128))/(((0) * t100_2) + ((3.892) * t100) + 1)
        F3 = (((0.0365) * t100_2) + ((-0.0369) * t100) + (0))/(((0) * t100_2) + ((0) * t100) + 1)
        
        Ew = Epw + (E * m)
        Fw = Fpw + (F1 * (m ** (3/2))) + (F2 * (m)) + (F3 * (m ** (1/2)))
        
        Iwr = (1/Ew) * log(fabs(Ew + Fw))
        Iw = (1/Ew) * log(fabs((Ew * (P/70.))+ Fw))
        
        # Densidad de la salmuera libre de gas a las condiciones T y P de evaluacion
        pgfw = pgfwr * exp(Iw - Iwr)
        
        # Volumen especifico de la salmuera libre de gas
        vgfw = 1/pgfw
        
        
        # TERMINO3 VMCH4w: VOLUMEN PARCIAL MOLAR DE METANO EN LA SALMUERA A CONDICIONES P Y T DE EVALUACION
        # Derivadas con respecto a P de los siguientes terminos
        C3 = (7.698589e-2) + ((-5.0253331e-5) * (T + 273.15)) + ((-30.092013)/(T + 273.15)) + ((4.8468502e3)/((T + 273.15) ** 2))  
        C4 = (3.924e-4) + (2 * (-1.97e-6) * P)
        C5 = 0
            
        # Volumen molar parcial de metano en la salmuera
        VMCH4w = 8.314467 * (T + 273) * ((C3) + (2 * m * C4) + ((m ** 2) * C5))
        
        
        # TERMINO4 dvgfwdP: DERIVADA DEL VOLUMEN ESPECIFICO DE LA SALMUERA LIBRE DE GAS RESPECTO A LA PRESION
        dvgfwdP = -(vgfw) * Cgfw 
        
        # TERMINO5 dVMCH4wdP: DERIVADA DEL VOLUMEN MOLAR DE METANO DISUELTO EN SALMUERA RESPECTO A LA PRESION 
        dVMCH4wdP = 8.314467 * (T + 273.15) * ((0) + ((2 * m) * (2 * (-1.97e-6))) + ((m ** 2) * (0)))
        
        # TERMINO4 mCH4w: SOLUBILIDAD DEL METANO EN LA SALMUERA [gmol/KgH2O] A CONDICIONES P Y T DE EVALUACION
        # Concentracion molal de NaCl [gmol/kgH2O] a partir de S
        m = (1000 * s100)/(58.4428 * (1 - s100))
        
        # Presion de vapor del agua pura a partir de la formulacion IAWPS-95
        Tc = 647.096 # K
        Pc = 22.064 # MPa
        v = 1 - ((T + 273.15)/Tc)
        
        lnPv = ((Tc/(T + 273.15)) * (((-7.85951783) * v) + ((1.84408259) * (v ** 1.5)) + ((-11.7866497) * (v ** 3)) + ((22.6807411) * (v ** 3.5)) + ((-15.9618719) * (v ** 4)) + ((1.80122502) * (v ** 7.5)))) + log(Pc)
        Pv = exp(lnPv)
        
        # Solubilidad del metano en agua pura
        A = (((0) * t100_2) + ((-0.004462) * t100) + (-0.06763))/(((0) * t100_2) + ((0) * t100) + 1)
        B = (((-0.03602) * t100_2) + ((0.18917) * t100) + (0.97242))/(((0) * t100_2) + ((0) * t100) + 1)
        C = (((0.6855) * t100_2) + ((-3.1992) * t100) + (-3.7968))/(((0.07711) * t100_2) + ((0.2229) * t100) + 1)
         
        mathDomain = P - Pv 
        
        if P > Pb:# compresibilidad de salmuera subsaturada
        
            if mathDomain > 0: 
                
                mCH4pw = exp((A * ((log(P - Pv)) ** 2)) + (B * log (P - Pv)) + (C))
                
                # Solubilidad del metano en salmuera
                C1 = (-0.80898) + (1.0827e-3 * (T + 273.15)) + ((183.85)/(T + 273.15)) + ((3.924e-4) * P) + ((-1.97e-6) * (P ** 2))
                C2 = -3.89e-3
                
                mCH4w = mCH4pw * exp(((-2) * C1 * m) - (C2 * (m ** 2)))
                
                # TERMINO7 dmCH4wdP: DERIVADA DE LA SOLUBILIDAD DEL METANO CON RESPECTO A LA PRESION
                dmCH4wdP = mCH4w * ((((2 * A * log(P - Pv)) + B)/(P - Pv)) - ((2) * ((3.924e-4) + (2 * (-1.97e-6) * P)) * m))
                    
                if Z == None:                 
                    Cw = None
                else:
                    # TERMINO8 VMCH4g: VOLUMEN MOLAR [cm³/gmol] DE METANO EN LA FASE GASEOSA A CONDICIONES T y P DE EVALUACION
                    VMCH4g = 8.314467 * Z * (T + 273.15)/(P)                    
                    # pesos moleculares MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol                  
                    Cw = (((1000 + (m * 58.4428)) * dvgfwdP) + (mCH4w * dVMCH4wdP))/(((1000 + (m * 58.4428)) * vgfw) + (mCH4w * VMCH4w)) 
                    Cw=-Cw
                    Cw = Cw * 6.89475729*10**-3 # conversion de MPa^-1 a psi^-1
              
            else: 
               
                Cw = None
        
        else:  # compresibilidad de salmuera saturada 
        
            if mathDomain > 0: 
                
                mCH4pw = exp((A * ((log(P - Pv)) ** 2)) + (B * log (P - Pv)) + (C))
                
                # Solubilidad del metano en salmuera
                C1 = (-0.80898) + (1.0827e-3 * (T + 273.15)) + ((183.85)/(T + 273.15)) + ((3.924e-4) * P) + ((-1.97e-6) * (P ** 2))
                C2 = -3.89e-3
                
                mCH4w = mCH4pw * exp(((-2) * C1 * m) - (C2 * (m ** 2)))
                
                # TERMINO7 dmCH4wdP: DERIVADA DE LA SOLUBILIDAD DEL METANO CON RESPECTO A LA PRESION
                dmCH4wdP = mCH4w * ((((2 * A * log(P - Pv)) + B)/(P - Pv)) - ((2) * ((3.924e-4) + (2 * (-1.97e-6) * P)) * m))
                    
                
                if Z == None:
                    Cw = None
                    
                else:
                    # TERMINO8 VMCH4g: VOLUMEN MOLAR [cm³/gmol] DE METANO EN LA FASE GASEOSA A CONDICIONES T y P DE EVALUACION
                    VMCH4g = 8.314467 * Z * (T + 273.15)/(P)
                    
                    # pesos moleculares MNaCl: 58.4428 g/gmol; MCH4: 16.043 g/gmol 
                               
                    Cw = (((1000 + (m * 58.4428)) * dvgfwdP) + (mCH4w * dVMCH4wdP) + (dmCH4wdP * (VMCH4w - VMCH4g)))/(((1000 + (m * 58.4428)) * vgfw) + (mCH4w * VMCH4w))
                    Cw=-Cw                    
                    Cw = Cw * 6.89475729e-3 # conversion de MPa^-1 a psi^-1
            
            else:
                
                Cw = None
                   
}




 # endif // WATER_ISOTHERMAL_COMPRESSIBILITY_IMPL_H
