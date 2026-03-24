import matplotlib.pyplot as plt
import numpy as np
import h5py as h5

temp = 900
files = ["U235.h5", "U238.h5", "O16.h5", "H1.h5"]

def pad_xs(xs, n):
    out = np.zeros(n)
    out[:len(xs)] = xs
    return out

def write_binary(filename, energy, total, elastic, capture, fission):
    energy = np.asarray(energy, dtype=np.float64)
    total = np.asarray(total, dtype=np.float64)
    elastic = np.asarray(elastic, dtype=np.float64)
    capture = np.asarray(capture, dtype=np.float64)
    fission = np.asarray(fission, dtype=np.float64)

    nE = np.array([len(energy)], dtype=np.uint64)

    with open("bin/" + filename, "wb") as f:
        nE.tofile(f)
        energy.tofile(f)
        total.tofile(f)
        elastic.tofile(f)
        capture.tofile(f)
        fission.tofile(f)

data = {}

for file in files:
    name = file.replace(".h5", "")

    with h5.File("hdf5/"+file, "r") as f:
        nuc = f[name]

        energy = np.array(nuc["energy"][f"{temp}K"])
        nE = len(energy)

        elastic = np.zeros(nE)
        capture = np.zeros(nE)
        fission = np.zeros(nE)

        if "reaction_002" in nuc["reactions"]:
            xs = np.array(nuc["reactions"]["reaction_002"][f"{temp}K"]["xs"])
            elastic = pad_xs(xs, nE)

        if "reaction_102" in nuc["reactions"]:
            xs = np.array(nuc["reactions"]["reaction_102"][f"{temp}K"]["xs"])
            capture = pad_xs(xs, nE)

        if "reaction_018" in nuc["reactions"]:
            xs = np.array(nuc["reactions"]["reaction_018"][f"{temp}K"]["xs"])
            fission = pad_xs(xs, nE)

        total = elastic + capture + fission

        data[f"{name}_energy"] = energy
        data[f"{name}_total"] = total
        data[f"{name}_mt_2"] = elastic
        data[f"{name}_mt_102"] = capture
        data[f"{name}_mt_18"] = fission

        write_binary(
            f"{name}.bin",
            energy,
            total,
            elastic,
            capture,
            fission
        )


        #print(list(nuc.keys()))
        print(list(nuc["reactions"]["reaction_002"]["product_0"]["distribution_0"]["angle"]["mu"]))
        #print(f"Wrote {name}.bin with {nE} points")

