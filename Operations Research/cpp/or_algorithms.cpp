#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
#include <functional>

using namespace std;

const double EPS = 1e-9;
const double INF = 9999.0;

void lp_graphical() {
    cout << "\n==== LINEAR PROGRAMMING - GRAPHICAL METHOD ====\n";
    string obj;
    double c1, c2;
    int m;

    cout << "Objective (max/min): ";
    cin >> obj;
    cout << "Coefficient of x1: ";
    cin >> c1;
    cout << "Coefficient of x2: ";
    cin >> c2;
    cout << "Number of constraints: ";
    cin >> m;

    vector<double> a1(m), a2(m), b(m);
    vector<string> op(m);
    for (int i = 0; i < m; i++) {
        cout << "Constraint " << i + 1 << " (a1 a2 op b): ";
        cin >> a1[i] >> a2[i] >> op[i] >> b[i];
    }

    vector<pair<double, double>> points = {{0.0, 0.0}};

    for (int i = 0; i < m; i++) {
        if (abs(a1[i]) > EPS) points.push_back({max(0.0, b[i] / a1[i]), 0.0});
        if (abs(a2[i]) > EPS) points.push_back({0.0, max(0.0, b[i] / a2[i])});
    }

    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m; j++) {
            double det = a1[i] * a2[j] - a1[j] * a2[i];
            if (abs(det) <= EPS) continue;
            double x = (b[i] * a2[j] - b[j] * a2[i]) / det;
            double y = (a1[i] * b[j] - a1[j] * b[i]) / det;
            if (x >= -EPS && y >= -EPS) points.push_back({max(0.0, x), max(0.0, y)});
        }
    }

    vector<pair<double, double>> feasible;
    for (auto p : points) {
        bool ok = true;
        for (int i = 0; i < m; i++) {
            double v = a1[i] * p.first + a2[i] * p.second;
            if (op[i] == "<=" && v > b[i] + 1e-6) ok = false;
            if (op[i] == ">=" && v < b[i] - 1e-6) ok = false;
            if ((op[i] == "=" || op[i] == "==") && abs(v - b[i]) > 1e-6) ok = false;
        }
        if (ok) feasible.push_back(p);
    }

    vector<pair<double, double>> uniq;
    for (auto p : feasible) {
        bool dup = false;
        for (auto q : uniq) {
            if (abs(p.first - q.first) < 1e-6 && abs(p.second - q.second) < 1e-6) {
                dup = true;
                break;
            }
        }
        if (!dup) uniq.push_back(p);
    }

    if (uniq.empty()) {
        cout << "No feasible corner points found.\n";
        return;
    }

    double bestZ = (obj == "max") ? -1e18 : 1e18;
    pair<double, double> bestP = {0, 0};

    for (auto p : uniq) {
        double z = c1 * p.first + c2 * p.second;
        cout << fixed << setprecision(4) << "(" << p.first << ", " << p.second << ") => Z = " << z << "\n";
        if ((obj == "max" && z > bestZ) || (obj == "min" && z < bestZ)) {
            bestZ = z;
            bestP = p;
        }
    }

    cout << "Optimal: x1 = " << bestP.first << ", x2 = " << bestP.second << ", Z = " << bestZ << "\n";
}

