import math

EPS = 1e-9


def read_floats(prompt=""):
    if prompt:
        print(prompt)
    return list(map(float, input().split()))


def lp_graphical():
    print("=" * 60)
    print("LINEAR PROGRAMMING - GRAPHICAL METHOD")
    print("=" * 60)

    obj_type = input("Enter objective (max/min): ").strip().lower()
    c1 = float(input("Enter coefficient of x1: "))
    c2 = float(input("Enter coefficient of x2: "))

    m = int(input("Enter number of constraints: "))
    constraints = []
    print("Enter each constraint in format: a1 a2 <= b")
    for i in range(m):
        t = input(f"Constraint {i + 1}: ").split()
        constraints.append((float(t[0]), float(t[1]), t[2], float(t[3])))

    points = [(0.0, 0.0)]

    for a1, a2, op, b in constraints:
        if abs(a1) > EPS:
            x = b / a1
            if x >= -EPS:
                points.append((max(0.0, x), 0.0))
        if abs(a2) > EPS:
            y = b / a2
            if y >= -EPS:
                points.append((0.0, max(0.0, y)))

    for i in range(m):
        for j in range(i + 1, m):
            a1, a2, _, b1 = constraints[i]
            c1_, c2_, _, b2 = constraints[j]
            det = a1 * c2_ - c1_ * a2
            if abs(det) > EPS:
                x = (b1 * c2_ - b2 * a2) / det
                y = (a1 * b2 - c1_ * b1) / det
                if x >= -EPS and y >= -EPS:
                    points.append((max(0.0, x), max(0.0, y)))

    feasible = []
    for x, y in points:
        ok = True
        for a1, a2, op, b in constraints:
            v = a1 * x + a2 * y
            if op == "<=" and v > b + 1e-6:
                ok = False
            elif op == ">=" and v < b - 1e-6:
                ok = False
            elif op in ("=", "==") and abs(v - b) > 1e-6:
                ok = False
        if ok:
            feasible.append((x, y))

    unique = []
    for p in feasible:
        if not any(abs(p[0] - q[0]) < 1e-6 and abs(p[1] - q[1]) < 1e-6 for q in unique):
            unique.append(p)

    if not unique:
        print("No feasible corner points found.")
        return

    best = None
    for x, y in unique:
        z = c1 * x + c2 * y
        print(f"Point ({x:.4f}, {y:.4f}) => Z = {z:.4f}")
        if best is None:
            best = (x, y, z)
        elif (obj_type == "max" and z > best[2]) or (obj_type == "min" and z < best[2]):
            best = (x, y, z)

    print("\nOptimal Solution")
    print(f"x1 = {best[0]:.4f}, x2 = {best[1]:.4f}, Z = {best[2]:.4f}")


def simplex_max():
    print("=" * 60)
    print("SIMPLEX METHOD - MAXIMIZATION")
    print("=" * 60)
    n = int(input("Enter number of decision variables: "))
    m = int(input("Enter number of constraints: "))

    print("Enter objective coefficients:")
    c = list(map(float, input().split()))

    A = []
    b = []
    print("Enter each constraint as: a1 a2 ... b")
    for i in range(m):
        row = list(map(float, input(f"Constraint {i + 1}: ").split()))
        A.append(row[:-1])
        b.append(row[-1])

    tab = []
    for i in range(m):
        row = A[i] + [0.0] * m
        row[n + i] = 1.0
        row.append(b[i])
        tab.append(row)
    tab.append([-x for x in c] + [0.0] * m + [0.0])

    total = n + m
    basis = [n + i for i in range(m)]

    while True:
        obj = tab[-1]
        pivot_col = min(range(total), key=lambda j: obj[j])
        if obj[pivot_col] >= -1e-10:
            break

        ratios = []
        for i in range(m):
            if tab[i][pivot_col] > 1e-10:
                ratios.append((tab[i][-1] / tab[i][pivot_col], i))
        if not ratios:
            print("Unbounded solution.")
            return

        _, pivot_row = min(ratios)
        basis[pivot_row] = pivot_col

        piv = tab[pivot_row][pivot_col]
        tab[pivot_row] = [v / piv for v in tab[pivot_row]]

        for i in range(m + 1):
            if i == pivot_row:
                continue
            factor = tab[i][pivot_col]
            tab[i] = [tab[i][j] - factor * tab[pivot_row][j] for j in range(len(tab[i]))]

    sol = [0.0] * n
    for i, bv in enumerate(basis):
        if bv < n:
            sol[bv] = tab[i][-1]

    print("Optimal solution:")
    for i in range(n):
        print(f"x{i + 1} = {sol[i]:.4f}")
    print(f"Maximum Z = {tab[-1][-1]:.4f}")


