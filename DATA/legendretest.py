'''
Description: 
Author: Ming Fang
Date: 1969-12-31 18:00:00
LastEditors: Ming Fang
LastEditTime: 2021-10-22 12:28:38
'''
import numpy as np
import numpy.polynomial.legendre as L
import matplotlib.pyplot as plt
plt.style.use(['science', 'high-vis', 'grid'])
plt.rcParams.update({'font.size': 18})

fig, ax= plt.subplots(1, 2, figsize=(12, 6))
for i in np.arange(4):
    coef = np.zeros(i+1)
    coef[-1] = 1
    f = L.Legendre(coef)
    ax[0].plot(np.linspace(-1,1), f(np.linspace(-1,1)))

    coefg = L.legint(coef, lbnd=-1)
    g = L.Legendre(coefg)
    ax[1].plot(np.linspace(-1,1), g(np.linspace(-1,1)))

plt.show()