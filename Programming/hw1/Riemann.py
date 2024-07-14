import numpy as np


def zeta(s, N=1000):
    """Compute the Riemann zeta function"""
    return np.sum(1 / (np.arange(1, N + 1) ** s))


def zeta_taylor(s, terms=10):
    """Compute the first terms of the Taylor series expansion of the Riemann zeta function"""
    coeffs = [1 / (k ** s) for k in range(1, terms + 1)]
    return np.poly1d(coeffs[::-1])


def find_zeros(N_zeros, terms=10):
    """Find the first N_zeros zeros of the Riemann zeta function"""
    zeros = []
    taylor = zeta_taylor(0)  # Taylor series around s=0
    for _ in range(N_zeros):
        # Use roots() to find zeros of the Taylor series
        zero = np.roots(taylor)
        zeros.append(zero)
        taylor = np.polyder(taylor)  # Update Taylor series for next zero
    return zeros


# Find the first 5 zeros of the Riemann zeta function
zeros = find_zeros(5)
for i, zero in enumerate(zeros, 1):
    print(f"Zero {i}: {zero}, Imaginary part: {zero.imag}")
