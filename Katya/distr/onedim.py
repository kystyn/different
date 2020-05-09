def dichotomy(function, left_bound, right_bound, sigma, epsilon):
    """
    Dichotomy optimization method
    """

    precision = 0.5 * (right_bound - left_bound)

    while precision > epsilon:
        center = .5 * (left_bound + right_bound)
        x1 = center - sigma
        x2 = center + sigma

        f1 = function(x1)
        f2 = function(x2)

        if f1 <= f2:
            right_bound = x2
        else:
            left_bound = x1

        precision = 0.5 * (right_bound - left_bound)

    x = 0.5 * (right_bound + left_bound)
    return x


def fibonacci(func, left_bound, right_bound, epsilon):
    F = [1, 1]
    d = (right_bound - left_bound) / epsilon

    while F[-1] <= d:
        F.append(F[-1] + F[-2])
    n = len(F)

    lambd = left_bound + (right_bound - left_bound) * F[-1 - 2] / F[-1]
    mu = left_bound + (right_bound - left_bound) * F[-1 - 1] / F[-1]

    # main step
    for k in range(1, n - 2):

        f_lambd = func(lambd)
        f_mu = func(mu)
        if f_lambd > f_mu:
            left_bound = lambd
            lambd = mu
            mu = left_bound + (right_bound - left_bound) * F[-1 - k - 1] / F[-1 - k]
        else:
            right_bound = mu
            mu = lambd
            lambd = left_bound + (right_bound - left_bound) * F[-1 - k - 2] / F[-1 - k]

    x_solution = (left_bound + right_bound) / 2
    return x_solution
