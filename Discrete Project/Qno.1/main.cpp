#include <iostream>
using namespace std;

bool isReflexive(int matrix[10][10], int n) {
    for (int i = 0; i < n; ++i) {
        if (matrix[i][i] != 1) {
            return false;
        }
    }
    return true;
}

bool isSymmetric(int matrix[10][10], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

bool isAntisymmetric(int matrix[10][10], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && matrix[i][j] == 1 && matrix[j][i] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool isTransitive(int matrix[10][10], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (matrix[i][j] == 1 && matrix[j][k] == 1 && matrix[i][k] != 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    int n;

    cout << "Enter the size of the matrix: ";
    cin >> n;

    int matrix[10][10];

    cout << "Enter the matrix elements (0 or 1):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
        }
    }

    if (isReflexive(matrix, n)) {
        cout << "Reflexive Relation" << endl;
    }
    else {
        cout << "To make it reflexive, add the pairs: ";
        for (int i = 0; i < n; ++i) {
            if (matrix[i][i] != 1) {
                cout << "(" << i + 1 << "," << i + 1 << ") ";
            }
        }
        cout << endl;
    }

    if (isSymmetric(matrix, n)) {
        cout << "Symmetric Relation" << endl;
    }
    else {
        cout << "To make it symmetric, add the pairs: ";
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (matrix[i][j] != matrix[j][i]) {
                    cout << "(" << j + 1 << "," << i + 1 << ") ";
                }
            }
        }
        cout << endl;
    }

    if (isAntisymmetric(matrix, n)) {
        cout << "Antisymmetric Relation" << endl;
    }
    else {
        cout << "To make it antisymmetric, remove the pairs: ";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j && matrix[i][j] == 1 && matrix[j][i] == 1) {
                    cout << "(" << i + 1 << "," << j + 1 << ") ";
                }
            }
        }
        cout << endl;
    }

    if (isTransitive(matrix, n)) {
        cout << "Transitive Relation" << endl;
    }
    else {
        cout << "To make it transitive, add the pairs: ";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    if (matrix[i][j] == 1 && matrix[j][k] == 1 && matrix[i][k] != 1) {
                        cout << "(" << i + 1 << "," << k + 1 << ") ";
                    }
                }
            }
        }
        cout << endl;
    }

    return 0;
}
