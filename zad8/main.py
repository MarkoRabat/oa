import numpy as np
import matplotlib.pyplot as plt

opt_fun_values = []
for i in range(20):
    with open(f"file{i}.txt") as f:
        opt_fun_val = [int(i) for i in f.read().split(" ")[:-1]]
        c_vals = [opt_fun_val[0]]
        for i in range(len(opt_fun_val) - 1):
            c_vals.append(min(c_vals[-1], opt_fun_val[i + 1]))
        opt_fun_values.append(c_vals)

plt.xscale("log")
plt.yscale("log")
x = np.arange(1, 1000001)
for i in range(20):
    plt.plot(x, opt_fun_values[i])
plt.show()

c_sr_vals = np.array(opt_fun_values).cumsum(axis=0)[-1] / 20

plt.xscale("log")
plt.yscale("log")
plt.plot(x, c_sr_vals)
plt.show()



