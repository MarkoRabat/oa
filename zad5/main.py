import random
import scipy
import matplotlib.pyplot as plt
import numpy as np

def hiddenActivation(x):
    return 1 / (1 + np.exp(-x))

def yOut(x, weights):
    activations = []
    for i in range(5):
        activations.append(hiddenActivation(weights[i] * x + weights[i + 5]))
    rez = 0
    for i in range(5):
        rez += weights[i + 10] * activations[i]
    return rez + weights[15]

def error(training_dataX, weights, training_dataY):
    sum = 0
    for i in range(len(training_dataX)):
        sum += np.square(yOut(training_dataX[i], weights) - training_dataY[i])
    return sum / 51

weight_lower = -30
weight_upper = 30
weights = np.array([random.uniform(weight_lower, weight_upper) for i in range(16)])

training_dataX = []
training_dataY = []
with open("data.txt", "r") as file:
    for line in file.readlines():
        x, y = line.split(" ")
        training_dataX.append(float(x))
        training_dataY.append(float(y))

def weightError(input):
    training_dataX = []
    training_dataY = []
    with open("data.txt", "r") as file:
        for line in file.readlines():
            x, y = line.split(" ")
            training_dataX.append(float(x))
            training_dataY.append(float(y))
    return error(training_dataX, input, training_dataY)

print("error:", weightError(weights))
for i in range(10):
    rezult = scipy.optimize.minimize(fun=weightError, x0=weights, method="Nelder-Mead", bounds=tuple([(weight_lower, weight_upper) for i in range(len(weights))]))
    weights = rezult.x
    print("[ ", end="\n")
    i = 0
    for weight in weights:
        print("{0:.15f} ".format(weight), end="")
        i += 1
        if i % 4 == 0:
            print()
            i = 0
    print("]\nerror:{0:.15f}".format(weightError(weights)))


prediction2 = []
for i in range(len(training_dataX)):
    prediction2.append(yOut(training_dataX[i], weights))
plt.plot(training_dataX, prediction2)
plt.plot(training_dataX, training_dataY)
plt.show()