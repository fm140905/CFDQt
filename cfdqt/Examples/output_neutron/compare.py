import numpy as np
import matplotlib.pyplot as plt
import glob
plt.rcParams.update({'font.size': 18})


def readOutp(path):
    tmparray = []
    lino = 0
    flag = False
    with open(path) as f:
        for line in f:
            lino = lino + 1
            if not flag:
                if line[:12] == "      energy":
                    # print("First line: ", lino + 1)
                    flag = True
                continue
            if flag and line[:11] == "      total":
                # print("Last line: ", lino - 1)
                flag = False
                break
            tmp = line.strip()
            tmp = tmp.split()
            tmparray.append([float(x) for x in tmp])
    # reshape
    tmparray = np.array(tmparray)
    return np.transpose(tmparray)


fig, ax = plt.subplots(1, 1, figsize=(6, 6))

fpath = 'outq'
[ergbins, counts, errors] = readOutp(fpath)
lethargy = np.log(ergbins)
d_lethargy = lethargy[1:]-lethargy[:-1]
ax.step(ergbins[1:], counts[1:] / d_lethargy, label='MCNP, NPS = 1E8', linewidth=4, alpha=0.5, where='pre')

cfdpath = "tally.txt"
CFDdist = np.loadtxt(cfdpath, delimiter='\t')
CFDdist[:, 0] = CFDdist[:, 0] / 1e6
ax.step(ergbins[1:], CFDdist[:100, 1] / d_lethargy, label='CFD, NPS = 1E6', linewidth=4, alpha=0.5, where='pre')

ax.set_xlabel('Energy (MeV)')
ax.set_ylabel('Fluence per unit NPS per unit lethargy')
ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlim((1e-9, 20))
ax.legend()

# fig.savefig('F4_comparison.png',bbox_inches='tight',dpi=fig.dpi)
plt.show()
