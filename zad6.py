import numpy as np
import scipy

# x = [x11, x12, x13, x14, x21, x22, x23, x24, x31, x32, x33, x34, x41, x42, x43, x44, x51, x52, x53, x54]

A_ub = np.array([
    [1] * 4 + [0] * 16,
    [0] * 4 + [1] * 4 + [0] * 12,
    [0] * 8 + [1] * 4 + [0] * 8,
    [0] * 12 + [1] * 4 + [0] * 4,
    [0] * 16 + [1] * 4,
])

b_ub = np.array([500, 300, 700, 250, 750])

A_eq = np.array([
    [1 if i % 4 == 0 else 0 for i in range(20)],
    [0] + [1 if i % 4 == 0 else 0 for i in range(19)],
    [0, 0] + [1 if i % 4 == 0 else 0 for i in range(18)],
    [0, 0, 0] + [1 if i % 4 == 0 else 0 for i in range(17)],
])

b_eq = np.array([1000, 500, 200, 300])

c = np.array([10, 20, 40, 60,
              30, 30, 10, 15,
              50, 35, 20, 30,
              70, 45, 15, 20,
              20, 65, 30, 60])

res = scipy.optimize.linprog(c=c, A_ub=A_ub, b_ub=b_ub, A_eq=A_eq, b_eq=b_eq, method="simplex")

print("optimalno resenje: {0}".format(res.fun/10))
print("x = (", end="")
for index, elem in enumerate(res.x):
    print("{0}{1}".format(elem, ", " if index != 19 else ")"), end=" ")
