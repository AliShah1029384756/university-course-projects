# Operations Research Project

![Course](https://img.shields.io/badge/Course-Operations%20Research-0ea5e9)
![Languages](https://img.shields.io/badge/Languages-Python%20%7C%20C%2B%2B-16a34a)

Collection of Operations Research algorithms organized in two implementations:
- Python (interactive scripts)
- C++ (console implementations)

---

## Project Structure

- `python/or_algorithms.py`: consolidated interactive Python suite
- `cpp/or_algorithms.cpp`: consolidated interactive C++ suite

---

## Implemented Topics

1. Linear Programming (Graphical Method)
2. Simplex Method
3. Big M Method
4. Sensitivity Analysis (tableau based)
5. Duality (Primal to Dual conversion)
6. Assignment Problem (Hungarian-style reduction approach)
7. Transportation Problem:
   - Northwest Corner
   - Least Cost
   - Vogel's Approximation Method (VAM)
8. Integer Linear Programming (0/1 Branch and Bound)
9. Dynamic Programming (stage-wise shortest/longest path)
10. Knapsack:
    - 0/1 Knapsack
    - Fractional Knapsack
11. Travelling Salesman Problem (Branch and Bound)

---

## Run Instructions

### Python

```bash
cd python
python or_algorithms.py
```

### C++

```bash
cd cpp
g++ or_algorithms.cpp -o or_algorithms
./or_algorithms
```

On Windows (MinGW):

```bash
g++ or_algorithms.cpp -o or_algorithms.exe
or_algorithms.exe
```

---

## Notes

- This project is intended for learning and coursework demonstration.
- Numerical tolerance is used in several methods to reduce floating-point issues.
- Some advanced topics (full industrial-strength simplex pivot rules, full post-optimality ranges) are implemented in educational form.
