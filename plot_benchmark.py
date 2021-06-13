import matplotlib.pyplot as plt
import pickle
import numpy as np
import scipy.stats

with open('out.out', 'rb') as f:
    read = pickle.load(f)

ns, runtimes, stdevs = read
runtimes = np.array(runtimes)/10
print(read)

x = np.log(ns)
m,b = np.polyfit(x, runtimes, 1)
slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(x, runtimes)
print("r**2 is", r_value**2)
print('slope and intercept are', m, b)
diff_squared = []

for i, elt in enumerate(x):
    diff_squared.append((runtimes[i]-(m*elt + b))**2/(stdevs[i]/np.sqrt(10)/10)**2)
print("chi squared is", sum(diff_squared))


plt.errorbar(x, runtimes, [s/np.sqrt(10)/10 for s in stdevs], fmt="b.")
plt.plot(x, m*x+b, 'r')
plt.title(r"$\log(n)$ vs average runtime per operation on size n ETT")
plt.xlabel(r'$\log(n)$')

plt.ylabel('Runtimes ($\mu$s)')
plt.show()
