from dataclasses import dataclass

import numpy as np
from jaxtyping import Float


@dataclass
class OpCount:
    mul: int = 0 # số phép nhân vô hướng
    add: int = 0 # số phép cộng/trừ vô hướng
    leaf: int = 0 # số lần chạm đáy đệ quy
    node: int = 0 # số lần chia 7

    @classmethod
    def naive(cls, n: int) -> "OpCount":
        """Chi phí của 1 tích naive n x n (cũng chính là 1 lá của cây đệ quy)."""
        # C[i][j] = sum_p A[i][p] * B[p][j]  -> n số hạng
        #
        #        | mỗi phần tử |  x n^2 phần tử
        #   -----+-------------+----------------
        #   mul  |      n      |  n^2 * n
        #   add  |    n - 1    |  n^2 * (n-1)
        #
        # total = 2n^3 - n^2 ~ 2n^3 FLOPs
        return cls(mul=n**3, add=n**2 * (n - 1), leaf=1, node=0)

    @property
    def total(self) -> int:
        return self.mul + self.add

    def __iadd__(self, other: "OpCount") -> "OpCount":
        """Cộng dồn tại chỗ: cnt += OpCount.naive(n)"""
        self.mul += other.mul
        self.add += other.add
        self.leaf += other.leaf
        self.node += other.node
        return self

    def __str__(self) -> str:
        return (
            f"mul={self.mul:,}  add={self.add:,}  total={self.total:,}  "
            f"(leaf={self.leaf:,}, node={self.node:,})"
        )


def strassen(
    A: Float[np.ndarray, "n n"], B: Float[np.ndarray, "n n"], n0: int, cnt: OpCount
) -> Float[np.ndarray, "n n"]:
    """Strassen đệ quy cho ma trận vuông n x n, n là luỹ thừa của 2.

    Chia đôi thành 4 block rồi tính 7 tích thay vì 8 tích như naive:

        M1 = (A11+A22)(B11+B22)        C11 = M1 + M4 - M5 + M7
        M2 = (A21+A22) B11             C12 = M3 + M5
        M3 =  A11     (B12-B22)        C21 = M2 + M4
        M4 =  A22     (B21-B11)        C22 = M1 - M2 + M3 + M6
        M5 = (A11+A12) B22
        M6 = (A21-A11)(B11+B12)
        M7 = (A12-A22)(B21+B22)

    Đổi lại phải trả thêm 18 phép +/- trên block ở mỗi tầng (10 để dựng toán hạng,
    8 để ghép C), tức T(n) = 7*T(n/2) + 18*(n/2)^2 -> O(n^log2(7)) = O(n^2.807).

    Cái giá thứ hai, không thấy trong bộ đếm: Strassen KÉM ỔN ĐỊNH SỐ hơn naive. Các
    phép trừ block (A21-A11, B12-B22, ...) gây triệt tiêu, sai số nhân lên qua từng
    tầng. Đo với float32 so ground-truth float64, sai số lớn nhất:

        n=128, n0=1 -> tệ hơn naive ~94 lần
        n=256, n0=1 -> tệ hơn naive ~98 lần
        n=256, n0=8 -> tệ hơn naive ~11 lần

    Đây là lý do BLAS thật không dùng Strassen thuần, và là lý do thứ ba (cùng số phép
    tính và tốc độ) để n0 không nên để bằng 1.

    n0  ngưỡng dừng, từ đó dùng thẳng A @ B. Vì mỗi tầng chỉ chia đôi được nên kích
        thước lá thực tế là luỹ thừa 2 lớn nhất <= n0 (n0=3 chạy y hệt n0=2).
    cnt bộ đếm, bị sửa tại chỗ (in-place).
    """
    n = A.shape[0]
    if n <= n0:
        cnt += OpCount.naive(n)  # n^3 mul + n^2(n-1) add, leaf += 1
        return A @ B

    cnt.node += 1
    h = n // 2

    A11: Float[np.ndarray, "h h"] = A[:h, :h]
    A12: Float[np.ndarray, "h h"] = A[:h, h:]
    A21: Float[np.ndarray, "h h"] = A[h:, :h]
    A22: Float[np.ndarray, "h h"] = A[h:, h:]

    B11: Float[np.ndarray, "h h"] = B[:h, :h]
    B12: Float[np.ndarray, "h h"] = B[:h, h:]
    B21: Float[np.ndarray, "h h"] = B[h:, :h]
    B22: Float[np.ndarray, "h h"] = B[h:, h:]

    cnt.add += 10 * h**2  # 10 phép +/- trên block để dựng 7 cặp toán hạng
    M1: Float[np.ndarray, "h h"] = strassen(A11 + A22, B11 + B22, n0, cnt)
    M2: Float[np.ndarray, "h h"] = strassen(A21 + A22, B11      , n0, cnt)
    M3: Float[np.ndarray, "h h"] = strassen(A11      , B12 - B22, n0, cnt)
    M4: Float[np.ndarray, "h h"] = strassen(A22      , B21 - B11, n0, cnt)
    M5: Float[np.ndarray, "h h"] = strassen(A11 + A12, B22      , n0, cnt)
    M6: Float[np.ndarray, "h h"] = strassen(A21 - A11, B11 + B12, n0, cnt)
    M7: Float[np.ndarray, "h h"] = strassen(A12 - A22, B21 + B22, n0, cnt)

    cnt.add += 8 * h**2  # 3 + 1 + 1 + 3 phép +/- để ghép 4 block của C
    C: Float[np.ndarray, "n n"] = np.empty_like(A, dtype=A.dtype)
    C[:h, :h] = M1 + M4 - M5 + M7 # C11
    C[:h, h:] = M3 + M5           # C12
    C[h:, :h] = M2 + M4           # C21
    C[h:, h:] = M1 - M2 + M3 + M6 # C22

    return C