def big_m_method():
    print("=" * 60)
    print("BIG M METHOD")
    print("=" * 60)
    print("Educational implementation: handles <=, >=, == constraints.")
    print("Use M = 10000.")

    n = int(input("Enter number of decision variables: "))
    m = int(input("Enter number of constraints: "))
    obj_type = input("Enter objective (max/min): ").strip().lower()
    c = list(map(float, input("Enter objective coefficients: ").split()))

    constraints = []
    for i in range(m):
        line = input(f"Constraint {i + 1} (a1 a2 ... op b): ").split()
        coeffs = list(map(float, line[:-2]))
        op = line[-2]
        rhs = float(line[-1])
        constraints.append((coeffs, op, rhs))

    M = 10000.0
    num_slack = sum(1 for _, op, _ in constraints if op == "<=")
    num_surplus = sum(1 for _, op, _ in constraints if op == ">=")
    num_art = sum(1 for _, op, _ in constraints if op in (">=", "==", "="))

    cols = n + num_slack + num_surplus + num_art + 1
    tab = []
    basis = []
    art_idx = []
    var = n

    for coeffs, op, rhs in constraints:
        row = coeffs + [0.0] * (cols - n - 1)
        if op == "<=":
            row[var] = 1.0
            basis.append(var)
            var += 1
        elif op == ">=":
            row[var] = -1.0
            var += 1
            row[var] = 1.0
            basis.append(var)
            art_idx.append(var)
            var += 1
        else:
            row[var] = 1.0
            basis.append(var)
            art_idx.append(var)
            var += 1
        row[-1] = rhs
        tab.append(row)

    obj = ([-x for x in c] if obj_type == "max" else c[:]) + [0.0] * (cols - n - 1)
    for a in art_idx:
        obj[a] = M if obj_type == "max" else -M
    tab.append(obj)

    for i, bv in enumerate(basis):
        if bv in art_idx:
            factor = tab[-1][bv]
            tab[-1] = [tab[-1][j] - factor * tab[i][j] for j in range(cols)]

    while True:
        obj_row = tab[-1]
        if obj_type == "max":
            pivot_col = min(range(cols - 1), key=lambda j: obj_row[j])
            if obj_row[pivot_col] >= -1e-6:
                break
        else:
            pivot_col = max(range(cols - 1), key=lambda j: obj_row[j])
            if obj_row[pivot_col] <= 1e-6:
                break

        ratios = []
        for i in range(m):
            if tab[i][pivot_col] > 1e-10:
                ratios.append((tab[i][-1] / tab[i][pivot_col], i))
        if not ratios:
            print("Unbounded solution.")
            return

        _, pr = min(ratios)
        basis[pr] = pivot_col
        pv = tab[pr][pivot_col]
        tab[pr] = [x / pv for x in tab[pr]]

        for i in range(m + 1):
            if i == pr:
                continue
            f = tab[i][pivot_col]
            tab[i] = [tab[i][j] - f * tab[pr][j] for j in range(cols)]

    sol = [0.0] * n
    for i, bv in enumerate(basis):
        if bv < n:
            sol[bv] = tab[i][-1]

    print("Optimal solution:")
    for i, x in enumerate(sol, 1):
        print(f"x{i} = {x:.4f}")
    val = -tab[-1][-1] if obj_type == "max" else tab[-1][-1]
    print(f"Optimal Z = {val:.4f}")


