'''
Description: Check generated PDF table
Author: Ming Fang
Date: 2021-10-22 10:49:34
LastEditors: Ming Fang
LastEditTime: 2021-10-22 11:16:45
'''
import numpy as np
import matplotlib.pyplot as plt
from scipy.special import legendre
from functools import partial
plt.style.use(['science', 'high-vis', 'grid'])
plt.rcParams.update({'font.size': 18})


# from cross-section table
def angularDist(coefs, mu):
    val = 0.5
    for i, v in enumerate(coefs):
        Pi = legendre(i+1)
        val += (2*i+3) / 2 * v * Pi(mu)
    return val


# O-16
# # E = 1e-5 eV
# coefs = [-2.62229e-12]
# sigma_s = 54.1469  # total elastic scattering cross-section, barns
# E = 10 MeV
coefs = [2.832051e-1, 2.406998e-1, 2.398418e-1, 1.829444e-1, 3.803701e-2, 4.461334e-2,
             9.796117e-3, 1.003734e-3]
# sigma_s = 0.739919  # total elastic scattering cross-section, barns
f = partial(angularDist, coefs)  # normalized angular distribution, in CMS

# from PDF table
PDFs = np.loadtxt("O16-elastic-scattering-PDF.txt")

fig, ax = plt.subplots(figsize=(10, 8))
mu_cms = np.linspace(-1, 1, num=100)

ax.plot(mu_cms, f(mu_cms))
ax.step(mu_cms, PDFs[1580, 1:], where='mid')
ax.set_xlabel('mu')
ax.set_ylabel('f(mu, E)')
ax.set_ylim([0,1])
fig.savefig('O16PDF_1e7.png')
plt.show()