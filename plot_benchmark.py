import matplotlib.pyplot as plt
import pickle
import numpy as np


with open('out.out', 'rb') as f:
    read = pickle.load(f)

ns, runtimes, stdevs = read
print(read)

plt.errorbar(np.log(ns), runtimes, stdevs)
plt.show()
