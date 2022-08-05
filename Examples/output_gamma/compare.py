import numpy as np
import matplotlib.pyplot as plt
import glob
plt.rcParams.update({'font.size': 18})


def readOutp(path):
    startLine = 622
    endLine = 721
    tmparray = []
    lino = 1
    with open(path) as f:
        while lino < startLine:
            next(f)
            lino = lino + 1
        for line in f:
            if lino > endLine:
                break
            lino = lino + 1
            tmp = line.strip()
            tmp = tmp.split()
            tmparray.append([float(x) for x in tmp])
    # reshape
    tmparray = np.array(tmparray)
    return np.transpose(tmparray)


fig, ax = plt.subplots(1, 1, figsize=(6, 6))
CFDdist = np.loadtxt('tally.txt', delimiter='\t')
for i in np.arange(len(CFDdist)):
    if CFDdist[i, 0] <= 0.1:
        CFDdist[i, 1] = 0
ax.step(CFDdist[:, 0], CFDdist[:, 1], label='CFD, NPS = 1E7', linewidth=4, alpha=0.5, where='mid')

fpath = 'outq'
if glob.glob(fpath):
    fpath = (glob.glob(fpath))[0]
    # print(fpath)
    [ergbins, counts, errors] = readOutp(fpath)
    ergbins = ergbins - 0.005
    counts[ergbins<=0.1] = 0
    ax.step(ergbins, counts, label='MCNP, NPS = 1E8', linewidth=4, alpha=0.5, where='mid')
else:
    print(fpath + ' not found')

ax.set_xlabel('Energy (MeV)')
ax.set_ylabel('Fluence per unit NPS')
ax.set_yscale('log')
ax.set_xlim((0.0, 0.7))
ax.set_ylim((1e-7, 1e-4))
ax.legend()

# fig.savefig('F4_comparison.png',bbox_inches='tight',dpi=fig.dpi)
plt.show()