void simplex_max() {
    cout << "\n==== SIMPLEX METHOD (MAX) ====\n";
    int n, m;
    cout << "Decision variables: ";
    cin >> n;
    cout << "Constraints: ";
    cin >> m;

    vector<double> c(n);
    cout << "Objective coefficients:\n";
    for (int i = 0; i < n; i++) cin >> c[i];

    vector<vector<double>> A(m, vector<double>(n));
    vector<double> b(m);
    cout << "Enter constraints (a1 ... an b):\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) cin >> A[i][j];
        cin >> b[i];
    }

    int total = n + m;
    vector<vector<double>> tab(m + 1, vector<double>(total + 1, 0.0));
    vector<int> basis(m);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) tab[i][j] = A[i][j];
        tab[i][n + i] = 1.0;
        tab[i][total] = b[i];
        basis[i] = n + i;
    }
    for (int j = 0; j < n; j++) tab[m][j] = -c[j];

    while (true) {
        int pc = 0;
        for (int j = 1; j < total; j++) if (tab[m][j] < tab[m][pc]) pc = j;
        if (tab[m][pc] >= -1e-10) break;

        int pr = -1;
        double best = numeric_limits<double>::infinity();
        for (int i = 0; i < m; i++) {
            if (tab[i][pc] > 1e-10) {
                double ratio = tab[i][total] / tab[i][pc];
                if (ratio < best) {
                    best = ratio;
                    pr = i;
                }
            }
        }
        if (pr == -1) {
            cout << "Unbounded solution.\n";
            return;
        }

        basis[pr] = pc;
        double piv = tab[pr][pc];
        for (int j = 0; j <= total; j++) tab[pr][j] /= piv;

        for (int i = 0; i <= m; i++) {
            if (i == pr) continue;
            double f = tab[i][pc];
            for (int j = 0; j <= total; j++) tab[i][j] -= f * tab[pr][j];
        }
    }

    vector<double> sol(n, 0.0);
    for (int i = 0; i < m; i++) if (basis[i] < n) sol[basis[i]] = tab[i][total];

    for (int i = 0; i < n; i++) cout << "x" << i + 1 << " = " << fixed << setprecision(4) << sol[i] << "\n";
    cout << "Maximum Z = " << fixed << setprecision(4) << tab[m][total] << "\n";
}

void knapsack_01() {
    cout << "\n==== 0/1 KNAPSACK ====\n";
    int n, cap;
    cout << "Items: ";
    cin >> n;
    cout << "Capacity: ";
    cin >> cap;

    vector<int> w(n), v(n);
    for (int i = 0; i < n; i++) {
        cout << "Item " << i + 1 << " (weight value): ";
        cin >> w[i] >> v[i];
    }

    vector<vector<int>> dp(n + 1, vector<int>(cap + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= cap; c++) {
            dp[i][c] = dp[i - 1][c];
            if (w[i - 1] <= c) dp[i][c] = max(dp[i][c], dp[i - 1][c - w[i - 1]] + v[i - 1]);
        }
    }

    cout << "Maximum value = " << dp[n][cap] << "\n";
}

void knapsack_fractional() {
    cout << "\n==== FRACTIONAL KNAPSACK ====\n";
    int n;
    double cap;
    cout << "Items: ";
    cin >> n;
    cout << "Capacity: ";
    cin >> cap;

    vector<tuple<double, double, double, int>> items;
    for (int i = 0; i < n; i++) {
        double w, v;
        cout << "Item " << i + 1 << " (weight value): ";
        cin >> w >> v;
        items.push_back({w, v, v / w, i + 1});
    }

    sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return get<2>(a) > get<2>(b); });

    double total = 0;
    for (auto& it : items) {
        if (cap <= 0) break;
        double w = get<0>(it), v = get<1>(it);
        double take = min(cap, w);
        double frac = take / w;
        total += frac * v;
        cap -= take;
    }

    cout << "Maximum fractional value = " << fixed << setprecision(4) << total << "\n";
}

void transportation_nw_corner() {
    cout << "\n==== TRANSPORTATION - NORTHWEST CORNER ====\n";
    int m, n;
    cout << "Sources: ";
    cin >> m;
    cout << "Destinations: ";
    cin >> n;

    vector<double> s(m), d(n);
    cout << "Supply:\n";
    for (int i = 0; i < m; i++) cin >> s[i];
    cout << "Demand:\n";
    for (int j = 0; j < n; j++) cin >> d[j];

    vector<vector<double>> c(m, vector<double>(n));
    cout << "Cost matrix:\n";
    for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) cin >> c[i][j];

    double ts = 0, td = 0;
    for (double x : s) ts += x;
    for (double x : d) td += x;
    if (abs(ts - td) > 1e-9) {
        if (ts > td) {
            d.push_back(ts - td);
            for (int i = 0; i < m; i++) c[i].push_back(0.0);
            n++;
        } else {
            s.push_back(td - ts);
            c.push_back(vector<double>(n, 0.0));
            m++;
        }
    }

    vector<vector<double>> a(m, vector<double>(n, 0.0));
    int i = 0, j = 0;
    while (i < m && j < n) {
        double q = min(s[i], d[j]);
        a[i][j] = q;
        s[i] -= q;
        d[j] -= q;
        if (s[i] < 1e-9) i++;
        if (d[j] < 1e-9) j++;
    }

    double total = 0;
    for (int r = 0; r < m; r++) for (int k = 0; k < n; k++) total += a[r][k] * c[r][k];
    cout << "Total cost = " << fixed << setprecision(4) << total << "\n";
}

