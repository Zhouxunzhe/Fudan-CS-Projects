from scipy.optimize import newton, fsolve
import numpy as np


def newton_method(f, df, x0, tol=1e-6, max_iter=1000):
    x = x0
    for i in range(max_iter):
        x_next = x - f(x) / df(x)
        if abs(x_next - x) < tol:
            return x_next
        x = x_next
    raise ValueError("cannot converge")


def f(x):
    return x + np.cos(x)


def df(x):
    return 1 - np.sin(x)


# 调用牛顿迭代法求解方程的根
my_newton_sol = newton_method(f, df, 0)
newton_sol = newton(f, 0)
fsolve_sol = fsolve(f, np.array([0]))

print("my newton Solution:", my_newton_sol)
print("scipy.newton Solution:", newton_sol)
print("scipy.fsolve Solution:", fsolve_sol[0])
