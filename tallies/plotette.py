import serpentTools
from serpentTools.settings import rc

rc["serpentVersion"] = "2.1.32"

import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator   # ⭐ add this
from cycler import cycler

og_matlab_colors = [(0, 0, 1),(0, 1, 0),(1, 0, 0),(0, 1, 1),(1, 0, 1),(1, 1, 0),(0, 0, 0)]
plt.rcParams.update({"axes.prop_cycle": cycler(color=og_matlab_colors),"lines.linewidth": 1.5,"lines.linestyle": "-","axes.grid": False,"font.size": 12,})

import numpy as np
import math


det = serpentTools.read("../serpent/input_k_det0.m")

# d238 = det.detectors["dabs_u238"]
# E = d238.grids["E"] * 1e6  
# edges = np.r_[E[:,0], E[-1,1]]

# d238 = d238.tallies.ravel()

# plt.step(edges, np.r_[d238[0], d238], where="post", lw=1.8, label="Fuel")


# plt.xscale("log")
# # plt.yscale("log")  # optional

# plt.xlabel("Energy (MeV)")
# plt.ylabel(r"fission rate$")

# plt.legend()

d = det.detectors["delastic_o"]
E = d.grids["E"] * 1e6  
edges = np.r_[E[:,0], E[-1,1]]

d = d.tallies.ravel()

plt.step(edges, np.r_[d[0], d], where="post", lw=1.8, label="Fuel")


plt.xscale("log")
# plt.yscale("log")  # optional

plt.xlabel("Energy (MeV)")
plt.ylabel(r"$\hat{t}$")

plt.legend()


plt.show()
