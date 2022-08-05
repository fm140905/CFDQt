'''
Description: CHeck calculated invese CDF table
Author: Ming Fang
Date: 2021-10-22 14:43:22
LastEditors: Ming Fang
LastEditTime: 2021-10-22 15:30:55
'''
import numpy as np
import numpy.polynomial.legendre as L
import matplotlib.pyplot as plt
plt.style.use(['science', 'high-vis', 'grid'])
plt.rcParams.update({'font.size': 18})


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

data = np.loadtxt("O16-elastic-scattering-CDF.txt")
epsilon = -1
for j, d in enumerate(data):
    cdf = L.Legendre(CDFCoef(coefs[j]))
    for i in range(2, len(d)):
        # monotonicity
        if d[i-1] >= d[i]:
            print(j, i, d[i])
            raise ValueError("Roots are not monotonic.")
        # range
        if abs(d[i]) >= 1.001:
            print(j, i, d[i])
            raise ValueError("Roots are out of range [-1, 1].")
        # check if bins are equal-rpobable
        epsilon = max(epsilon, abs(cdf(d[i]) - (i-1)/100))
        if epsilon > 0.001:
            print(j, i, d[i])
            raise ValueError("Insufficient precision.")
print("Max error: ", epsilon)


fig, ax = plt.subplots(1,1,figsize=(10, 8))
cdf = L.Legendre(CDFCoef(coefs[1580]))
bin = data[1580, 1:]
ax.step(cdf(bin), bin, where='mid')
ax.set_ylabel('mu')
ax.set_xlabel('CDF(mu, E)')
fig.savefig('O16InverseCDF_1e7.png')
plt.show()