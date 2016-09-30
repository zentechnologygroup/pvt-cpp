from math import exp, log10, floor, log, fabs

def PbAlMarhounCorrelation(Yg, Yo, Rsb, T):        
    Pb = (5.38088 * 10 ** -3) * (Rsb ** 0.715082) * (Yg ** -1.87784) * (Yo ** 3.1437) * (T ** 1.32657)
        
    if Pb < 0:
        Pb = 0
        
    PbAlMarhoun = Pb
        
    return PbAlMarhoun

def PbAlShammasiCorrelation(Yg, Yo, Rsb, T):
    c1 = 5.527215
    c2 = -1.841408
    c3 = 0.783716
        
    Pb = (Yo ** c1) * exp(c2 * floor(Yo * Yg)) * (Rsb * (T + 460) * Yg) ** c3 
        
    PbAlShammasi = Pb
        
    return PbAlShammasi

def PbDeGhettoCorrelation(Yg, Rsb, API, T):
    if API <= 10: # Extra-heavy oil
        Pb = (Rsb / Yg) ** (1 / 1.1128) * 10.7025 / (10 ** ((0.0169 * API) - (0.00156 * T)))
    else:
        Pb = 15.7286*(((Rsb/Yg)**0.7885)*((10**(0.0020*T))/(10**(0.0142*API))))
        
    PbDeGhetto = Pb
        
    return PbDeGhetto


def PbDindorukChristmanCorrelation(Yg, Rsb, API, T):
    a1 = 1.42828 * 10 ** -10
    a2 = 2.844591797
    a3 = -6.74896 * 10 ** -4
    a4 = 1.225226436
    a5 = 0.033383304
    a6 = - 0.272945957
    a7 = -0.084226069
    a8 = 1.869979257
    a9 = 1.221486524
    a10 = 1.370508349
    a11 = 0.011688308
        
    A = ((a1 * T**a2) + (a3 * API**a4)) / (a5 + (2 * Rsb**a6 / Yg**a7))**2
       
    Pb = a8 * ((Rsb**a9 * 10**A / Yg**a10) + a11)
            
    PbDindorukChristman = Pb
        
    return PbDindorukChristman

def PbDoklaOsmanCorrelation(Yg, Rsb, API, T):
    Yo = 141.5 / ( API + 131.5 ) # Leandro
       
    Pb = (0.836386 * 10 ** 4) * (Yg ** -1.01049) * (Yo ** 0.107991) * (T ** -0.952584) * (Rsb ** 0.724047)
        
    PbDoklayOsman = Pb
        
    return PbDoklayOsman

def PbGlasoCorrelation(Yg, Rsb, API, T, n2Concentration, co2Concentration, h2sConcentration):
    X = (Rsb / Yg) ** 0.816 * T ** 0.172 / API ** 0.989
    PbHC = 10 ** (1.7669 + (1.7447 * log10(X)) - (0.30218 * (log10(X)) ** 2))
    n2Effect = 1 + ((-2.65 * 10 **-4 * API + 5.5 * 10 **-3) * T + (0.0931 * API - 0.8295)) * n2Concentration + ((1.954 * 10 **-11 * API **4.699) * T + (0.027 * API - 2.366)) * n2Concentration **2
    co2Effect = 1 - 693.8 * co2Concentration * T **-1.553
    h2sEffect = 1 - (0.9035 + 0.0015 * API) * h2sConcentration + 0.019 * (45 - API) *  h2sConcentration **2
    Pb = PbHC * n2Effect * co2Effect * h2sEffect
    if Pb < 0:
        Pb = 0
    PbGlaso = Pb
    return PbGlaso

def PbHanafyCorrelation(Rsb):
    # Total flash gas-oil ratio
    Rsft = 69 + 1.071 * Rsb
        
    # Initial differential gas-oil ratio
    Rsi = 23.94 + 1.101 * Rsft
        
    Pb = (3.205 * Rsi) + 157.27
        
    PbHanafy = Pb
        
    return PbHanafy

def PbKartoatmodjoSchmidtCorrelation(Yg, Rsb, API, T, Tsep, Psep):
    C = 1 + 0.1595 * (API ** 0.4078) * (Tsep ** -0.2466) * log10(Psep / 114.7)
    Ygcorr = C * Yg
    if API > 30:
        C1 = 0.0315
        C2 = 0.7587
        C3 = 11.289
        C4 = 0.9143
    else:
        C1 = 0.05958
        C2 = 0.7972
        C3 = 13.1405
        C4 = 0.9986
    Pb = (Rsb / (C1 * (Ygcorr ** C2) * (10 ** (C3 * API / (T + 460) )))) ** C4
    if Pb < 0:
        Pb = 0
    PbKartoatmodjoSchmidt = Pb
    return PbKartoatmodjoSchmidt

def PbLasaterCorrelation(Yg, Rsb, API, T):
    Yo = 141.5 / (API + 131.5)
    # Effective molecular weight
    if API <= 40:
        Mo = 630 - (10 * API)
    else:
        Mo = 73110 * (API ** -1.562) 
            
    # Mol fraction of gas
    Ygfactor = (Rsb / 379.3) / ((Rsb / 379.3) + (350 * Yo / Mo))
        
    # Bubble point pressure factor
    if Ygfactor <= 0.6:
        Pbfactor = (0.679 * exp (2.786 * Ygfactor)) - 0.323 
    else:
        Pbfactor = (8.26 * (Ygfactor ** 3.56)) + 1.95
            
    Pb = ((Pbfactor) * (T + 459.6)) / Yg
        
    PbLasater = Pb
        
    return PbLasater

def PbPetroskyFarshadCorrelation(Yg, Rsb, API, T):
    X = 4.561 * 10 ** -5 * T ** 1.3911 - (7.916 * 10 ** -4 * API ** 1.541)
        
    Pb = 112.727 * (((Rsb ** 0.5774 / Yg ** 0.8439) * (10 ** X)) - 12.34)
        
    PbPetroskyFarshad = Pb
        
    return PbPetroskyFarshad

def PbStandingCorrelation(Yg, Rsb, API, T):
    Pb = 18.2 * (((Rsb / Yg) ** 0.83 * (10 ** ((0.00091 * T) - (0.0125 * API)))) - 1.4)
    return Pb

def PbTotalCFPCorrelation(Yg, Rsb, API, T):
    if API <= 10:
        c1 = 12.847
        c2 = 0.9636
        c3 = 0.000993
        c4 = 0.03417
    
    if 10 < API <= 35:
        c1 = 25.2755
        c2 = 0.7617
        c3 = 0.000835
        c4 = 0.011292
        
    if 35 < API <= 45:
        c1 = 216.4711
        c2 = 0.6922
        c3 = -0.000427
        c4 = 0.02314
            
    Pb = c1 * ((Rsb / Yg) ** c2) * (10 ** ((c3 * T) - (c4 * API)))
        
    PbTotalCFP = Pb
        
    return PbTotalCFP


def PbVasquezBeggsCorrelation(Yg, Rsb, API, T, Tsep, Psep):
    if API <= 30:
        c1 = 0.0362
        c2 = 1.0937
        c3 = 25.724
    else:
        c1 = 0.0178
        c2 = 1.187
        c3 = 23.931
        
    # Gas gravity that would result from separator conditions of 100 psig (approximately 114.7 psia)
    Ygs = Yg*(1.+5.912*10**-5*(API)*(Tsep)*log10(Psep/114.7))
    
    Pb = (Rsb / (c1 * Ygs * exp((c3 * API) / (T + 460)))) ** (1 / c2)
            
    PbVasquezBegg = Pb
    
    return PbVasquezBegg

def PbVelardeCorrelation(Yg, Rsb, API, T):
    x = (0.013098 * T ** 0.282372) - (8.2 * 10 ** -6 * API ** 2.176124)
    Pb = 1091.47 * ((Rsb ** 0.081465 * Yg ** -0.161488 * 10 ** x) - 0.740152) ** 5.354891
    PbVelarde = Pb
    return PbVelarde

def PbCegarraCorrelation(Yg, Rsb, API, T):
    if API < 29.9:
        l1 = 154.158
        l2 = 0.4577
        l3 = 0.0006680
        l4 = 0.000514
        l5 = 4.70257
    elif API >= 29.9:
        l1 = 809.238
        l2 = 0.32
        l3 = 0.00061
        l4 = 0.011 
        l5 = 1.1142
            
    Pb = l1 * ((((Rsb / Yg) ** l2) * (10 ** ((l3 * T) - (l4 * API)))) - l5)
        
    PbCegarra = Pb
        
    return PbCegarra

def PbPerezMLCorrelation(Yg, Rsb, API, T):
    X = (0.0002573 * T) - (0.0253643 * API)
    Pb = 10.667657 * ((Rsb / Yg) ** 1.00139101) * (10 ** X)
    PbPerezML = Pb
    return PbPerezML

def PbMillanArciaCorrelation(Yg, Rsb, API, T):
    X = (0.00091 * T) - (0.0125 * API)
    Pb = 25.3302 * ((((Rsb / Yg) ** 0.8303) * (10 ** X)) ** 0.9433)
    PbMillanArcia = Pb
    return PbMillanArcia

def PbManucciRosalesCorrelation(Yg, Rsb, API, T):
    X = (0.000922 * T) - (0.0072 * API)
    Pb = 84.88 * (((Rsb / Yg) ** 0.53) * (10 ** X))
    PbManucciRosales = Pb
    return PbManucciRosales

def RsAlShammasiCorrelation(Yg, Pb, P, Yo, T, Rsb):
    # Transformation from °API to oil specific gravity
    #Yo = 141.5 / ( API + 131.5 ) # conversion Leandro
    print Yg
    print Pb
    print P
    print Yo
    print T
    print Rsb
    c1 = 5.527215
    c2 = -1.841408
    c3 = 0.783716
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        Rs = ((P / (Yo ** c1 * exp(c2 * floor(Yo * Yg)))) ** (1 / c3)) / ((T + 460) * Yg)
    RsAlShammasi = Rs
    return RsAlShammasi


def RsAlMarhounCorrelation(Yg, Pb, P, Yo, T, Rsb):
    c1 = 5.38088 * 10 ** -3
    c2 = -1.87784
    c3 = 3.1437
    c4 = 1.32657
    c5 = 1 / 0.715082
        
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        Rs = (P / (c1 * Yg ** c2 * Yo ** c3 * T ** c4)) ** c5
    RsAlMarhoun = Rs
    return RsAlMarhoun

