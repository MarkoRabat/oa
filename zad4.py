import numpy as np
import matplotlib.pyplot as plt

def calcFs(n, delta, beta, d, teta):
    sum = 0
    for k in range(n):
        sum += np.exp(complex(0, - k * (delta + beta * d * np.cos(teta))))
    return sum

def calcSin(k, delta, alpha):
    return np.sin(k * delta + alpha)

def calcCos(k, delta, alpha):
    return np.cos(k * delta + alpha)

def calcSinSum(n, delta, beta, d, teta):
    sum = 0
    for k in range(n):
        alpha = k * beta * d * np.cos(teta)
        sum += calcSin(k, delta, alpha)
    return sum

def calcCosSum(n, delta, beta, d, teta):
    sum = 0
    for k in range(n):
        alpha = k * beta * d * np.cos(teta)
        sum += calcCos(k, delta, alpha)
    return sum

def calcKSinSum(n, delta, beta, d, teta):
    sum = 0
    for k in range(n):
        alpha = k * beta * d * np.cos(teta)
        sum += k * calcSin(k, delta, alpha)
    return sum

def calcKCosSum(n, delta, beta, d, teta):
    sum = 0
    for k in range(n):
        alpha = k * beta * d * np.cos(teta)
        sum += k * calcCos(k, delta, alpha)
    return sum

def deltaDerivativeOfFs(n, delta, beta, d, teta):

    return (
            calcSinSum(n, delta, beta, d, teta) * calcKCosSum(n, delta, beta, d, teta)
            - calcCosSum(n, delta, beta, d, teta) * calcKSinSum(n, delta, beta, d, teta)
        ) / np.sqrt(
        np.square(calcCosSum(n, delta, beta, d, teta))
        + np.square(calcSinSum(n, delta, beta, d, teta))
    )

inputs = {
    "n" : 6,
    "beta" : 20 * np.pi,
    "teta" : np.pi / 3,
    "d" : 1 / 20
}

def deltaDerivativeOfFsWrapper(delta):
    return deltaDerivativeOfFs(inputs["n"], delta, inputs["beta"], inputs["d"], inputs["teta"])

def calcFsWrapper(delta):
    return calcFs(inputs["n"], delta, inputs["beta"], inputs["d"], inputs["teta"])

def bisection(lowerBound, upperBound, precision):
    while np.abs(np.abs(calcFsWrapper(lowerBound)) - np.abs(calcFsWrapper(upperBound))) > precision:
        middle = (lowerBound + upperBound) / 2
        if deltaDerivativeOfFsWrapper(lowerBound) * deltaDerivativeOfFsWrapper(middle) > 0:
            lowerBound = middle
        else: upperBound = middle
    return [ lowerBound, np.abs(calcFsWrapper(lowerBound)), ]

def graph(lowerBound, upperBound, numberOfPoints, func):
    x = np.arange(lowerBound, upperBound, (upperBound - lowerBound) / numberOfPoints)
    y = [ func(elem) for elem in x]
    # y = np.vectorize(func)(x)
    plt.scatter(x, y)
    plt.show()

graph(0, 2 * np.pi, 5000, lambda x : np.abs(calcFsWrapper(x)))
print("delta = {0}\nmax(|Fs(delta)|) = {1}".format(*bisection(4, 5.2, 10 ** -7)))