def matmul(
    A: Float[np.ndarray, "n n"],
    B: Float[np.ndarray, "n n"],
    n0: int = 1,
    cnt: OpCount | None = None,
) -> tuple[Float[np.ndarray, "n n"], OpCount]:
    """Nhân A @ B và trả kèm bộ đếm.

    cnt=None -> tạo mới, chỉ đếm riêng lời gọi này.
    Truyền sẵn 1 OpCount vào nếu muốn cộng dồn qua nhiều lời gọi.
    """
    n = A.shape[0]
    assert A.shape == B.shape == (n, n), "chỉ nhận ma trận vuông cùng kích thước"
    assert n > 0 and n & (n - 1) == 0, f"n phải là luỹ thừa của 2, nhận được n={n}"

    if cnt is None:
        cnt = OpCount()
    return strassen(A, B, n0, cnt), cnt


def theoretical(n: int, n0: int = 1) -> OpCount:
    """Công thức đóng cho số phép tính mà strassen() ở trên sẽ đếm được.

    n là luỹ thừa của 2. Đệ quy dừng khi size <= n0, tức sau k tầng chia đôi
    (k = số tầng đệ quy: 1 tầng -> 7 lời gọi, 2 tầng -> 7*7, ...), lá có size p:

        k = max(0, log2(n) - floor(log2(n0)))        p = n / 2^k

    Cây có 7^k lá và 1 + 7 + ... + 7^(k-1) = (7^k - 1)/6 node trong.

        mul = 7^k * p^3

        add = 7^k * p^2*(p-1)                    <- phần naive ở mỗi lá
            + sum_{i<k} 7^i * 18*(n/2^(i+1))^2   <- 18h^2 ở mỗi node trong

    Tổng cấp số nhân với công bội 7/4:

        sum_{i<k} 7^i * 18*(n/2^(i+1))^2 = 18n^2/4 * ((7/4)^k - 1)/(3/4)
                                         = 6n^2 * ((7/4)^k - 1)
                                         = 6 * (7^k * p^2 - n^2)     [vì n^2/4^k = p^2]
    """
    # n không phải luỹ thừa của 2 thì công thức vẫn ra số, nhưng là số vô nghĩa
    # (theoretical(6) trả kết quả của n=4) trong khi strassen(6x6) thì crash -> chặn luôn.
    assert n > 0 and n & (n - 1) == 0, f"n phải là luỹ thừa của 2, nhận được n={n}"
    assert n0 > 0

    # x.bit_length() - 1 == floor(log2(x)) for x >= 1, e.g. 8 (0b1000) -> 4 bits -> 3.
    # Integer-exact; math.floor(math.log2(x)) starts rounding up wrong around 2**49.
    k = max(0, (n.bit_length() - 1) - (n0.bit_length() - 1))
    p = n >> k    # n // 2**k
    pow7 = 7**k

    return OpCount(
        mul=pow7 * p**3,
        add=pow7 * p**2 * (p - 1) + 6 * (pow7 * p**2 - n**2),
        leaf=pow7,
        node=(pow7 - 1) // 6,
    )


def test():
    rng = np.random.default_rng(0)

    for e in range(1, 8):
        n = 2**e # size of square matrix
        # 3 và 6 không phải luỹ thừa của 2 -> chốt hành vi floor(log2)
        for n0 in (1, 2, 3, 4, 6, 16):
            if n0 > n:
                continue
            A = rng.random((n, n), dtype=np.float32) * 20
            B = rng.random((n, n), dtype=np.float32) * 20

            C, cnt = matmul(A, B, n0)
            assert np.allclose(A @ B, C, rtol=1e-4, atol=1e-4), f"sai ở {n=} {n0=}"
            assert cnt == theoretical(n, n0), f"đếm lệch ở {n=} {n0=}: {cnt} != {theoretical(n, n0)}"

    print("OK: kết quả và bộ đếm khớp lý thuyết\n")

    print(f"{'n':>5} {'n0':>3} | {'strassen mul':>14} {'naive mul':>14} {'tỉ lệ':>7} "
          f"| {'strassen ops':>14} {'naive ops':>14} {'tỉ lệ':>7}")
    print("-" * 100)
    for e in range(1, 11):
        n = 2**e # size of square matrix
        for n0 in (1, 8):
            if n0 > n:
                continue
            s, nv = theoretical(n, n0), OpCount.naive(n)
            print(f"{n:>5} {n0:>3} | {s.mul:>14,} {nv.mul:>14,} {nv.mul / s.mul:>7.2f} "
                  f"| {s.total:>14,} {nv.total:>14,} {nv.total / s.total:>7.2f}")


if __name__ == "__main__":
    test()