def sensitivity_analysis():
    print("=" * 60)
    print("SENSITIVITY ANALYSIS (TABLEAU-BASED)")
    print("=" * 60)

    n = int(input("Enter number of decision variables: "))
    m = int(input("Enter number of constraints: "))
    print(f"Enter {m + 1} rows of final tableau:")
    tab = [list(map(float, input().split())) for _ in range(m + 1)]

    basis = [x - 1 for x in map(int, input("Enter basic variable indices (1-based): ").split())]

    print("Shadow prices:")
    for i in range(m):
        col = n + i
        shadow = -tab[-1][col] if col < len(tab[-1]) - 1 else 0.0
        print(f"Constraint {i + 1}: {shadow:.4f}")

    print("Reduced costs:")
    for j in range(n):
        if j in basis:
            print(f"x{j + 1}: basic variable")
        else:
            print(f"x{j + 1}: reduced cost = {tab[-1][j]:.4f}")


def duality_converter():
    print("=" * 60)
    print("DUALITY - PRIMAL TO DUAL")
    print("=" * 60)

    obj = input("Primal objective (max/min): ").strip().lower()
    n = int(input("Number of primal variables: "))
    m = int(input("Number of primal constraints: "))

    c = list(map(float, input("Enter primal objective coefficients: ").split()))
    A = [list(map(float, input(f"Row {i + 1} of A: ").split())) for i in range(m)]
    ops = [input(f"Constraint {i + 1} operator (<=/>=/==): ").strip() for i in range(m)]
    b = list(map(float, input("Enter RHS values b: ").split()))
    var_types = [input(f"x{i + 1} type (>=0/free): ").strip().lower() for i in range(n)]

    print("\nDual objective:")
    print("Minimize" if obj == "max" else "Maximize")
    print("W = " + " + ".join([f"{b[i]}*y{i + 1}" for i in range(m)]))

    print("\nDual constraints:")
    for j in range(n):
        lhs = " + ".join([f"{A[i][j]}*y{i + 1}" for i in range(m)])
        if obj == "max":
            sign = "=" if var_types[j] == "free" else ">="
        else:
            sign = "=" if var_types[j] == "free" else "<="
        print(f"{lhs} {sign} {c[j]}")

    print("\nDual variable signs:")
    for i in range(m):
        if obj == "max":
            s = "y{} >= 0".format(i + 1) if ops[i] == "<=" else ("y{} <= 0".format(i + 1) if ops[i] == ">=" else "y{} free".format(i + 1))
        else:
            s = "y{} <= 0".format(i + 1) if ops[i] == "<=" else ("y{} >= 0".format(i + 1) if ops[i] == ">=" else "y{} free".format(i + 1))
        print(s)


def assignment_hungarian():
    print("=" * 60)
    print("ASSIGNMENT PROBLEM - HUNGARIAN STYLE")
    print("=" * 60)

    n = int(input("Enter size n for n x n cost matrix: "))
    cost = [list(map(float, input(f"Row {i + 1}: ").split())) for i in range(n)]
    mat = [row[:] for row in cost]

    for i in range(n):
        rmin = min(mat[i])
        mat[i] = [x - rmin for x in mat[i]]
    for j in range(n):
        cmin = min(mat[i][j] for i in range(n))
        for i in range(n):
            mat[i][j] -= cmin

    assigned = []
    row_used = [False] * n
    col_used = [False] * n

    for _ in range(n * 2):
        progress = False
        for i in range(n):
            if row_used[i]:
                continue
            zeros = [j for j in range(n) if abs(mat[i][j]) < 1e-9 and not col_used[j]]
            if len(zeros) == 1:
                j = zeros[0]
                assigned.append((i, j))
                row_used[i] = True
                col_used[j] = True
                progress = True
        if not progress:
            break

    for i in range(n):
        if row_used[i]:
            continue
        for j in range(n):
            if not col_used[j] and abs(mat[i][j]) < 1e-9:
                assigned.append((i, j))
                row_used[i] = True
                col_used[j] = True
                break

    total = 0.0
    print("Assignments:")
    for i, j in assigned:
        total += cost[i][j]
        print(f"Task {i + 1} -> Worker {j + 1}, Cost = {cost[i][j]:.2f}")
    print(f"Total Cost = {total:.2f}")


