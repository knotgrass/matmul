import numpy as np
from jaxtyping import Float


def strassen(
    A: Float[np.ndarray, "n n"], B: Float[np.ndarray, "n n"], n0: int
) -> Float[np.ndarray, "n n"]:
    n = A.shape[0]
    if n <= n0:
        return A @ B

    h = n //2

    A11: Float[np.ndarray, "n n"] = A[:h, :h]
    A12: Float[np.ndarray, "n n"] = A[:h, h:]
    A21: Float[np.ndarray, "n n"] = A[h:, :h]
    A22: Float[np.ndarray, "n n"] = A[h:, h:]

    B11: Float[np.ndarray, "n n"] = B[:h, :h]
    B12: Float[np.ndarray, "n n"] = B[:h, h:]
    B21: Float[np.ndarray, "n n"] = B[h:, :h]
    B22: Float[np.ndarray, "n n"] = B[h:, h:]

    M1 = strassen(A11 + A22, B11 + B22, n0)
    M2 = strassen(A21 + A22, B11      , n0)
    M3 = strassen(A11      , B12 - B22, n0)
    M4 = strassen(A22      , B21 - B11, n0)
    M5 = strassen(A11 + A12, B22      , n0)
    M6 = strassen(A21 - A11, B11 + B12, n0)
    M7 = strassen(A12 - A22, B21 + B22, n0)

    C: Float[np.ndarray, "n n"] = np.empty_like(A)
    C[:h, :h] = M1 + M4 - M5 + M7 # C11
    C[:h, h:] = M3 + M5           # C12
    C[h:, :h] = M2 + M4           # C21
    C[h:, h:] = M1 - M2 + M3 + M6 # C22

    return C

def strassen_(
    A: Float[np.ndarray, "m k"],
    B: Float[np.ndarray, "k n"],
    n0: int = 4,
) -> Float[np.ndarray, "m n"]:
    m, ka = A.shape
    kb, n = B.shape
    assert ka == kb
    k = ka

    if min(m, k, n) <= n0:
        return A@B

    if m % 2 or n % 2 or k % 2:
        ...
    return



def test():
    import random
    for _ in range(100):
        n = 2 ** random.randint(1, 6) #+ 1
        A = (random.randint(1, 20) * np.random.random((n, n))).astype(np.float32)
        B = (random.randint(1, 20) * np.random.random((n, n))).astype(np.float32)

        assert np.allclose(A @ B, strassen(A, B, 1), rtol=1e-4, atol=1e-4)
        assert np.allclose(B @ A, strassen(B, A, 1), rtol=1e-4, atol=1e-4)

if __name__ == '__main__':
    test()
