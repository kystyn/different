# collection of one-dimension optimization methods
from math import sqrt
from onedim import fibonacci

from scipy.linalg import ldl, solve_triangular
import numpy as np


def dot(u, v):
    assert len(u) == len(v)
    return sum(u[i] * v[i] for i in range(len(u)))


def norm2(x):
    return dot(x, x)


def newton(f, gradient, hessian,
           alpha0: float,
           gradient_precision: float,
           epsilon: float):
    dim = len(gradient)
    x = [10] * dim

    while norm2(gradient(x)) > gradient_precision:
        L, D, perm = ldl(hessian(x))

        z = solve_triangular(L, -gradient(x), lower=True)
        y = solve_triangular(D, z)
        d = solve_triangular(L.transpose(), y)

        alpha = alpha0
        while f(x + alpha * d) - f(x) > epsilon * alpha * np.dot(gradient, d):
            alpha = alpha / 2

        x = x + alpha * d

    return list(x), f(x)

def optimal_gradient(function, gradient, point, delta):
    # Method begin
    grad = np.array(gradient(point))
    while norm2(grad) >= delta:
        l = [0]
        #alpha = fibonacci(lambda a: function(point - a * grad), 0, 1e9, delta)
        alpha = 20
        while function(point - alpha * grad) - function(point) > 0.5 * alpha * norm2(gradient(point)):
            alpha = alpha / 2
        newPoint = point - alpha * grad

        point = newPoint

        new_grad = np.array(gradient(point))

        grad = new_grad
    # Method end

    return point, function(point)
