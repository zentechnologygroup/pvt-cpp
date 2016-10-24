# -*- coding: utf-8 -*-
"""
Created on Mon Jan  9 11:17:51 2012

@author: albertovalderrama
"""
from scipy.optimize import leastsq
import numpy 

def tunedFuntion (xPressure, yLabData, yCorrelationValue ):
    
       
    def residuals(p, yLabData, yCorrelationValue ):
        res = yLabData - (p[0]* yCorrelationValue + p[1]) 
        return res   
     
    p0 = [1.00000, 0.000000]  # valores iniciales de iteracion   
    
    pleastsq = leastsq(residuals, p0, args=(yLabData, yCorrelationValue))
    parameters = pleastsq[0]

    return parameters 
    