def balance_transport(supply, demand, cost):
    s, d = sum(supply), sum(demand)
    m, n = len(supply), len(demand)
    if abs(s - d) < 1e-9:
        return supply, demand, cost
    if s > d:
        demand.append(s - d)
        for i in range(m):
            cost[i].append(0.0)
    else:
        supply.append(d - s)
        cost.append([0.0] * n)
    return supply, demand, cost


def transportation_nw_corner():
    m = int(input("Sources: "))
    n = int(input("Destinations: "))
    supply = read_floats("Supply values:")
    demand = read_floats("Demand values:")
    cost = [read_floats(f"Cost row {i + 1}:") for i in range(m)]

    supply, demand, cost = balance_transport(supply, demand, cost)
    m, n = len(supply), len(demand)
    alloc = [[0.0] * n for _ in range(m)]

    i = j = 0
    sleft = supply[:]
    dleft = demand[:]
    while i < m and j < n:
        q = min(sleft[i], dleft[j])
        alloc[i][j] = q
        sleft[i] -= q
        dleft[j] -= q
        if sleft[i] < 1e-9:
            i += 1
        if dleft[j] < 1e-9:
            j += 1

    total = sum(alloc[i][j] * cost[i][j] for i in range(m) for j in range(n))
    print(f"Total transportation cost = {total:.2f}")


def transportation_least_cost():
    m = int(input("Sources: "))
    n = int(input("Destinations: "))
    supply = read_floats("Supply values:")
    demand = read_floats("Demand values:")
    cost = [read_floats(f"Cost row {i + 1}:") for i in range(m)]

    supply, demand, cost = balance_transport(supply, demand, cost)
    m, n = len(supply), len(demand)
    alloc = [[0.0] * n for _ in range(m)]
    sleft, dleft = supply[:], demand[:]

    while any(x > 1e-9 for x in sleft) and any(x > 1e-9 for x in dleft):
        best = None
        for i in range(m):
            if sleft[i] <= 1e-9:
                continue
            for j in range(n):
                if dleft[j] <= 1e-9:
                    continue
                if best is None or cost[i][j] < best[0]:
                    best = (cost[i][j], i, j)
        if best is None:
            break
        _, i, j = best
        q = min(sleft[i], dleft[j])
        alloc[i][j] = q
        sleft[i] -= q
        dleft[j] -= q

    total = sum(alloc[i][j] * cost[i][j] for i in range(m) for j in range(n))
    print(f"Total transportation cost = {total:.2f}")


def transportation_vam():
    m = int(input("Sources: "))
    n = int(input("Destinations: "))
    supply = read_floats("Supply values:")
    demand = read_floats("Demand values:")
    cost = [read_floats(f"Cost row {i + 1}:") for i in range(m)]

    supply, demand, cost = balance_transport(supply, demand, cost)
    m, n = len(supply), len(demand)
    alloc = [[0.0] * n for _ in range(m)]
    sleft, dleft = supply[:], demand[:]
    row_done = [False] * m
    col_done = [False] * n

    while not all(row_done) and not all(col_done):
        penalties = []
        for i in range(m):
            if row_done[i]:
                continue
            vals = [cost[i][j] for j in range(n) if not col_done[j]]
            if not vals:
                continue
            vals = sorted(vals)
            p = vals[1] - vals[0] if len(vals) >= 2 else vals[0]
            penalties.append((p, "row", i))

        for j in range(n):
            if col_done[j]:
                continue
            vals = [cost[i][j] for i in range(m) if not row_done[i]]
            if not vals:
                continue
            vals = sorted(vals)
            p = vals[1] - vals[0] if len(vals) >= 2 else vals[0]
            penalties.append((p, "col", j))

        if not penalties:
            break

        _, typ, idx = max(penalties, key=lambda x: x[0])
        if typ == "row":
            i = idx
            j = min([j for j in range(n) if not col_done[j]], key=lambda jj: cost[i][jj])
        else:
            j = idx
            i = min([i for i in range(m) if not row_done[i]], key=lambda ii: cost[ii][j])

        q = min(sleft[i], dleft[j])
        alloc[i][j] = q
        sleft[i] -= q
        dleft[j] -= q
        if sleft[i] < 1e-9:
            row_done[i] = True
        if dleft[j] < 1e-9:
            col_done[j] = True

    total = sum(alloc[i][j] * cost[i][j] for i in range(m) for j in range(n))
    print(f"Total transportation cost = {total:.2f}")


