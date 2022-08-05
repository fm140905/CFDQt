'''
Description: Calculate PDF table
Author: Ming Fang
Date: 2021-10-22 09:52:48
LastEditors: Ming Fang
LastEditTime: 2021-10-22 12:20:58
'''
import numpy as np
import numpy.polynomial.legendre as L


def getlegendreTable(n):
    xs = np.linspace(-1, 1, num=100)
    coef = np.zeros(n+1)
    coef[-1] = 1
    Pi = L.Legendre(coef)
    return np.array(Pi(xs))


def getPDFTable(coef):
    val = np.zeros(legendreTables[0].shape)
    for i, v in enumerate(coef):
        val += (2*i+1) / 2 * v * legendreTables[i]
    return val


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


MAXORDER = 8
legendreTables = np.array([getlegendreTable(n) for n in range(9)])
ergs, coefs = loadCoefs("O16-elastic-scattering-DA.txt")
# PDFTables = [getPDFTable(coefs[i]) for i in range(len(ergs))]
PDFTables = []
for i, energy in enumerate(ergs):
    if i == 1580:
        PDFTables.append(np.insert(getPDFTable(coefs[i]), 0, energy))
    else:
        PDFTables.append(np.insert(getPDFTable(coefs[i]), 0, energy))

np.savetxt("O16-elastic-scattering-PDF.txt", PDFTables, fmt='%.8e', delimiter=' ')