void tsp_branch_bound() {
    cout << "\n==== TSP - BRANCH AND BOUND ====\n";
    int n;
    cout << "Cities: ";
    cin >> n;
    vector<vector<double>> mat(n, vector<double>(n));
    cout << "Cost matrix (use 9999 for no edge):\n";
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> mat[i][j];

    double best = numeric_limits<double>::infinity();
    vector<int> bestTour;

    function<void(vector<int>, vector<bool>, double)> dfs = [&](vector<int> path, vector<bool> used, double cost) {
        int cur = path.back();
        if ((int)path.size() == n) {
            double total = cost + mat[cur][0];
            if (total < best) {
                best = total;
                bestTour = path;
                bestTour.push_back(0);
            }
            return;
        }

        for (int nxt = 0; nxt < n; nxt++) {
            if (used[nxt]) continue;
            if (mat[cur][nxt] >= INF) continue;
            if (cost + mat[cur][nxt] >= best) continue;
            auto np = path;
            auto nu = used;
            np.push_back(nxt);
            nu[nxt] = true;
            dfs(np, nu, cost + mat[cur][nxt]);
        }
    };

    vector<bool> used(n, false);
    used[0] = true;
    dfs({0}, used, 0.0);

    if (bestTour.empty()) {
        cout << "No feasible tour found.\n";
        return;
    }

    cout << "Tour: ";
    for (size_t i = 0; i < bestTour.size(); i++) {
        if (i) cout << " -> ";
        cout << bestTour[i] + 1;
    }
    cout << "\nMinimum cost = " << fixed << setprecision(4) << best << "\n";
}

void feature_placeholder(const string& name) {
    cout << "\n" << name << " is documented in this project and fully implemented in Python suite.\n";
    cout << "You can extend this C++ file with your exact classroom version as needed.\n";
}

int main() {
    while (true) {
        cout << "\n============================================================\n";
        cout << "OPERATIONS RESEARCH ALGORITHMS (C++)\n";
        cout << "============================================================\n";
        cout << "1. Linear Programming (Graphical Method)\n";
        cout << "2. Simplex Method\n";
        cout << "3. Big M Method\n";
        cout << "4. Sensitivity Analysis\n";
        cout << "5. Duality (Primal to Dual)\n";
        cout << "6. Assignment Problem (Hungarian)\n";
        cout << "7. Transportation - Northwest Corner\n";
        cout << "8. Transportation - Least Cost\n";
        cout << "9. Transportation - VAM\n";
        cout << "10. Integer LP - Branch and Bound\n";
        cout << "11. Dynamic Programming Path\n";
        cout << "12. 0/1 Knapsack\n";
        cout << "13. Fractional Knapsack\n";
        cout << "14. TSP - Branch and Bound\n";
        cout << "0. Exit\n";

        int choice;
        cout << "Choose option: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                lp_graphical();
                break;
            case 2:
                simplex_max();
                break;
            case 3:
                feature_placeholder("Big M Method");
                break;
            case 4:
                feature_placeholder("Sensitivity Analysis");
                break;
            case 5:
                feature_placeholder("Duality Conversion");
                break;
            case 6:
                feature_placeholder("Assignment Problem (Hungarian)");
                break;
            case 7:
                transportation_nw_corner();
                break;
            case 8:
                feature_placeholder("Transportation Least Cost");
                break;
            case 9:
                feature_placeholder("Transportation VAM");
                break;
            case 10:
                feature_placeholder("Integer LP - Branch and Bound");
                break;
            case 11:
                feature_placeholder("Dynamic Programming Path");
                break;
            case 12:
                knapsack_01();
                break;
            case 13:
                knapsack_fractional();
                break;
            case 14:
                tsp_branch_bound();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }

    cout << "Exiting...\n";
    return 0;
}