def ilp_branch_bound_binary():
    print("0/1 ILP - Branch and Bound")
    n = int(input("Number of variables: "))
    c = list(map(float, input("Objective coefficients: ").split()))
    m = int(input("Number of constraints: "))
    cons = [list(map(float, input(f"Constraint {i + 1} (a1 ... an b): ").split())) for i in range(m)]

    best_sol = None
    best_val = -float("inf")

    def dfs(partial):
        nonlocal best_sol, best_val
        d = len(partial)
        ub = sum(c[i] * partial[i] for i in range(d)) + sum(max(0, c[i]) for i in range(d, n))
        if ub <= best_val + 1e-9:
            return

        if d == n:
            for row in cons:
                if sum(row[i] * partial[i] for i in range(n)) > row[-1] + 1e-9:
                    return
            val = sum(c[i] * partial[i] for i in range(n))
            if val > best_val:
                best_val = val
                best_sol = partial[:]
            return

        dfs(partial + [0])
        dfs(partial + [1])

    dfs([])
    if best_sol is None:
        print("No feasible solution")
    else:
        print("Best solution:", best_sol)
        print(f"Best value: {best_val:.4f}")


def dp_shortest_longest_path():
    stages = int(input("Number of stages: "))
    nodes = [int(input(f"Nodes in stage {i + 1}: ")) for i in range(stages)]
    mode = input("Mode (min/max): ").strip().lower()

    costs = []
    for s in range(stages - 1):
        layer = []
        print(f"Enter matrix from stage {s + 1} to {s + 2}:")
        for i in range(nodes[s]):
            layer.append(list(map(float, input(f"From node {i + 1}: ").split())))
        costs.append(layer)

    inf = float("inf")
    dp = [[inf] * nodes[s] if mode == "min" else [-inf] * nodes[s] for s in range(stages)]
    par = [[None] * nodes[s] for s in range(stages)]
    dp[0][0] = 0.0

    for s in range(stages - 1):
        for i in range(nodes[s]):
            if (mode == "min" and dp[s][i] == inf) or (mode == "max" and dp[s][i] == -inf):
                continue
            for j in range(nodes[s + 1]):
                w = costs[s][i][j]
                if w == -1:
                    continue
                nv = dp[s][i] + w
                if (mode == "min" and nv < dp[s + 1][j]) or (mode == "max" and nv > dp[s + 1][j]):
                    dp[s + 1][j] = nv
                    par[s + 1][j] = i

    end = min(range(nodes[-1]), key=lambda j: dp[-1][j]) if mode == "min" else max(range(nodes[-1]), key=lambda j: dp[-1][j])
    path = [end]
    for s in range(stages - 1, 0, -1):
        path.insert(0, par[s][path[0]])

    print("Path:")
    for s, node in enumerate(path, 1):
        print(f"Stage {s}: Node {node + 1}")
    print(f"{'Minimum' if mode == 'min' else 'Maximum'} cost = {dp[-1][end]:.4f}")


