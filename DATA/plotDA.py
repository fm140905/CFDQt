'''
Author: your name
Date: 1969-12-31 18:00:00
LastEditTime: 2021-10-20 23:56:28
LastEditors: Please set LastEditors
Description: Plot angular distribution
FilePath: /DATA/plotSA.py
'''
import numpy as np
import matplotlib.pyplot as plt
from scipy.special import legendre
from functools import partial
plt.style.use(['science', 'high-vis', 'grid'])
plt.rcParams.update({'font.size': 18})


def angularDist(coefs, mu):
    val = 0.5
    for i, v in enumerate(coefs):
        Pi = legendre(i+1)
        val += (2*i+3) / 2 * v * Pi(mu)
    return val


# H-1
A = 1
# # E = 1e-5 eV
# coefs = [-1.16740e-14, -1.01123e-16, -1.82863e-17, 3.03417e-17, -3.10313e-18, 3.93859e-18]
# sigma_s = 1159.66  # total elastic scattering cross-section, barns
# E = 1 MeV
coefs = [-5.949241e-4,-1.378120e-5,-1.739117e-7, 8.26192e-10, 2.33759e-12,-1.08658e-13]
sigma_s = 4.24951
# # E = 10 MeV
# coefs = [-1.064356e-2, 1.473377e-4, -1.442281e-4, 2.423436e-5, 5.413201e-7, 8.242175e-8]
# sigma_s = 0.936877  # total elastic scattering cross-section, barns

# # O-16
# A = 16
# # E = 1e-5 eV
# coefs = [-2.62229e-12]
# sigma_s = 54.1469  # total elastic scattering cross-section, barns
# # # E = 10 MeV
# # coefs = [2.832051e-1, 2.406998e-1, 2.398418e-1, 1.829444e-1, 3.803701e-2, 4.461334e-2,
# #              9.796117e-3, 1.003734e-3]
# # sigma_s = 0.739919  # total elastic scattering cross-section, barns

f = partial(angularDist, coefs)  # normalized angular distribution, in CMS

# in lab system
fig, ax = plt.subplots(figsize=(10, 8))
mu_cms = np.linspace(-1, 1, num=50)
mu_labs = []
dsigma_labs = []  # differential cross-section, lab system, barn / sr
for mu_cm in mu_cms:
    E_cm = (A/(A+1))**2
    E_lab = (1+A**2+2*A*mu_cm) / (A+1)**2
    mu_lab = mu_cm * np.sqrt(E_cm / E_lab) + 1/(A+1) * np.sqrt(1/E_lab)
    dmu_cm_over_dmu_lab = np.sqrt(E_lab / E_cm) / (1-mu_lab / (A+1) * np.sqrt(1/E_lab))
    dsigma_lab = sigma_s / (2*np.pi) * f(mu_cm) * dmu_cm_over_dmu_lab
    mu_labs.append(mu_lab)
    dsigma_labs.append(dsigma_lab)

ax.plot(np.arccos(mu_labs) / np.pi * 180, dsigma_labs)
xs = np.linspace(0, 1, num=50)
ax.plot(xs * 90, sigma_s / np.pi * np.cos(xs * np.pi / 2))
ax.set_xlabel('theta (degree)')
ax.set_ylabel('differential cross-section (barns / sr)')
fig.savefig('H1DA_1e6.png')
# plt.show()