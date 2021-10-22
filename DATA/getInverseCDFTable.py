'''
Description: Generate 100 equal-probable bins of a CDF
Author: Ming Fang
Date: 2021-10-22 11:24:39
LastEditors: Ming Fang
LastEditTime: 2021-10-22 15:34:43
'''
import numpy as np
from scipy.optimize import newton
import numpy.polynomial.legendre as L


def read_float(v):
    """
    Convert ENDF6 string to float
    """
    if v.strip() == '':
        return 0.
    try:
        return float(v)
    except ValueError:
        # ENDF6 may omit the e for exponent
        return float(v[0] + v[1:].replace('+', 'e+').replace('-', 'e-'))  # don't replace leading negative sign


def loadCoefs(fname):
    with open(fname) as f:
        for i in range(12):
            next(f)
        coefs = []
        ergs = []
        recordNum = -1
        for line in f:
            # line = line.strip()
            if line[:11].isspace():
                # second line of current record
                for i in range(2):
                    st = 12 + i * 11
                    ed = 12 + (i+1) * 11
                    coef.append(read_float(line[st:ed]))
            else:
                if recordNum > -1:
                    for i in range(len(coef), MAXORDER + 1):
                        coef.append(0.)
                    ergs.append(energy)
                    coefs.append(coef)
                recordNum = recordNum + 1
                if recordNum > 10000:
                    break
                # new record
                energy = read_float(line[:11])
                coef = [1]  # coef for P_0
                for i in range(6):
                    st = 12 + i * 11
                    ed = 12 + (i+1) * 11
                    coef.append(read_float(line[st:ed]))
        # save last record
        ergs.append(energy)
        coefs.append(coef)
    return [ergs, coefs]


def CDFCoef(coef):
    return L.legint(coef, lbnd=-1)


MAXORDER = 8
ergs, coefs = loadCoefs("O16-elastic-scattering-DA.txt")
for j in range(len(coefs)):
    for i in range(len(coefs[j])):
        coefs[j][i] = coefs[j][i] * (2*i+1) / 2
CDFTables = []
for i, energy in enumerate(ergs):
    if i%100 == 0:
        print(i)
    bin = []
    fprime = L.Legendre(coefs[i])
    fprime2 = L.Legendre(L.legder(coefs[i]))
    for j in np.arange(1, 100):
        cdfcoef = CDFCoef(coefs[i])
        p = j / 100
        cdfcoef[0] -= p
        f = L.Legendre(cdfcoef)
        root = f.roots()
        counts = 0
        for r in root:
            if abs(r.imag) < 1e-3 and abs(r.real) < 1.001:
                counts += 1
                bin.append(r.real)
                # print(r)
        if counts != 1:
            raise ValueError("No or more than one roots found.")

        # bin.append(newton(f, 2*p-1, fprime=fprime, maxiter=1000, fprime2=fprime2))
    bin.append(1.0)
    bin = np.insert(bin, 0, -1.0)
    bin = np.insert(bin, 0, energy)
    CDFTables.append(np.array(bin))
    # print(len(bin))
CDFTables = np.array(CDFTables)
print(CDFTables.shape)
np.savetxt("O16-elastic-scattering-CDF.txt", CDFTables, fmt='%.8e', delimiter=' ')