def knapsack_01():
    n = int(input("Number of items: "))
    cap = int(input("Capacity: "))
    items = [tuple(map(int, input(f"Item {i + 1} (w v): ").split())) for i in range(n)]

    dp = [[0] * (cap + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        w, v = items[i - 1]
        for c in range(cap + 1):
            dp[i][c] = dp[i - 1][c]
            if w <= c:
                dp[i][c] = max(dp[i][c], dp[i - 1][c - w] + v)

    chosen = []
    c = cap
    for i in range(n, 0, -1):
        if dp[i][c] != dp[i - 1][c]:
            chosen.append(i)
            c -= items[i - 1][0]
    chosen.reverse()

    print("Chosen items:", chosen)
    print("Max value:", dp[n][cap])


def knapsack_fractional():
    n = int(input("Number of items: "))
    cap = float(input("Capacity: "))
    items = []
    for i in range(n):
        w, v = map(float, input(f"Item {i + 1} (w v): ").split())
        items.append((w, v, v / w, i + 1))

    items.sort(key=lambda x: x[2], reverse=True)
    total = 0.0
    rem = cap
    for w, v, _, idx in items:
        if rem <= 0:
            break
        take = min(rem, w)
        frac = take / w
        total += frac * v
        rem -= take
        print(f"Item {idx}: take {frac * 100:.2f}%")
    print(f"Total value = {total:.4f}")


def tsp_branch_and_bound():
    n = int(input("Number of cities: "))
    print("Enter cost matrix; use 9999 for INF:")
    mat = [list(map(float, input(f"Row {i + 1}: ").split())) for i in range(n)]

    best_cost = float("inf")
    best_tour = None

    def dfs(path, used, cost):
        nonlocal best_cost, best_tour
        if len(path) == n:
            total = cost + mat[path[-1]][0]
            if total < best_cost:
                best_cost = total
                best_tour = path + [0]
            return

        cur = path[-1]
        for nxt in range(n):
            if nxt in used:
                continue
            w = mat[cur][nxt]
            if w >= 9999:
                continue
            nc = cost + w
            if nc >= best_cost:
                continue
            dfs(path + [nxt], used | {nxt}, nc)

    dfs([0], {0}, 0.0)

    if best_tour is None:
        print("No feasible tour")
    else:
        print("Tour:", " -> ".join(str(x + 1) for x in best_tour))
        print(f"Minimum cost = {best_cost:.4f}")


def main():
    menu = {
        "1": ("Linear Programming (Graphical Method)", lp_graphical),
        "2": ("Simplex Method", simplex_max),
        "3": ("Big M Method", big_m_method),
        "4": ("Sensitivity Analysis", sensitivity_analysis),
        "5": ("Duality (Primal to Dual)", duality_converter),
        "6": ("Assignment Problem (Hungarian)", assignment_hungarian),
        "7": ("Transportation - Northwest Corner", transportation_nw_corner),
        "8": ("Transportation - Least Cost", transportation_least_cost),
        "9": ("Transportation - VAM", transportation_vam),
        "10": ("Integer LP - Branch and Bound", ilp_branch_bound_binary),
        "11": ("Dynamic Programming Path", dp_shortest_longest_path),
        "12": ("0/1 Knapsack", knapsack_01),
        "13": ("Fractional Knapsack", knapsack_fractional),
        "14": ("TSP - Branch and Bound", tsp_branch_and_bound),
    }

    while True:
        print("\n" + "=" * 60)
        print("OPERATIONS RESEARCH ALGORITHMS (PYTHON)")
        print("=" * 60)
        for key, (name, _) in menu.items():
            print(f"{key}. {name}")
        print("0. Exit")

        choice = input("Choose option: ").strip()
        if choice == "0":
            print("Exiting...")
            break
        if choice not in menu:
            print("Invalid choice.")
            continue

        try:
            menu[choice][1]()
        except Exception as exc:
            print(f"Error while running {menu[choice][0]}: {exc}")


if __name__ == "__main__":
    main()