def RsDeGhettoCorrelation(Yg, Pb, P, API, T, Tsep, Psep, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        if API <= 10: # Extra-heavy oil
            Rs = Yg * ((P / 10.7025) * 10 **(0.0169 * API - 0.00156 * T)) ** 1.1128
        else:
            YgCorr = Yg * (1 + 0.5912 * API * Tsep * log10(Psep / 114.7) * 10 ** -4)
            Rs = ((YgCorr * P ** 1.2057) / 56.434) * 10 ** (10.9267 * API / (T + 460))
    RsDeGhetto = Rs
    return RsDeGhetto
        

def RsDindorukChristmanCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:    
        a1 = 4.86996 * 10 ** -6
        a2 = 5.730982539
        a3 = 9.9251 * 10 ** -3
        a4 = 1.776179364
        a5 = 44.2500268
        a6 = 2.702889206
        a7 = 0.744335673
        a8 = 3.35975497
        a9 = 28.10133245
        a10 = 1.57905016
        a11 = 0.928131344
        A = (a1 * API ** a2 + a3 * T ** a4) / (a5 + (2 * API ** a6) / (Pb ** a7)) ** 2
        Rs = ((P / a8 + a9) * Yg ** a10 * 10 ** A) ** a11
    RsDindorukChristman = Rs
    return  RsDindorukChristman

def RsDoklaOsmanCorrelation(Yg, Pb, P, Yo, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else: 
        Rs = ((1/0.836386 * 10 **4) * P * Yg ** 1.01049 * Yo ** -0.107991 * T ** 0.952584) ** (1/0.724047)
    RsDoklaOsman = Rs
    return RsDoklaOsman       

def RsGlasoCorrelation (Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        a = -0.30218
        b = 1.7447
        c = 1.7669 - log10(P)
        if (b ** 2 - 4 * a * c) < 0: # To avoid calculating the root of a negative number
            R = -b / (2 * a)
        else:
            R = (-b + (b ** 2 - 4 * a * c) ** 0.5) / (2 * a)
        F = 10 ** R
        Rs = Yg*((F * API ** 0.989) / (T ** 0.172)) ** (1 / 0.816)
        RsGlaso = Rs
        return RsGlaso


def RsKartoatmodjoSchmidtCorrelation(Yg, Pb, P, API, T, Tsep, Psep, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        c = 1 + 0.1595 * API ** 0.4078 * Tsep ** -0.2466 * log10(Psep / 114.7)
        YgCorr = c * Yg 
        if API > 30:
            c1 = 0.0315
            c2 = 0.7587
            c3 = 1.0937
            c4 = 11.289
        else:
            c1 = 0.05958
            c2 = 0.7972
            c3 = 1.0014
            c4 = 13.1405
    Rs = c1 * (YgCorr) **c2 * (P) ** c3 * (10) ** (c4 * API / (T + 460) )
    RsKartoatmodjoSchmidt = Rs
    return RsKartoatmodjoSchmidt
    
def RsLasaterCorrelation(Yg, Pb, P, Yo, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        API = 141.5 / Yo - 131.5
        if API <= 40:
            Mo = 630 - 10 * API
        else:
            Mo = 73110 * API ** -1.562
        # Calculation of Xpb = Bubble point pressure factor (P * Yg) / T
        Xpb = P * Yg / T
        if Xpb < 3.29:
            c1 = 0.359
            c2 = ((1.473 * P * Yg) / T)
            c3 = 0.476
            Sy = c1 * log(c2 + c3)
        else:
            c1 = ((0.121 * P * Yg) / T)
            c2 = -0.236
            c3 = 0.281
            Sy = (c1 + c2) ** c3
                
        if Sy>=1:
            Sy=0.99999999
        Rs = (132755 * Yo * Sy) / (Mo * (1 - Sy))    
    
        if Rs < 0: # Logical condition            
            Rs = 0
    
        RsLasater = Rs
    
        return RsLasater


def RsPetroskyFarshadCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        X = 7.916 * 10 ** -4 * API ** 1.541 - 4.561 * 10 ** -5 * T ** 1.3911
        Rs = ((P / 112.727 + 12.34) * Yg ** 0.8439 * 10 ** X) ** 1.73184
    RsPetroskyFarshad = Rs
    return RsPetroskyFarshad


def RsStandingCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        Rs = Yg * (((P / 18.2) + 1.4) * 10 ** (0.0125 * API - 0.00091 * T)) ** 1.2048  
    if Rs < 0: # Logical condition
        Rs = 0
    RsStanding = Rs
    return RsStanding

def RsTotalCFPCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        if API <= 10:
            c1 = 12.2651
            c2 = 0.030405
            c3 = 0
            c4 = 0.9669
            
        if 10 < API <= 35:
            c1 = 15.0057
            c2 = 0.0152
            c3 = 4.484 * 10 ** -4
            c4 = 1.095
        
        if 35 < API <= 45:
            c1 = 112.925
            c2 = 0.0248
            c3 = -1.469 * 10 ** -3
            c4 = 1.129
        
        Rs = Yg * ((P / c1) * (10) ** ((c2 * API) - (c3 * T))) ** c4
        
    if Rs < 0: # Logical condition
        Rs = 0 
    RsTotalCFP = Rs
    return RsTotalCFP

def RsVasquezBeggsCorrelation(Yg, Pb, P, API, T, Tsep, Psep, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        if API <= 30:
            c1 = 0.0362
            c2 = 1.0937
            c3 = 25.724
        else:
            c1 = 0.0178
            c2 = 1.187
            c3 = 23.931
        Ygs= Yg*(1.+5.912*10**-5*(API)*(Tsep)*log10(Psep/114.7))
        Rs = c1 * Ygs * P ** c2 * exp((c3 * API) / (T + 460))
    
    if Rs < 0:
        Rs = 0 
    
    RsVasquezBeggs = Rs
    
    return RsVasquezBeggs


def RsVelardeCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        a0 = 9.73 * 10 ** -7
        a1 = 1.672608
        a2 = 0.92987
        a3 = 0.247235
        a4 = 1.056052
        
        b0 = 0.022339
        b1 = -1.00475
        b2 = 0.337711
        b3 = 0.132795
        b4 = 0.302065
        
        c0 = 0.725167
        c1 = -1.48548
        c2 = -0.164741
        c3 = -0.09133
        c4 = 0.047094
        
        S1 = a0 * Yg ** a1 * API ** a2 * T ** a3 * Pb ** a4
        S2 = b0 * Yg ** b1 * API ** b2 * T ** b3 * Pb ** b4
        S3 = c0 * Yg ** c1 * API ** c2 * T ** c3 * Pb ** c4
        
        Pr = 1.0*P / Pb
        Rsr = S1 * Pr ** S2 + (1 - S1) * Pr ** S3
        Rs = Rsr * Rsb    
    
    if Rs < 0: # Logical condition            
        Rs = 0
    
    RsVelarde = Rs
    
    return RsVelarde

def RsCegarraCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        if API < 29.9:
            l1 = 154.158
            l2 = 0.4577
            l3 = 0.0006680
            l4 = 0.000514
            l5 = 4.70257
        elif API >= 29.9:
            l1 = 809.238
            l2 = 0.32
            l3 = 0.00061
            l4 = 0.011 
            l5 = 1.1142
        Rs = Yg * ((((P/l1) + l5) * (10 ** ((l4 * API) - (l3 * T)))) ** (1/l2))
    if Rs < 0: # Logical condition
        Rs = 0 
    RsCegarra = Rs
    return RsCegarra

def RsPerezMLCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        Rs = Rsb * ((1.0*P/Pb) ** 0.881)
    if Rs < 0: # Logical condition
        Rs = 0 
    RsPerezML = Rs
    return RsPerezML

def RsMillanArciaCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        Rs = (Rsb/1.031) * ((1.0*P/Pb) ** 0.83)
    if Rs < 0: # Logical condition
        Rs = 0 
    RsMillanArcia = Rs
    return RsMillanArcia
        

def RsManucciRosalesCorrelation(Yg, Pb, P, API, T, Rsb):
    if P >= Pb: # Logical condition
        Rs = Rsb
    else:
        X = (0.000922 * T) - (0.0072 * API)
        Rs = Yg * ((P/(84.88 * (10 ** X))) ** (1/0.53))
        if Rs < 0: # Logical condition
            Rs = 0 
    RsManucciRosales = Rs
    return RsManucciRosales

def BoAlmarhounCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        F = Rs ** 0.74239 * Yg ** 0.323294 * Yo ** -1.20204
        Bo = 0.497069 + 0.862963 * 10 ** -3 * T + 0.182594 * 10 ** -2 * F + 0.318099 * 10 ** -5 * F ** 2 
    else: # Undersaturated oil
        Fb = Rsb ** 0.74239 * Yg ** 0.323294 * Yo ** -1.20204
        Bob = 0.497069 + 0.862963 * 10 ** -3 * T + 0.182594 * 10 ** -2 * Fb + 0.318099 * 10 ** -5 * Fb ** 2
        Bo = Bob * exp(Co * (Pb - P))
        if Bo < 1:
            Bo = 1            
        
    BoAlmarhoun = Bo
    return BoAlmarhoun 


def BoAlShammasiCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 1 + (5.53 * 10 ** -7) * (Rs * (T - 60)) + 0.000181 * (Rs / Yo) + 0.000449 * ((T - 60) / Yo) + 0.000206 * (Rs * Yg / Yo)
    else: # Undersaturated oil
        Bob = 1 + (5.53 * 10 ** -7) * (Rsb * (T - 60)) + 0.000181 * (Rsb / Yo) + 0.000449 * ((T - 60) / Yo) + 0.000206 * (Rsb * Yg / Yo)
        Bo = Bob * exp(Co * (Pb - P))
    if Bo < 1:
        Bo = 1
    BoAlShammasi = Bo
    return BoAlShammasi 
    
def BoAlShammasiCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 1 + (5.53 * 10 ** -7) * (Rs * (T - 60)) + 0.000181 * (Rs / Yo) + 0.000449 * ((T - 60) / Yo) + 0.000206 * (Rs * Yg / Yo)
    else: # Undersaturated oil
        Bob = 1 + (5.53 * 10 ** -7) * (Rsb * (T - 60)) + 0.000181 * (Rsb / Yo) + 0.000449 * ((T - 60) / Yo) + 0.000206 * (Rsb * Yg / Yo)
        Bo = Bob * exp(Co * (Pb - P))
            
    if Bo < 1:
        Bo = 1
           
    BoAlShammasi = Bo
    return BoAlShammasi 

def BoDeGhettoCorrelation(Yg, API, Rs, Rsb, T, Tsep, P, Pb, Psep, Co):
    c1 = 4.677 * 10 ** -4
    c2 = 1.751 * 10 ** -5
    c3 = -1.811 * 10 ** -8
        
    Ygs = Yg * (1. + 5.912 * (10**-5) * (API) * (Tsep) * log10(Psep/114.7))
                         
    if P < Pb: # Saturated oil
        Bo = 1 + c1 * Rs + c2 * (T - 60) * (API / Ygs) + c3 * Rs * (T - 60) * (API / Ygs)
    else: # Undersaturated oil
        Bob = 1 + c1 * Rsb + c2 * (T - 60) * (API / Ygs) + c3 * Rsb * (T - 60) * (API / Ygs)
            
        Bo = Bob * exp(Co * (Pb - P))
                    
    if Bo < 1:
        Bo = 1
    BoDeGhetto = Bo
    return BoDeGhetto


def BoDindorukChristmanCorrelation(Yg, API, Rs, Rsb, T, Tsep, P, Pb, Co):
    a1 = 2.510755 * 10 ** 0
    a2 = -4.852538 * 10 ** 0
    a3 = 1.1835 * 10 ** 1
    a4 = 1.365428 * 10 ** 5
    a5 = 2.25288 * 10 ** 0
    a6 = 1.00719 * 10 ** 1
    a7 = 4.450849 * 10 ** -1
    a8 = 5.352624 * 10 ** 0
    a9 = -6.309052 * 10 ** -1
    a10 = 9.000749 * 10 ** -1
    a11 = 9.871766 * 10 ** -1
    a12 = 7.865146 * 10 ** -4
    a13 = 2.689173 * 10 ** -6
    a14 = 1.100001 * 10 ** -5
    
    b1 = 4.236114474
    b2 = 24.316998249
    b3 = 0.958319868
    b4 = 0.924700438
    b5 = 1.287177430
    b6 = 1.353868836
    b7 = 12.581487761
    b8 = 9.828286832
    
    Yo = 141.5 / (131.5 + API)
    
    if P < Pb: # Saturated oil
        A = (((Rs ** a1 * Yg ** a2) / Yo ** a3) + a4 * (T - 60) ** a5 + a6 * Rs) ** a7 / (a8 + (2 * Rs ** a9 / Yg ** a10) * (T - 60)) ** 2
        Bo = a11 + a12 * A + a13 * A ** 2 + a14 * (T - 60) * API / Yg
        if Bo > 2:
            BoDL = Bo
            n = (T - Tsep) ** b1 * (log10(BoDL) * tanh(BoDL)) ** b2 + b3 * (BoDL - 1) ** b4
            d = (1 + BoDL ** b5 * (T - Tsep) ** b6 * (log10(BoDL)) ** b7 ) ** b8
            Bo = 1 + (n / d)
    else: # Undersaturated oil
        Ab = (((Rsb ** a1 * Yg ** a2) / Yo ** a3) + a4 * (T - 60) ** a5 + a6 * Rsb) ** a7 / (a8 + (2 * Rsb ** a9 / Yg ** a10) * (T - 60)) ** 2
        Bob = a11 + a12 * Ab + a13 * Ab ** 2 + a14 * (T - 60) * API / Yg
        if Bob > 2:
            BoDL = Bob
            n = (T - Tsep) ** b1 * (log10(BoDL) * tanh(BoDL)) ** b2 + b3 * (BoDL - 1) ** b4
            d = (1 + BoDL ** b5 * (T - Tsep) ** b6 * (log10(BoDL)) ** b7 ) ** b8
            Bob = 1 + (n / d)
        Bo = Bob * exp(Co * (Pb - P))
    if Bo < 1:
        Bo = 1
    BoDindorukChristman = Bo
    return BoDindorukChristman
    
def BoDoklaOsmanCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        M = Rs ** 0.773572 * Yg ** 0.40402 * Yo ** -0.882605
        Bo = 0.431935 * 10 ** -1 + 0.156667 * 10 ** -2 * T + 0.139775 * 10 ** -2 * M + 0.380525 * 10 ** -5 * M ** 2
    else: # Undersaturated oil
        Mb = Rsb ** 0.773572 * Yg ** 0.40402 * Yo ** -0.882605
        Bob = 0.431935 * 10 ** -1 + 0.156667 * 10 ** -2 * T + 0.139775 * 10 ** -2 * Mb + 0.380525 * 10 ** -5 * Mb ** 2
        Bo = Bob * exp(Co * (Pb - P))
    BoDoklaOsman = Bo
    return BoDoklaOsman 

def BoGlasoCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        F = Rs * (Yg / Yo) ** 0.526 + 0.968 * T
        Bo = 1 + 10 ** (-6.58511 + 2.91329 * log10(F) - 0.27683 * (log10(F)) ** 2)
    else: # Undersaturated oil
        Fb = Rsb * (Yg / Yo) ** 0.526 + 0.968 * T
        Bob = 1 + 10 ** (-6.58511 + 2.91329 * log10(Fb) - 0.27683 * (log10(Fb)) ** 2)
    Bo = Bob * exp(Co * (Pb - P))
    BoGlaso = Bo
        
    return BoGlaso

def BoHanafyCorrelation(Rs, Rsb, P, Pb, Co):
    if P < Pb: # Saturated oil  
        Bo = 0.0006 * Rs + 1.079
    else: # Undersaturated oil
        Bob = 0.0006 * Rsb + 1.079
        Bo = Bob * exp(Co * (Pb - P))
    BoHanafy = Bo
    return BoHanafy

def BoKartoatmodjoCorrelation(Yg, Yo, Rs, Rsb, T, Tsep, P, Pb, Psep, Co):
    API = 141.5 / Yo - 131.5
    c = 1 + 0.1595 * (API ** 0.4078) * (Tsep ** -0.2466) * log10(Psep / 114.7)
    Ygcorr = c * Yg
    if P < Pb: # Saturated oil
        F = (Rs ** 0.755) * (Ygcorr ** 0.25) * (Yo ** -1.5) + 0.45 * T
        Bo = 0.98496 + 0.0001 * F ** 1.5
    else: # Undersaturated oil
        Fb = (Rsb ** 0.755) * (Ygcorr ** 0.25) * (Yo ** -1.5) + 0.45 * T
        Bob = 0.98496 + 0.0001 * Fb ** 1.5
        Bo = Bob * exp(Co * (Pb - P))
        
    BoKartoatmodjoSchmidt = Bo
    return BoKartoatmodjoSchmidt

def BoLasaterCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 0.972 + 0.000147 * (Rs * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
    else: # Undersaturated oil
        Bob = 0.972 + 0.000147 * (Rsb * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
        Bo = Bob * exp(Co * (Pb - P))
    BoLasater = Bo
    return BoLasater

def BoPetroskyFarshadCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        F = (Rs ** 0.3738 * ((Yg ** 0.2914) / (Yo ** 0.6265)) + 0.24626 * T ** 0.5371)
        Bo = 1.0113 + 7.2046 * 10 ** -5 * F ** 3.0936
    else:  # Undersaturated oil
        Fb = (Rsb ** 0.3738 * ((Yg ** 0.2914) / (Yo ** 0.6265)) + 0.24626 * T ** 0.5371)
        Bob = 1.0113 + 7.2046 * 10 ** -5 * Fb ** 3.0936
        Bo = Bob * exp(Co * (Pb - P))
    BoPetroskyFarshad = Bo
    return BoPetroskyFarshad

def BoStandingCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 0.972 + 0.000147 * (Rs * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
    else: # Undersaturated oil
        Bob = 0.972 + 0.000147 * (Rsb * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
        Bo = Bob * exp(Co * (Pb - P))
    BoStanding = Bo
    return BoStanding

def BoTotalCFPCorrelation(Yg, API, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 1.022 + 4.857 * 10 ** -4 * Rs - 2.009 * 10 ** -6 * (T - 60) * (API / Yg) + 17.569 * 10 ** -9 * Rs * (T - 60) * (API / Yg)
    else: # Undersaturated oil
        Bob = 1.022 + 4.857 * 10 ** -4 * Rsb - 2.009 * 10 ** -6 * (T - 60) * (API / Yg) + 17.569 * 10 ** -9 * Rsb * (T - 60) * (API / Yg)
        Bo = Bob * exp(Co * (Pb - P))
    BoTotalCFP = Bo
    return BoTotalCFP

def BoVasquezBeggsCorrelation(Yg, API, Rs, Rsb, T, Tsep, P, Pb, Psep, Co):
    if API <= 30:
        c1 = 4.677 * 10 ** -4
        c2 = 1.751 * 10 ** -5
        c3 = -1.811 * 10 ** -8
    else:
        c1 = 4.67 * 10 ** -4
        c2 = 1.1 * 10 ** -5
        c3 = 1.337 * 10 ** -9
    Ygs = Yg * (1. + 5.912 * (10 ** -5) * (API) * (Tsep) * log10(Psep/114.7))
    if P < Pb: # Saturated oil
        Bo = 1 + c1 * Rs + c2 * (T - 60) * (API / Ygs) + c3 * Rs * (T - 60) * (API / Ygs)
    else: # Undersaturated oil
        Bob = 1 + c1 * Rsb + c2 * (T - 60) * (API / Ygs) + c3 * Rsb * (T - 60) * (API / Ygs)
        Bo = Bob * exp(Co * (Pb - P))
    BoVasquezBeggs = Bo
    return BoVasquezBeggs

def BoCegarraCorrelation(Yg, Yo, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        Bo = 0.972 + 0.000147 * (Rs * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
    else: # Undersaturated oil
        Bob = 0.972 + 0.000147 * (Rsb * (Yg / Yo) ** 0.5 + 1.25 * T) ** 1.175 
        Bo = Bob * exp(Co * (Pb - P))
    BoCegarra = Bo
    return BoCegarra


def BoPerezMLCorrelation(Yg, Yo, Rsb, T, P, Pb, Co):
    C = (10 ** (-4)) * (((Rsb) * ((Yg/Yo) ** 0.5)) + (1.25 * T))
    Bob = 0.974322 + (4.35048 * C) - (2.85869 * (C ** 2))
    if P < Pb: # Saturated oil
        Bo = Bob * ((1) - ((8.801 + (0.1547 * T)) * (1e-3) * (1 - (1.0*P/Pb))))
    else: # Undersaturated oil
        Bo = Bob * exp(Co * (Pb - P))
    BoPerezML = Bo
    return BoPerezML

def BoVelardeMcCainCorrelation(Yg, API, Rs, Rsb, T, P, Pb, Co):
    pwater = 62.4 # The specific weight of water is 62.4 lb/ft³ or lb/scf
    psto = pwater * Yo
    A = 0 
    B = 52.8 - 0.01 * Rsb 
    while (fabs(A - B) > 0.00001):
        A = B
        ppo = A
        pa = -49.893 + 85.0149 * Yg - 3.70373 * Yg * ppo + 0.047981 * Yg * ppo ** 2 + 2.98914 * ppo- 0.035688 * ppo ** 2
        ppof = (Rs * Yg + 4600 * Yo) / (73.71 + Rs * Yg / pa)
        B = ppof     
        
    ppo = ppof
        
    if P < Pb: # Saturated oil
        pbs = ppo + (0.167 + 16.181 * (10 ** (-0.0425 * ppo))) * (P / 1000) - 0.01 * (0.299 + 263 * (10 ** (-0.0603 * ppo))) * (P / 1000) ** 2
        poR = pbs - (0.00302 + 1.505 * pbs ** -0.951) * (T - 60) ** 0.938 + (0.0233 * (10 ** (-0.0161 * pbs))) * (T - 60) ** 0.475
        Bo = (psto + 0.01357 * Rs * Yg) / (poR)
            
    else: # Undersaturated oil
        ppob = ppo
        pbsb = ppob + (0.167 + 16.181 * (10 ** (-0.0425 * ppob))) * (Pb / 1000) - 0.01 * (0.299 + 263 * (10 ** (-0.0603 * ppob))) * (Pb / 1000) ** 2
        poRb = pbsb - (0.00302 + 1.505 * pbsb ** -0.951) * (T - 60) ** 0.938 + (0.0233 * (10 ** (-0.0161 * pbsb))) * (T - 60) ** 0.475
        Bob = (psto + 0.01357 * Rsb * Yg) / (poRb)
        Bo = Bob * exp(Co * (Pb - P))
    BoVelarde = Bo
    return BoVelarde

def BoMillanArciaCorrelation(Yg, API, Rs, Rsb, T, P, Pb, Co):
    Bob = 1.3537 * (Rsb ** 0.1505) * (Pb ** (-0.1239)) * exp(-0.00405 * API)
    if P < Pb: # Saturated oil
        Bo = Bob * (0.9419 + (0.0608 * (1.0*P/Pb)))
    else: # Undersaturated oil
        Bo = Bob * exp(Co * (Pb - P))
    BoMillanArcia = Bo
    return BoMillanArcia

def BoManucciRosalesCorrelation(API, Rs, Rsb, T, P, Pb, Co):
    if P < Pb: # Saturated oil
        X = (3.27 * 1e-4 * T) + (0.00321 * API)
        Bo = 0.2378 * (Rs ** 0.221) * (10 ** X)
    else: # Undersaturated oil
        X = (3.27 * 1e-4 * T) + (0.00321 * API)
        Bob = 0.2378 * (Rsb ** 0.221) * (10 ** X)
        Bo = Bob * exp(Co * (Pb - P))
    BoManucciRosales = Bo
    return BoManucciRosales

def CoMcCainEtAlCorrelation (API, Rsb, T, P, Pb):
    Co = exp(-7.573 - 1.450 * log(P) - 0.383 * log(Pb) + 1.402 * log(T) + 0.256 * log(API) + 0.449 * log(Rsb))
    return Co


def CoDeGhettoCorrelation(Yg, API, Rsb, T, Tsep, P, Pb, Psep):
    c = 1 + 0.5912 * API * Tsep * log10(Psep / 114.7) * 10 ** -4
    YgCorr = c * Yg # Gas specific gravity correction (considering a separator pressure of 114.7 psia)
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil 
        if API <= 10:  # Extra-heavy oil  
            Co = (-889.6 + 3.1374 * Rsb + 20 * T - 627.3 * YgCorr - 81.4476 * API) / (P * 10 ** 5)
        else:
            Co = (-2841.8 + 2.9646 * Rsb + 25.5439 * T - 1230.5 * YgCorr + 41.91 * API) / (P * 10 ** 5)
    CoDeGhetto = Co
    return CoDeGhetto
        
def CoHanafyCorrelation(API, Rsb, T, P, Pb):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil    
        Bob = 0.0006 * Rsb + 1.079 # Bubble point oil volume factor
        pob = (2.366 - (1.358/Bob))**-1 # Bubble point oil density
        Co = exp((2.582/pob)-0.990) * 10 **-6
    CoHanafy = Co     
    return CoHanafy

def CoKartoatmodjoSchmidtCorrelation(Yg, API, Rsb, T, Tsep, P, Pb, Psep):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil          
        c = 1 + 0.1595 * (API ** 0.4078) * (Tsep ** -0.2466) * log10(Psep / 114.7)
        YgCorr = c * Yg # Gas specific gravity correction (considering the standardized separator pressure: Psep=100 psig)
        Co = 6.8257 * (10 ** -6) * (Rsb ** 0.5002) * P * (T ** 0.76606) * (YgCorr ** 0.35505)
    CoKartoatmodjoSchmidt = Co
    return CoKartoatmodjoSchmidt

def CoPetroskyFarshadCorrelation(Yg, API, Rsb, T, P, Pb):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil   
        Co = (1.705 * 10 ** -7) * (Rsb ** 0.69357) * (Yg ** 0.1885) * (API ** 0.3272) * (T ** 0.6729) * (P ** -0.5906)
    CoPetroskyFarshad = Co
    return CoPetroskyFarshad

def CoVasquezBeggsCorrelation(Yg, API, Rsb, T, Tsep, P, Pb, Psep):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil    
        Ygs = Yg * (1. + 5.912 * (10 ** -5) * (API) * (Tsep) * log10(Psep/114.7))
        Co = (-1433 + 5 * Rsb + 17.2 * T - 1180 * Ygs + 12.61 * API)/ (P * 10 ** 5)
    CoVasquezBeggs = Co
    return CoVasquezBeggs

def CoPerezMLCorrelation(Yg, API, Rsb, T, P, Pb):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil
        Cob = 2.856 * (1e-7) * (Rsb ** 0.69357) * (Yg ** 0.1885) * (API ** 0.3272) * (T ** 0.6729) * (Pb ** (-0.5906))   
        Co = Cob * ((P/Pb) ** 0.5)
    CoPerezML = Co
    return CoPerezML

def CoMillanArciaCorrelation(API, Rsb, T, P, Pb):
    if P < Pb: # Saturated oil - McCain et al. correlation
        Co = CoMcCainEtAlCorrelation(API, Rsb, T, P, Pb)
    else: # Undersaturated oil
        Cob = 2.075883 * (1e-6) * (API ** 0.5307) * (T ** -0.1143)  * exp((2.0523 * 1e-4 * Pb) + (4.0568 * 1e-4 * Rsb))   
        Co = 0.8557 * Cob * exp((-0.00143) * (P/Pb))
    CoMillanArcia = Co
    return CoMillanArcia

def PoBradleyCorrelation (Yg, Rs, Bo, Yo, P, Pb, Co):
    if P < Pb:
        po = (350*Yo + 0.0764*Yg*Rs)/(5.615*Bo)
    else:
        Bob = Bo/exp(Co * (Pb - P))
        pob = (350*Yo + 0.0764*Yg*Rs)/(5.615*Bob)
        po = pob * exp(Co * (P - Pb))
    return po

def UodBealCorrelation (API, T):
    a = 10 ** (0.43 + (8.33 / API))
    b = 0.32 + ((1.8 * 10 ** 7) / (API ** 4.53))
    c = 360.0 / (T + 200.0)
    uod = b * (c ** a)
    uodBeal = uod
    return uodBeal 

def UodBeggsRobinsonCorrelation( API, T):
    z = 3.0324 - 0.02023 * API
    y = 10 ** z
    X = y * T ** -1.163
    uod = 10 ** X - 1
    uodBeggsRobinson = uod
    return uodBeggsRobinson

 def UodEgbogahNgCorrelation(API, T):
     a = 1.8653 - 0.025086 * API - 0.5644 * log10 (T)
     b = 10 ** a
     c = 10 ** b
     uod = c - 1
     uodEgbogahNg = uod
     return uodEgbogahNg

 def UodGlaso(API, T):
     c = 3.141 * (10 ** 10) * (T ** -3.444)
     d = (10.313 * log10 (T)) - 36.447
     uod = c * (log10(API) ** d)
     uodGlaso = uod
     return uodGlaso
    
def UodKartoatmodjoSchmidtCorrelation(API, T):
    a = (5.7526 * log10(T)) - 26.9718
    uod = 16e8 * (T ** -2.8177) * (log10(API)) ** a
    uodKartoatmodjoSchmidt = uod
    return uodKartoatmodjoSchmidt
    
def UodSattarinEtAlCorrelation(API, T):
    if API >= 28:
        a = (0.00735 * (T ** 2)) - (4.3175 * T) + 641.3572
        b = -(1.51 * T) + 568.84
        d = exp(b / API)        
        uod = a * d / API
    else:
        a = (-5.8936 * 1e7 * (T ** 2)) + (3.511 * 1e10 * T) - (5.2145 * 1e12)
        b = (0.00418 * (T ** 2)) - (2.50406 * T) + 368.78706
        uod = a * (API ** b)
    uodSattarin = uod
    return uodSattarin
    
def UodNaseriCorrelation(API, T):
    a = log10 (API)
    b = log10 (T)
    c = 11.2699 - 4.2699 * a - 2.052 * b
    uod = 10 ** c
    uodNaseri = uod
    return uodNaseri
    
def UodPetroskyFarshadCorrelation(API, T):
    X = (4.59388 * log10 (T)) - 22.82792
    uod = (2.3511e7) * (T ** -2.10255) * (log10 (API)) ** X 
    uodPetroskyFarshad = uod
    return uodPetroskyFarshad

def UodDeGhettoEtAlCorrelation(API, T):
    if API <= 10: # Extra-heavy oil
        a = 10 ** (1.90296 - 0.012619 * API - 0.61748 * log10(T) )
    else:
        a = 10 ** (2.06492 - 0.0179 * API - 0.70226 * log10(T) )
    uod = (10 ** a) - 1 
    uodDeGhetto = uod 
    return uodDeGhetto

def UodPerezMLCorrelation(API, T):
    Z = 1.6288 - 0.01537 * API
    X = (10 ** Z) * (T ** (-0.4479))
    uod = (10 ** X) - 1
    uodPerezML = uod
    return uodPerezML

def UobBeggsRobinsonCorrelation (uod, Rs):
    A = 10.715 * (Rs + 100) ** -0.515
    B = 5.44 * (Rs + 150) ** -0.338
    uob = A * uod**B
    uobBeggsRobinson = uob
    return uobBeggsRobinson

def UobChewConnallyCorrelation (uod, Rs):
    A = 10 ** (Rs*(2.2e-7 * Rs - 7.4e-4))
    b = 0.68/(10 ** (8.62e-5 * Rs)) + 0.25/(10 ** (1.1e-3 * Rs)) + 0.062/(10 ** (3.74e-3 * Rs))  
    uob = A * (uod ** b)
    uobChewConnally = uob
    return uobChewConnally

def UobKhanCorrelation (Rs, API, Yg, T, P, Pb):
    Yo = 141.5/(API + 131.5) # Relative oil density
    Tr = (T + 459.67)/459.67 # Relative Temperature
        
    a = (0.09 * (Yg) **0.5)
    b = (Rs) ** (1.0/3)  
    c = Tr ** 4.5
    d = (1-Yo) ** 3
        
    uoBubble = a / (b*c*d) # Bubble Point Oil Viscosity [cp] 
    
    uob = uoBubble*(1.0*P/Pb)**-0.14 * exp(-2.5e-4*(P-Pb)) # Oil Viscosity Below the Bubble Point [cp]
    uobKhan = uob
        
    return uobKhan 

def UobKartoatmodjoSchmidtCorrelation (uod, Rs):
    y = 10 ** (-0.00081*Rs)
    a = (0.2001 + 0.8428 * 10 ** (-0.000845 * Rs)) 
    b = uod ** (0.43 + 0.5165 * y)
    F = a * b
    uob = -0.06821 + (0.9824 * F) + (0.0004034 * (F ** 2))
    uobKartoatmodjoSchmidt = uob
    return uobKartoatmodjoSchmidt 
    

def UobPetroskyFarshadCorrelation(uod, Rs):
    A = (0.1651) + (0.6165 * (10 **(-6.0866e-4 * Rs)))
    B = (0.5131) + (0.5109 * (10 ** (-1.1831e-3 * Rs)))   
    uob = A * (uod ** B)
    uobPetroskyFarshad = uob
    return uobPetroskyFarshad
    
def UobPerezMLCorrelation(uod, Rs):
    B = (0.5704) + (0.4296 * (10 ** ((-0.00180) * Rs)))
    uob = (uod ** B)
    uobPerezML = uob
    return uobPerezML

def UobGilFonsecaCorrelation(uod, Rs):
    A = 0.76922 + (0.2244 * 10 ** ((-0.0139) * Rs))
    B = 0.10563 + (0.89405 * 10 ** ((-0.00057) * Rs))
    uob = A * (uod ** B)
    uobGilFonseca = uob
    return uobGilFonseca

def UobDeGhettoEtAlCorrelation (uod, Rs, API):
    y = 10**(-0.00081 * Rs)
    if API <= 10: # Extra-heavy oil
        F = (-0.0335 + 1.0785 * 10**(-0.000845 * Rs)) * uod**(0.5798 + 0.3432 * y)
        uob = 2.3945 + 0.8927 * F + 0.001567 * F**2
    else:
        F = (0.2478 + 0.6114 * 10**(-0.000845 * Rs)) * uod**(0.4731 + 0.5158 * y)
        uob = -0.6311 + 1.078 * F + 0.003653 * F**2            
    uobDeGhettoEtAl = uob
    return uobDeGhettoEtAl

def UoaKartoatmodjoSchmidtCorrelation(uoBubble, P, Pb):
    uoa = 1.00081 * uoBubble + 0.001127 * (P - Pb) * ((-0.006517) * (uoBubble ** 1.8148) + 0.038 * uoBubble ** 1.59)
    uoaKartoatmodjoSchmidt = uoa
    return uoaKartoatmodjoSchmidt

def UoaDeGhettoEtAlCorrelation (uoBubble, P, Pb, uod, API):
    if API <= 10: # Extra-heavy oil
        n = (10 ** -2.19) * (uod ** 1.055) * (Pb ** 0.3132)
        d = 10 ** (0.0099 * API)
        uoa = uoBubble - ((1 - (1.0*P/Pb)) * (n/d))
    else:
        uoa = 0.9886 * uoBubble + 0.002763 * (P - Pb) * (-0.01153 * (uoBubble ** 1.7933) + 0.0316 * (uoBubble ** 1.5939))
    uoaDeGhettoEtAl = uoa
    return uoaDeGhettoEtAl  

def UoaBealCorrelation(uoBubble, P, Pb):
    uoa = uoBubble + (0.001 * (P - Pb)) * ((0.024 * uoBubble ** 1.6) + (0.038 * uoBubble ** 0.56)) 
    uoaBeal = uoa
    return uoaBeal

def UoaVasquezBeggsCorrelation(uoBubble, P, Pb):
    C1 = 2.6
    C2 = 1.187
    C3 = -11.513
    C4 = -8.98e-5
    m = C1 * (P ** C2) * exp (C3 + C4 * P) 
    uoa = uoBubble * (1.0*P/Pb) ** m
    uoaVasquezBeggs = uoa
    return uoaVasquezBeggs

def UoaKhanCorrelation(uoBubble, P, Pb):
    uoa = uoBubble * exp((9.6e-5) * (P - Pb))
    uoaKhan = uoa
    return uoaKhan

def UoaPetroskyFarshadCorrelation(uoBubble, P, Pb):
    A = -1.0146 + 1.3322 * log10(uoBubble) - 0.4876 * (log10(uoBubble)) ** 2 - 1.15036 * (log10(uoBubble)) ** 3
    uoa = uoBubble + (1.3449e-3) * (P-Pb) * 10 ** A    
    uoaPetroskyFarshad = uoa
    return uoaPetroskyFarshad

def UoaAbediniCorrelation(uoBubble, P, Pb):
    uoa = uoBubble + 0.001 * (P - Pb) * ((0.05601 * uoBubble ** 1.45198) + (0.47557 * uoBubble ** 0.35997) + (-0.2257 * uoBubble ** 0.86389) + (-0.29598 * Pb ** -0.41866) + (-0.07734 * Pb ** -0.29981) + (-0.42436 * Pb ** -0.1946) + (-1.64149 * Pb ** -0.31339))
    uoaAbedini = uoa
    return uoaAbedini

def UoaPerezMLCorrelation(uob, P, Pb):
    uoa = uob * (1 + (3.181 * 1e-4 * (P - Pb)))
    uoaPerezML = uoa
    return uoaPerezML
    
def PbGlasoCorrelation(Yg, Rsb, API, T, n2Concentration, co2Concentration, h2sConcentration):
    X = (Rsb / Yg) ** 0.816 * T ** 0.172 / API ** 0.989
    PbHC = 10 ** (1.7669 + (1.7447 * log10(X)) - (0.30218 * (log10(X)) ** 2))
    # Effects of nonhydrocarbons on bubble point pressure
    n2Effect = 1 + ((-2.65 * 10 **-4 * API + 5.5 * 10 **-3) * T + (0.0931 * API - 0.8295)) * n2Concentration + ((1.954 * 10 **-11 * API **4.699) * T + (0.027 * API - 2.366)) * n2Concentration **2
    co2Effect = 1 - 693.8 * co2Concentration * T **-1.553
    h2sEffect = 1 - (0.9035 + 0.0015 * API) * h2sConcentration + 0.019 * (45 - API) *  h2sConcentration **2
    Pb = PbHC * n2Effect * co2Effect * h2sEffect
    PbGlaso = Pbd
    return PbGlaso

def SgoBakerSwerdloffCorrelation(T,API,P):
    sgo68 = 39 - 0.2571*API
    sgo100 = 37.5 - 0.2571*API    
    if T <= 68:
        sgoT = sgo68
    elif T>= 100:
        sgoT = sgo100
    else:
        sgoT = sgo68 - (T-68)*(sgo68-sgo100)/32 # Linear interpolation between the values obtained at 68 and 100 °F
    # Effect of dissolved gas on the dead oil interfacial tension
    C = 1 - 0.024 * P**0.45
    sgoP = C*sgoT # Interfacial tension at any pressure
    sgo= sgoP    
    sgoBakerSwerdloff = sgo
    return sgoBakerSwerdloff

def YgHCWichertAzizCorrelation(Yg, n2Concentration, co2Concentration, h2sConcentration):
        YgHC = (Yg - 0.967 * n2Concentration - 1.52 * co2Concentration - 1.18 * h2sConcentration)/(1 - n2Concentration - co2Concentration - h2sConcentration) # Yg is the gravity of the whole mixture and YgHC is the gravity of the hydrocarbon portion
        YgHCWichertAziz = YgHC
        return YgHCWichertAziz

def PscMKayCorrelation(PscHC, n2Concentration, co2Concentration, h2sConcentration):
    PscM = (1 - n2Concentration - co2Concentration - h2sConcentration) * PscHC + 493 * n2Concentration + 1071 * co2Concentration + 1306 * h2sConcentration # Pseudocritical pressure of the whole gas mixture
    PscMKay = PscM
    return PscMKay

def AdjustedPscWichertAzizCorrelation(PscM, TscM, co2Concentration, h2sConcentration):
    A = co2Concentration + h2sConcentration
    B = h2sConcentration
    E = 120 * (A ** 0.9 - A ** 1.6) + 15 * (B ** 0.5 - B ** 4) 
    n = PscM * (TscM - E)
    d = TscM + B * (1 - B) * E
    AdjustedPsc = n/d
    AdjustedPscWichertAziz = AdjustedPsc
    return AdjustedPscWichertAziz

def PscHCBrownKOACorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    PscHC = 677 + 15 * YgHC - 37.5 * (YgHC ** 2) # Pseudocritical pressure of the hydrocarbon portion
    PscBrownKOA = PscHC
    return PscBrownKOA

def PscHcSuttonCorrelation(YgHC, n2Concentration, co2Concentration):
    PscHC = 756.8 - 131.0 * YgHC - 3.6 * (YgHC ** 2) # Pseudocritical pressure of the hydrocarbon portion
    PscHCSutton = PscHC
    return PscHCSutton

def PscHCGuoGhalamborCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    PscHC = 709.604 - 58.718 * YgHC # Pseudocritical pressure of the hydrocarbon portion
    PscHCGuoGhalambor = PscHC
    return PscHCGuoGhalambor

def PscAhmedCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    Psc = 678 - 50 * (YgHC - 0.5) - 206.7 * n2Concentration + 440.0 * co2Concentration + 606.7 * h2sConcentration
    PscAhmed = Psc
    return PscAhmed

def CondensatePscHCStandingCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    PscHC = 706 - 51.7 * YgHC - 11.1 * (YgHC ** 2) # Pseudocritical pressure of the hydrocarbon portion
    PscBrownKOA = PscHC
    return PscBrownKOA

def UgCarrKBCorrelation(T, P, Tsc, Psc, Yg, n2Concentration, co2Concentration, h2sConcentration):
    Tsr = T/Tsc
    Psr = P/Psc
    Ugs = (1.709e-5 - 2.062e-6 * Yg) * (T - 460) + 8.188e-3 - (6.15e-3 * log10(Yg))
    Cco2 = co2Concentration * 1e-3 * (9.08 * log10(Yg) + 6.24)
    Cn2 = n2Concentration * 1e-3 * (8.48 * log10(Yg) + 9.59)
    Ch2s = h2sConcentration * 1e-3 * (8.49 * log10(Yg) + 3.73)
    Ugsc = Ugs + Cco2 + Ch2s + Cn2
    A0 = -2.46211820e0
    A1 = 2.97054714e0
    A2 = -2.86264054e-1
    A3 = 8.05420522e-3
    A4 = 2.80860949e0
    A5 = -3.49803305e0
    A6 = 3.60373020e-1
    A7 = -1.04432413e-2
    A8 = -7.93385684e-1
    A9 = 1.39643306e0
    A10 = -1.49144925e-1
    A11 = 4.41015512e-3
    A12 = 8.39387178e-2
    A13 = -1.86408848e-1
    A14 = 2.03367881e-2
    A15 = -6.09579263e-4
    X = A0 + (A1 * Psr) + (A2 * (Psr ** 2)) + (A3 * (Psr ** 3)) + (Tsr * (A4 + (A5 * Psr) + (A6 * (Psr ** 2)) + (A7 * (Psr ** 3)))) + ((Tsr ** 2) * (A8 + (A9 * Psr) + (A10 * (Psr ** 2)) + (A11 * (Psr ** 3)))) + ((Tsr ** 3) * (A12 + (A13 * Psr) + (A14 * (Psr ** 2)) + (A15 * (Psr ** 3)))) - log(Tsr)
    UgUgsc = exp(X)
    Ug = Ugsc * UgUgsc
    UgCarrKB = Ug
    return UgCarrKB
    
def UgLeeGECorrelation(Tr, P, Yg, Z):
    Mg = 28.96 * Yg # Peso molecular del gas [lb/lbmol]
    k = ((9.4 + 0.02 * Mg) * (Tr ** 1.5))/(209 + (19 * Mg) + Tr)
    x = 3.5 + (986/Tr) + (0.01 * Mg)
    y = 2.4 - 0.2 * x
    pg = 1.4935e-3 * P * Mg/(Z * Tr) # densidad del gas [g/cm3]
    Ug = 1e-4 * k * exp(x * (pg ** y))
    UgLeeGE = Ug
    return UgLeeGE
    
def UgDeanStielCorrelation(Tr, P, Tsc, Psc, Yg, Z):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    Mg = 28.96 * Yg # Peso molecular del gas [lb/lbmol]
    Em = 5.4402 * ((Tsc) ** (1./6.))/(((Mg) ** 0.5) * ((Psc) ** (2./3.))) # Em: Parametro de viscosidad
    pgr = 0.27 * Psr/(Z * Tsr) # pgr: densidad relativa del gas
    if Tsr <= 1.5:
        Ugs = 34e-5 * ((Tsr) ** (8./9.))/Em # Viscosidad del gas a presion atmosferica y temperatura de evaluacion
    elif Tsr > 1.5:
        Ugs = 166.8e-5 * ((0.1338 * Tsr - 0.0932) ** (5./9.))/Em
    Ug = Ugs + (10.8e-5 * (exp(1.439 * pgr) - exp(-1.111 * (pgr ** 1.888))))/Em
    UgDeanStiel = Ug  
    return UgDeanStiel


def ZFactorSaremCorrelation(Tr, P, Tsc, Psc):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    if (Tsr < 1.05) or (Tsr > 2.95) or (Psr < 0.1) or (Psr > 14.9):
        Z = None
    else: 
        x = ((2 * Psr) - 15)/14.8
        y = ((2 * Tsr) - 4)/1.9
        # Especificacion de los polinomios de Legendre en funcion de Psr y Tsr implicitos en x y y
        P0x = 0.7071068
        P1x = 1.224745 * x
        P2x = 0.7905695 * ((3 * (x ** 2)) - 1)
        P3x = 0.9354145 * ((5 * (x ** 3)) - (3 * x))
        P4x = 0.265165 * ((35 * (x ** 4)) - (30 * (x ** 2)) + 3)
        P5x = 0.293151 * ((63 * (x ** 5)) - (70 * (x ** 3)) + (15 * x))
        P0y = 0.7071068
        P1y = 1.224745 * y
        P2y = 0.7905695 * ((3 * (y ** 2)) - 1)
        P3y = 0.9354145 * ((5 * (y ** 3)) - (3 * y))
        P4y = 0.265165 * ((35 * (y ** 4)) - (30 * (y ** 2)) + 3)
        P5y = 0.293151 * ((63 * (y ** 5)) - (70 * (y ** 3)) + (15 * y))
        Z = (2.1433504) * P0x * P0y + (0.0831762) * P0x * P1y + (-0.0214670) * P0x * P2y + (-0.0008714) * P0x * P3y + (0.0042846)* P0x * P4y + (-0.0016595) * P0x * P5y \
        + (0.3312352) * P1x * P0y + (-0.1340361) * P1x * P1y + (0.0668810) * P1x * P2y + (-0.0271743) * P1x * P3y + (0.0088512) * P1x * P4y + (-0.002152) * P1x * P5y \
        + (0.1057287) * P2x * P0y + (-0.0503937) * P2x * P1y + (0.0050925) * P2x * P2y + (0.0105513) * P2x * P3y + (-0.0073182) * P2x * P4y + (0.0026960) * P2x * P5y \
        + (0.0521840) * P3x * P0y + (0.0443121) * P3x * P1y + (-0.0193294) * P3x * P2y + (0.0058973) * P3x * P3y + (0.0015367) * P3x * P4y + (-0.0028327) * P3x * P5y \
        + (0.0197040) * P4x * P0y + (-0.0263834) * P4x * P1y + (0.019262) * P4x * P2y + (-0.0115354) * P4x * P3y + (0.0042910) * P4x * P4y + (-0.0081303) * P4x * P5y \
        + (0.0053096) * P5x * P0y + (0.0089178) * P5x * P1y + (-0.0108948) * P5x * P2y + (0.0095594) * P5x * P3y + (-0.0060114) * P5x * P4y + (0.0031175) * P5x * P5y
    ZFactorSarem = Z 
    return ZFactorSarem
    
def ZFactorHallYarboroughCorrelation(Tpr, Ppr):
    if (Tpr < 1.2) or (Tpr > 3) or (Ppr < 0.1) or (Ppr > 24):
        Z = None
    else:
        # <- Delete for C++ 
        t = 1.0/Tpr
        A = 0.06125 * t * exp(-1.2 * ((1 - t) ** 2))
        B = 14.76 * t - 9.76 * (t ** 2) + 4.58 * (t ** 3)
        C = 90.7 * t - 242.2 * (t ** 2) + 42.4 * (t ** 3)
        D = 2.18 + 2.82 * t
        epsilon = 1.0e-8
        pr = 0 # pr: Reduced density defined by the authors of the method
        prprev = 0.00001
        # Newton-Raphson method
        while (fabs(prprev - pr)) > epsilon:
            pr = prprev
            F = - A * Ppr + ((pr + (pr ** 2) + (pr ** 3) - (pr ** 4))/((1 - pr) ** 3)) - B * (pr ** 2) + C * (pr ** D) 
            dFdpr= (1 + (4 * pr) + (4 * pr ** 2) - (4 * pr ** 3) + (pr ** 4))/((1 - pr) ** 4) - 2 * B * pr + C * D * (pr ** (D-1))
            prf = pr - F/dFdpr
            prprev = prf
        pr = prf
        Z = (0.06125 * Ppr * t * exp(-1.2 * (1 - t) ** 2))/pr
    ZFactorHallYarborough = Z
    return ZFactorHallYarborough
    

def ZFactorDranchukPRCorrelation(Tpr, Ppr):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    A1 = 0.31506237
    A2 = -1.0467099
    A3 = -0.57832729
    A4 = 0.53530771
    A5 = -0.61232032
    A6 = -0.10488813
    A7 = 0.68157001
    A8 = 0.68446549
    
    epsilon = 1.0e-8
    Z = 0.5
    Zprev = 0.6
    
    while fabs(Zprev - Z) > epsilon:
        Z = Zprev
        pr = 0.27 * Psr/(Z * Tsr)
        F = Z - (1 + (A1 + (A2/Tsr) + (A3/(Tsr **3))) * pr + ((A4 + (A5/Tsr)) * (pr ** 2)) + ((A5 * A6 * (pr ** 5))/Tsr) + A7 * (1 + A8 * (pr ** 2)) * ((pr ** 2)/(Tsr ** 3)) * exp(-A8 * (pr ** 2)))
        dFdZ = 1 + (A1 + (A2/Tsr) + (A3/(Tsr ** 3))) * (pr/Z) + 2 * (A4 + (A5/Tsr)) * (pr ** 2)/Z + ((5 * A5 * A6 * (pr **5))/(Z * Tsr)) + ((2 * A7 * (pr ** 2))/(Z * (Tsr ** 3))) * (1 + A8 * (pr ** 2) - ((A8 * (pr ** 2)) ** 2)) * exp(-A8 * (pr ** 2))
        Zf = Z - F/dFdZ
        Zprev = Zf
                
    Z = Zf   
    ZFactorDranchukPR = Z
    return ZFactorDranchukPR


def UodDindorukChristmanCorrelation(API, T, Pb, Rsb):
    a1 = 14.505357625
    a2 = -44.868655416
    a3 = 9.36579e9
    a4 = -4.194017808
    a5 = -3.1461171e-9
    a6 = 1.517652716
    a7 = 0.010433654
    a8 = -0.000776880
    A = a1 * log10(T) + a2
    uod = (a3 * T**a4 * (log10(API))**A) / (a5 * Pb**a6 + a7 * Rsb**a8)
    uodDindorukChristman = uod
    return uodDindorukChristman

def UobDindorukChristmanCorrelation(uod, Rs):
    a1 = 1.0
    a2 = 4.740729e-4
    a3 = -1.023451e-2
    a4 = 6.600358e-1
    a5 = 1.075080e-3
    a6 = 1.0
    a7 = -2.191172e-5
    a8 = -1.660981e-2
    a9 = 4.233179e-1
    a10 = -2.273945e-4
    A = (a1/exp(a2 * Rs)) + (a3 * Rs**a4/exp(a5 * Rs))
    B = (a6/exp(a7 * Rs)) + (a8 * Rs**a9/exp(a10 * Rs))
    uob = A * uod**B
    uobDindorukChristman = uob
    return uobDindorukChristman
    
def UoaDindorukChristmanCorrelation(uoBubble, P, Pb, Rs):
    a1 = 0.776644115
    a2 = 0.987658646
    a3 = -0.190564677
    a4 = 0.009147711
    a5 = -0.000019111
    a6 = 0.000063340
    A = a1 + a2 * log10(uoBubble) + a3 * log10(Rs) + a4 * uoBubble * log10(Rs) + a5 * (P - Pb)
    uoa = uoBubble + a6 * (P - Pb) * 10**A
    uoaDindorukChristman = uoa
    return uoaDindorukChristman

def ZFactorDranchukAKCorrelation(Tr, P, Tsc, Psc):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    A1 = 0.3265
    A2 = -1.07
    A3 = -0.5339
    A4 = 0.01569
    A5 = -0.05165
    A6 = 0.5475
    A7 = -0.7361
    A8 = 0.1844
    A9 = 0.1056
    A10 = 0.6134
    A11 = 0.721
    epsilon = 1.0e-8
    Z = 0.5
    Zprev = 0.6
    while fabs(Zprev - Z) > epsilon:
        Z = Zprev
        pr = 0.27 * Psr/(Z * Tsr)
        F = Z - (1 + (A1 + (A2/Tsr) + (A3/(Tsr **3)) + (A4/(Tsr ** 4)) + (A5/(Tsr ** 5))) * pr + (A6 + (A7/Tsr) + (A8/(Tsr ** 2))) * (pr ** 2) - A9 * ((A7/Tsr) + (A8/(Tsr ** 2))) * (pr ** 5) + A10 * (1 + A11 * (pr ** 2)) * ((pr ** 2)/(Tsr ** 3)) * exp(-A11 * (pr ** 2)))
        dFdZ = 1 + ((A1 + (A2/Tsr) + (A3/(Tsr ** 3)) + (A4/(Tsr ** 4)) + (A5/(Tsr ** 5))) * (pr/Z)) + (2 * (A6 + (A7/Tsr) + (A8/(Tsr ** 2))) * ((pr ** 2)/Z)) - ((5 * A9) * ((A7/Tsr) + (A8/(Tsr ** 2))) * ((pr ** 5)/Z)) + (((2 * A10 * (pr ** 2))/(Z * (Tsr ** 3))) * (1 + (A11 * (pr ** 2)) - ((A11 * (pr ** 2)) ** 2)) * exp (-A11 * (pr ** 2)))      
        Zf = Z - F/dFdZ
        Zprev = Zf
    Z = Zf   
    ZFactorDranchukAK = Z
    return ZFactorDranchukAK
    
def ZFactorGopalCorrelation(Tr, P, Tsc, Psc):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    if 0.2 < Psr <= 1.2:
        if 1.05 < Tsr <= 1.2:
            Z = Psr * ((1.6643 * Tsr) - 2.2114) - 0.3647 * Tsr + 1.4385
        elif 1.2 < Tsr <= 1.4:
            Z = Psr * ((0.0522 * Tsr) - 0.8511) - 0.0364 * Tsr + 1.0490
        elif 1.4 < Tsr <= 2.0:
            Z = Psr * ((0.1391 * Tsr) - 0.2988) + 0.0007 * Tsr + 0.9969
        elif 2.0 < Tsr <= 3.0:
            Z = Psr * ((0.0295 * Tsr) - 0.0825) + 0.0009 * Tsr + 0.9967
    elif 1.2 < Psr <= 2.8:
        if 1.05 < Tsr <= 1.2:
            Z = Psr * ((-1.3570 * Tsr) + 1.4942) + 4.6315 * Tsr - 4.7009
        elif 1.2 < Tsr <= 1.4:
            Z = Psr * ((0.1717 * Tsr) - 0.3232) + 0.5869 * Tsr + 0.1229   
        elif 1.4 < Tsr <= 2.0:
            Z = Psr * ((0.0984 * Tsr) - 0.2053) + 0.0621 * Tsr + 0.8580
        elif 2.0 < Tsr <= 3.0:
            Z = Psr * ((0.0211 * Tsr) - 0.0527) + 0.0127 * Tsr + 0.9549
    elif 2.8 < Psr <= 5.4:
        if 1.05 < Tsr <= 1.2:
            Z = Psr * ((-0.3278 * Tsr) + 0.4752) + 1.8223 * Tsr - 1.9036
        elif 1.2 < Tsr <= 1.4:
            Z = Psr * ((-0.2521 * Tsr) + 0.3871) + 1.6087 * Tsr - 1.6635
        elif 1.4 < Tsr <= 2.0:
            Z = Psr * ((-0.0284 * Tsr) + 0.0625) + 1.4714 * Tsr - 0.0011
        elif 2.0 < Tsr <= 3.0:
            Z = Psr * ((0.0041 * Tsr) + 0.0039) + 0.0607 * Tsr + 0.7927
    elif 5.4 < Psr <= 15: # para cualquier Tsr entre 1.05 y 3.0
        Z = Psr * ((0.711 + (3.66 * Tsr) + 0.0039) ** -1.4667) - (1.637 /(0.319 * Tsr + 0.522)) + 2.071
        ZFactorGopal = Z
        return ZFactorGopal

def ZFactorBrillBeggsCorrelation(Tr, P, Tsc, Psc):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    A = 1.39 * ((Tsr - 0.92) ** 0.5) - 0.36 * Tsr - 0.10
    B = (0.62 - 0.23 * Tsr) * Psr + ((0.066/(Tsr - 0.86)) - 0.037) * (Psr ** 2) + (0.32/(10 ** (9 * (Tsr - 1)))) * (Psr ** 6)
    C = 0.132 - 0.32 * log10(Tsr)
    D = 10 ** (0.3106 - 0.49 * Tsr + 0.1824 * (Tsr ** 2))
    mathDomain = B  #Se define el dominio matematico de la funcion de Z, sensible al argumento B de la funcion exponencial         
    if mathDomain > 700:
        Z = A + ((0)) + C * (Psr ** D) #el cero se corresponde con numeros muy altos en el denominador i.e >1e+300
    else:
        Z = A + ((1 - A)/exp(B)) + C * (Psr ** D)
    ZFactorBrillBeggs = Z
    return ZFactorBrillBeggs

def ZFactorPapayCorrelation(Tr, P, Tsc, Psc):
    Tsr = 1.0*Tr/Tsc
    Psr = 1.0*P/Psc
    Z = 1 - (3.52 * Psr/(10 ** (0.9813 * Tsr))) + ((0.274 * (Psr ** 2))/(10 ** (0.8157 * Tsr)))
    ZFactorPapay = Z 
    return ZFactorPapay

def TscMKayMixingRuleCorrelation(TscHC, n2Concentration, co2Concentration, h2sConcentration):
    TscM = (1 - n2Concentration - co2Concentration - h2sConcentration) * TscHC + 227 * n2Concentration + 548 * co2Concentration + 672 * h2sConcentration # Pseudocritical temperature of the whole gas mixture
    TscMKay = TscM
    return TscMKay

def AdjustedTscMWichertAzizCorrelation(TscM, co2Concentration, h2sConcentration):
    A = co2Concentration + h2sConcentration
    B = h2sConcentration
    E = 120 * (A ** 0.9 - A ** 1.6) + 15 * (B ** 0.5 - B ** 4) 
    AdjustedTscM = TscM - E
    AdjustedTscMWichertAziz = AdjustedTscM
    return AdjustedTscMWichertAziz
    

def TscHCStandingCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    TscHC = 168 + 325 * YgHC - 12.5 * (YgHC ** 2) # Pseudocritical temperature of the hydrocarbon portion
    TscHCStanding = TscHC
    return TscHCStanding    

def TscHCStandingHeavierFractionsCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    TscHC = 187 + 330 * YgHC - 71.5 * (YgHC ** 2) # Pseudocritical temperature of the hydrocarbon portion
    TscHCStanding = TscHC
    return TscHCStanding

def TscHCSuttonCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    TscHC = 169.2 + 349.5 * YgHC - 74.0 * (YgHC ** 2) # Pseudocritical temperature of the hydrocarbon portion
    TscHCSutton = TscHC
    return TscHCSutton

def TpcHCElsharkawyEtAlCorrelation(YgHC, n2Concentration, co2Concentration, h2sConcentration):
    TpcHC = 149.18 + (358.14 * YgHC) - (66.976 * (YgHC ** 2)) # Pseudocritical temperature of the hydrocarbon portion
    TpcHCElsharkawy = TpcHC
    return TpcHCElsharkawy

def CgSaremCorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    x = (2 * Ppr - 15)/14.8
    y = (2 * Tpr - 4)/1.9
    P0x = 0.0
    P1x = 0.16551
    P2x = 0.641002 * x
    P3x = 0.379221 * ((5 * (x ** 2)) - 1)
    P4x = 0.716652 * ((7 * (x ** 3)) - (3 * x))
    P5x = 0.594225 * ((21 * (x ** 4)) - (14 * (x ** 2)) + 1)
    P0y = 0.7071068
    P1y = 1.224745 * y
    P2y = 0.7905695 * ((3 * (y ** 2)) - 1)
    P3y = 0.9354145 * ((5 * (y ** 3)) - (3 * y))
    P4y = 0.265165 * ((35 * (y ** 4)) - (30 * (y ** 2)) + 3)
    P5y = 0.293151 * ((63 * (y ** 5)) - (70 * (y ** 3)) + (15 * y))
    dZdPpr = (2.1433504) * P0x * P0y + (0.0831762) * P0x * P1y + (-0.0214670) * P0x * P2y + (-0.0008714) * P0x * P3y + (0.0042846)* P0x * P4y + (-0.0016595) * P0x * P5y \
    + (0.3312352) * P1x * P0y + (-0.1340361) * P1x * P1y + (0.0668810) * P1x * P2y + (-0.0271743) * P1x * P3y + (0.0088512) * P1x * P4y + (-0.002152) * P1x * P5y \
    + (0.1057287) * P2x * P0y + (-0.0503937) * P2x * P1y + (0.0050925) * P2x * P2y + (0.0105513) * P2x * P3y + (-0.0073182) * P2x * P4y + (0.0026960) * P2x * P5y \
    + (0.0521840) * P3x * P0y + (0.0443121) * P3x * P1y + (-0.0193294) * P3x * P2y + (0.0058973) * P3x * P3y + (0.0015367) * P3x * P4y + (-0.0028327) * P3x * P5y \
    + (0.0197040) * P4x * P0y + (-0.0263834) * P4x * P1y + (0.019262) * P4x * P2y + (-0.0115354) * P4x * P3y + (0.0042910) * P4x * P4y + (-0.0081303) * P4x * P5y \
    + (0.0053096) * P5x * P0y + (0.0089178) * P5x * P1y + (-0.0108948) * P5x * P2y + (0.0095594) * P5x * P3y + (-0.0060114) * P5x * P4y + (0.0031175) * P5x * P5y
    Cgr = (1/Ppr) - (1/Z) * dZdPpr
    Cg =  Cgr/Ppc
    CgSarem = Cg
    return CgSarem

def CgHallYarboroughCorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    A = 0.06125 * (1/Tpr) * exp((-1.2) * ((1 - (1/Tpr)) ** 2))
    B = 14.76 * (1/Tpr) - 9.76 * ((1/Tpr) ** 2) + 4.58 * ((1/Tpr) ** 3)
    C = 90.7 * (1/Tpr) - 242.2 * ((1/Tpr) ** 2) + 42.4 * ((1/Tpr) ** 3)
    D = 2.18 + 2.82 * (1/Tpr)
    epsilon = 1.0e-8
    pr = 0
    prprev = 0.01
    while fabs(prprev - pr) > epsilon:
        pr = prprev
        F = -(A * Ppr) + (pr + (pr ** 2) + (pr ** 3) - (pr ** 4))/((1 - pr) ** 3) - B * (pr ** 2) + C * (pr ** D) 
        dFdpr = ((1 + (4 * pr) + (4 * pr) ** 2 - (4 * pr) ** 3 + (4 * pr) ** 4)/((1 - pr) ** 4)) - 2 * B * pr + C * D * (pr ** (D-1))
        prf = pr - F/dFdpr
        prprev = prf
    
    pr = prf
    dprdPpr = A * (((1-pr) ** 4)/(1 + (4 * pr) + 4 * (pr ** 2) - 4 * (pr ** 3) + (pr ** 4) - ((1-pr) ** 4) * ((2 * B * pr ) - (C *D * (pr ** (D-1))))))
    dZdPpr = (A/pr) - (A * Ppr/(pr ** 2)) * dprdPpr
    Cgr = (1/Ppr) - (1/Z) * dZdPpr
    Cg =  Cgr/Ppc
    CgHallYarborough = Cg
    return CgHallYarborough

def CgMattarBACorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    A1 = 0.31506237
    A2 = -1.0467099
    A3 = -0.57832729
    A4 = 0.53530771
    A5 = -0.61232032
    A6 = -0.10488813
    A7 = 0.68157001
    A8 = 0.68446549
    pr = 0.27 * Ppr/(Z * Tpr)
    dZdpr = A1 + (A2/Tpr) + (A3/(Tpr ** 3)) + 2 * (A4 + (A5/Tpr)) * pr + 5 * A5 * A6 * ((pr ** 4)/Tpr) + ((2 * A7 * pr)/(Tpr **3)) * (1 + A8 * (pr ** 2) - (A8 * (pr ** 2)) ** 2) * exp(-A8 * (pr ** 2))
    Cgr = (1/Ppr) - (0.27/((Z ** 2) * Tpr)) * (dZdpr/(1 + ((pr/Z) * dZdpr)))
    Cg =  Cgr/Ppc
    CgMattarBA = Cg
    return CgMattarBA


def CgGopalCorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    if 0.2 < Ppr <= 1.2:
        if 1.05 < Tpr <= 1.2:
            dZdPpr = 1.6643 * Tpr - 2.2114
        elif 1.2 < Tpr <= 1.4:
            dZdPpr = 0.0522 * Tpr - 0.8511
        elif 1.4 < Tpr <= 2.0:
            dZdPpr = 0.1391 * Tpr - 0.2988
        elif 2.0 < Tpr <= 3.0:
            dZdPpr = 0.0295 * Tpr - 0.0825
    elif 1.2 < Ppr <= 2.8:
        if 1.05 < Tpr <= 1.2:
            dZdPpr = -1.3570 * Tpr + 1.4942            
        elif 1.2 < Tpr <= 1.4:
            dZdPpr = 0.1717 * Tpr - 0.3232 
        elif 1.4 < Tpr <= 2.0:
            dZdPpr = 0.0984 * Tpr - 0.2053            
        elif 2.0 < Tpr <= 3.0:
            dZdPpr = 0.0211 * Tpr - 0.0527            
    elif 2.8 < Ppr <= 5.4:
        if 1.05 < Tpr <= 1.2:
            dZdPpr = -0.3278 * Tpr + 0.4752            
        elif 1.2 < Tpr <= 1.4:
            dZdPpr = -0.2521 * Tpr + 0.3871            
        elif 1.4 < Tpr <= 2.0:
            dZdPpr = -0.0284 * Tpr + 0.0625
        elif 2.0 < Tpr <= 3.0:
            dZdPpr = 0.0041 * Tpr + 0.0039           
    elif 5.4 < Ppr <= 15: # para cualquier Tpr entre 1.05 y 3.0
        dZdPpr = (0.711 + 3.66 * Tpr) ** (-1.4667)
    
    Cgr = (1/Ppr) - (1/Z) * dZdPpr
    Cg =  Cgr/Ppc
    CgGopal = Cg
    return CgGopal       

def CgBrillBeggsCorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    A = (1.39 * ((Tpr - 0.92) ** 0.5)) - (0.36 * Tpr) - 0.101
    B = ((0.62 - 0.23 * Tpr) * Ppr) + (((0.066/(Tpr - 0.86)) - 0.037) * (Ppr ** 2)) + ((0.32/(10 ** (9 * (Tpr - 1)))) * (Ppr ** 6))
    C = 0.132 - (0.32 * log10(Tpr))
    D = 10 ** (0.3106 - (0.49 * Tpr) + (0.1824 * (Tpr ** 2)))
                
    mathDomain = B  #Se define el dominio matematico de la funcion de dZdPpr, sensible al argumento B de la funcion exponencial         
    dZdPpr = -((1-A)/(((0.62 - (0.23 * Tpr)) + (((0.132/(Tpr - 0.86)) - 0.074) * Ppr) + ((1.92/(10 ** (9 * (Tpr - 1)))) * (Ppr ** 5))) * exp(B))) + (C * D * (Ppr ** (D - 1)))
    Cgr = (1/Ppr) - ((1/Z) * dZdPpr)
    Cg = Cgr/Ppc                
    CgBrillBeggs = Cg
    return CgBrillBeggs 

def CgPapayCorrelation(Tr, P, Tpc, Ppc, Z):
    Tpr = 1.0*Tr/Tpc
    Ppr = 1.0*P/Ppc
    dZdPpr = -(3.52/(10 ** (0.9813 * Tpr))) + (0.548 * Ppr/(10 ** (0.8157 * Tpr)))
    Cgr = (1/Ppr) - (1/Z) * dZdPpr
    Cg =  Cgr/Ppc
    CgPapay = Cg
    return CgPapay 

def PwSpiveyMNCorrelation(T, P, S):
    P = 1.0*P/1000000 # Transformacion de pascal a MPA
    ppwr = (((-0.127213) * ((T/100.) ** 2)) + ((0.645486) * (T/100.)) + (1.03265))/(((-0.070291) * ((T/100.) ** 2)) + ((0.639589) * (T/100.)) + 1)
    Epw = (((4.221) * ((T/100.) ** 2)) + ((-3.478) * (T/100.)) + (6.221))/(((0.5182) * ((T/100.) ** 2)) + ((-0.4405) * (T/100.)) + 1)
    Fpw = (((-11.403) * ((T/100.) ** 2)) + ((29.932) * (T/100.)) + (27.952))/(((0.20684) * ((T/100.) ** 2)) + ((0.3768) * (T/100.)) + 1)
    m = (1000 * (S/100.))/(58.4428 * (1 - (S/100.)))
    D1 = (((-1.1149e-4) * ((T/100.) ** 2)) + ((1.7105e-4) * (T/100.)) + (-4.3766e-4))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    D2 = (((-8.878e-4) * ((T/100.) ** 2)) + ((-1.388e-4) * (T/100.)) + (-2.96318e-3))/(((0) * ((T/100.) ** 2)) + ((0.51103) * (T/100.)) + 1)
    D3 = (((2.1466e-3) * ((T/100.) ** 2)) + ((1.2427e-2) * (T/100.)) + (4.2648e-2))/(((-8.1009e-2) * ((T/100.) ** 2)) + ((0.525417) * (T/100.)) + 1)
    D4 = (((2.356e-4) * ((T/100.) ** 2)) + ((-3.636e-4) * (T/100.)) + (-2.278e-4))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    pgfwr = ppwr + (D1 * (m ** 2)) + (D2 * (m ** (3./2))) + (D3 * m) + (D4 * (m ** (1./2)))
    E = (((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + (0.1249))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    F1 = (((-0.617) * ((T/100.) ** 2)) + ((-0.747) * (T/100.)) + (-0.4339))/(((0) * ((T/100.) ** 2)) + ((10.26) * (T/100.)) + 1)
    F2 = (((0) * ((T/100.) ** 2)) + ((9.917) * (T/100.)) + (5.1128))/(((0) * ((T/100.) ** 2)) + ((3.892) * (T/100.)) + 1)
    F3 = (((0.0365) * ((T/100.) ** 2)) + ((-0.0369) * (T/100.)) + (0))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    Ew = Epw + (E * m)
    Fw = Fpw + (F1 * (m ** (3./2))) + (F2 * (m)) + (F3 * (m ** (1./2)))
    Iwr = (1/Ew) * log(fabs(Ew + Fw))
    Iw = (1/Ew) * log(fabs((Ew * (P/70.))+ Fw))
    pgfw = pgfwr * exp(Iw - Iwr)
    vgfw = 1/pgfw
    m = (1000 * (S/100.))/(58.4428 * (1 - (S/100.)))
    Tc = 647.096 # K
    Pc = 22.064 # MPa
    v = 1 - ((T + 273.15)/Tc)
    lnPv = ((Tc/(T + 273.15)) * (((-7.85951783) * v) + ((1.84408259) * (v ** 1.5)) + ((-11.7866497) * (v ** 3)) + ((22.6807411) * (v ** 3.5)) + ((-15.9618719) * (v ** 4)) + ((1.80122502) * (v ** 7.5)))) + log(Pc)
    Pv = exp(lnPv)
    A = (((0) * ((T/100) ** 2)) + ((-0.004462) * (T/100)) + (-0.06763))/(((0) * ((T/100) ** 2)) + ((0) * (T/100)) + 1)
    B = (((-0.03602) * ((T/100) ** 2)) + ((0.18917) * (T/100)) + (0.97242))/(((0) * ((T/100) ** 2)) + ((0) * (T/100)) + 1)
    C = (((0.6855) * ((T/100) ** 2)) + ((-3.1992) * (T/100)) + (-3.7968))/(((0.07711) * ((T/100) ** 2)) + ((0.2229) * (T/100)) + 1)
    mathDomain = P - Pv
    mCH4pw = exp((A * ((log(P - Pv)) ** 2)) + (B * log (P - Pv)) + (C))
    C1 = (-0.80898) + ((1.0827e-3) * (T + 273.15)) + ((183.85)/(T + 273.15)) + ((3.924e-4) * P) + ((-1.97e-6) * (P ** 2))
    C2 = (-3.89e-3)
    mCH4w = mCH4pw * exp(((-2) * C1 * m) - (C2 * (m ** 2)))
    C3 = (7.698589e-2) + ((-5.0253331e-5) * (T + 273.15)) + ((-30.092013)/(T + 273.15)) + ((4.8468502e+3)/((T + 273.15) ** 2))  
    C4 = (3.924e-4) + (2 * (-1.97e-6) * P)
    C5 = 0
    VMCH4w = 8.314467 * (T + 273.15) * ((C3) + ((2 * m) * C4) + ((m ** 2) * C5))
    pw = ((1000) + (m * 58.4428) + (mCH4w * 16.043))/((((1000) + (m * 58.4428)) * vgfw) + (16.043 * VMCH4w))
    pwSpiveyMN = pw
    return pwSpiveyMN

def PwSpiveyMNGasFreeCorrelation(T, P, S):
    P = P/1000000 # Transformacion de pascal a MPA
    ppwr = (((-0.127213) * ((T/100.) ** 2)) + ((0.645486) * (T/100.)) + (1.03265))/(((-0.070291) * ((T/100.) ** 2)) + ((0.639589) * (T/100.)) + 1)
    Epw = (((4.221) * ((T/100.) ** 2)) + ((-3.478) * (T/100.)) + (6.221))/(((0.5182) * ((T/100.) ** 2)) + ((-0.4405) * (T/100.)) + 1)
    Fpw = (((-11.403) * ((T/100.) ** 2)) + ((29.932) * (T/100.)) + (27.952))/(((0.20684) * ((T/100.) ** 2)) + ((0.3768) * (T/100.)) + 1)
    m = (1000 * (S/100.))/(58.4428 * (1 - (S/100.)))
    D1 = (((-1.1149e-4) * ((T/100.) ** 2)) + ((1.7105e-4) * (T/100.)) + (-4.3766e-4))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    D2 = (((-8.878e-4) * ((T/100.) ** 2)) + ((-1.388e-4) * (T/100.)) + (-2.96318e-3))/(((0) * ((T/100.) ** 2)) + ((0.51103) * (T/100.)) + 1)
    D3 = (((2.1466e-3) * ((T/100.) ** 2)) + ((1.2427e-2) * (T/100.)) + (4.2648e-2))/(((-8.1009e-2) * ((T/100.) ** 2)) + ((0.525417) * (T/100.)) + 1)
    D4 = (((2.356e-4) * ((T/100.) ** 2)) + ((-3.636e-4) * (T/100.)) + (-2.278e-4))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    pgfwr = ppwr + (D1 * (m ** 2)) + (D2 * (m ** (1.5))) + (D3 * m) + (D4 * (m ** (1/2)))
    E = (((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + (0.1249))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    F1 = (((-0.617) * ((T/100.) ** 2)) + ((-0.747) * (T/100.)) + (-0.4339))/(((0) * ((T/100.) ** 2)) + ((10.26) * (T/100.)) + 1)
    F2 = (((0) * ((T/100.) ** 2)) + ((9.917) * (T/100.)) + (5.1128))/(((0) * ((T/100.) ** 2)) + ((3.892) * (T/100.)) + 1)
    F3 = (((0.0365) * ((T/100.) ** 2)) + ((-0.0369) * (T/100.)) + (0))/(((0) * ((T/100.) ** 2)) + ((0) * (T/100.)) + 1)
    Ew = Epw + (E * m)
    Fw = Fpw + (F1 * (m ** (1.5))) + (F2 * (m)) + (F3 * (m ** (0.5)))
    Iwr = (1/Ew) * log(fabs(Ew + Fw))
    Iw = (1/Ew) * log(fabs((Ew * (P/70.))+ Fw))
    pgfw = pgfwr * exp(Iw - Iwr)
    pwSpiveyMNGasFree = pgfw
    return pwSpiveyMNGasFree
        
def PwMcCainCorrelation(S, Bw):
    pwl = 62.368 + (0.438603 * S) + (1.60074e-3 * (S ** 2))
    pw = pwl/Bw
    pwMcCain = pw
    return pwMcCain

def UwMaoDuanCorrelation(T, P, S):
    T=1.0*T
    P=1.0*P
    S=1.0*S
    ppwr = (((-0.127213) * ((T/100) ** 2)) + ((0.645486) * (T/100)) + 1.03265)/(((-0.070291) * ((T/100) ** 2)) + ((0.639589) * (T/100)) + 1)
    Epw = ((4.221 * ((T/100) ** 2)) + ((-3.478) * (T/100)) + 6.221)/((0.5182 * ((T/100) ** 2)) + ((-0.4405) * (T/100)) + 1)
    Fpw = (((-11.403) * ((T/100) ** 2)) + ((29.932) * (T/100)) + 27.952)/((0.20684 * ((T/100) ** 2)) + ((0.3768) * (T/100)) + 1)
    Ipwr = (1/Epw) * log(fabs(Epw + Fpw))
    Ipw = (1/Epw) * log(fabs((Epw * (P/70))+ Fpw))
    ppw = ppwr * exp(Ipw - Ipwr)
    Upw = exp((((0.28853170e7) * ((T + 273) ** (-2))) + ((-0.11072577e5) * ((T + 273) ** (-1))) + ((-0.90834095e1) * ((T + 273) ** (0))) + ((0.30925651e-1) * ((T + 273) ** (1))) + ((-0.27407100e-4) * ((T + 273) ** (2)))) + (ppw * (((-0.19283851e7) * ((T + 273) ** (-2))) + ((0.56216046e4) * ((T + 273) ** (-1))) + ((0.13827250e2) * ((T + 273) ** (0))) + ((-0.47609523e-1) * ((T + 273) ** (1))) + ((0.35545041e-4) * ((T + 273) ** (2))))))
    A = (-0.21319213) + ((0.13651589e-2) * (T + 273)) + ((-0.12191756e-5) * ((T + 273) ** 2))
    B = (0.69161945e-1) + ((-0.27292263e-3) * (T + 273)) + ((0.20852448e-6) * ((T + 273) ** 2))
    C = (-0.25988855e-2) + ((0.77989227e-5) * (T + 273))
    m = (1000 * (S/100))/(58.4428 * (1 - (S/100)))
    Uwr = exp((A * m) + (B * (m ** 2)) + (C * (m ** 3)))
    Uw = Uwr * Upw 
    UwMaoDuan = Uw
    return UwMaoDuan


def UwVanWingenCorrelation(T):
    Uw = exp(1.003 - (1.479e-2 * T) + (1.982e-5 * (T ** 2)))
    UwVanWingen = Uw
    return UwVanWingen
        

def UwMatthewsRusselCorrelation(T, P, S):
    A = -0.04518 + (0.009313 * S) - (0.000393 * (S ** 2))
    B = 70.634 + (0.09576 * (S ** 2))
    Uwat = A + (B/T)
    Uw = Uwat * (1 + (3.5e-12 * (P ** 2) * (T - 40)))
    UwMatthewsRussel = Uw
    return UwMatthewsRussel

def UwMcCainCorrelation(T, P, S):
    A = 109.574 - (8.40564 * S) + (0.313314 * (S ** 2)) + (8.72213e-3 * (S ** 3))
    B = -1.12166 + (2.63951e-2 * S) - (6.79461e-4 * (S ** 2)) - (5.47119e-5 * (S ** 3)) + (1.55586e-6 * (S ** 4))
    Uwat = A * (T ** B)
    Uw = Uwat * (0.9994 + (4.0295e-5 * P) + (3.1062e-9 * (P ** 2)))
    UwMcCain = Uw
    return UwMcCain

def UwMcCoyCorrelation(T, S):
    Uwp = 0.02414 * (10 ** (247.8/(((0.555555556 * T) + 255.37) - 140)))
    Uw =  Uwp * (1 - (1.87e-3 * (S ** 0.5)) + (2.18e-4 * (S ** 2.5)) + (((T ** 0.5) - (1.35e-2 * T)) * ((2.76e-3 * S) - (3.44e-4 * (S ** 1.5)))))
    UwMcCoy = Uw
    return UwMcCoy
