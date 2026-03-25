#include<iostream>
using namespace std;

int evaluate1(int p, int q) {
    bool f;
    f = !(p || q);
    if (f) {
        return 1;
    }
    return 0;
}

int evaluate2(int p, int q) {
    bool f;
    f = !p && !q;
    if (f) {
        return 1;
    }
    return 0;
}

int evaluate3(int p, int q, int r) {
    bool f;
    f = (p || q) && !r;
    if (f) {
        return 1;
    }
    return 0;
}

int evaluate4(int p, int q, int r) {
    bool f;
    f = !(p || (!q && r));
    if (f) {
        return 1;
    }
    return 0;
}

int main() {
    int p, q, r;
    char ch;

    do {
        cout << "\nEnter the value of 'p': ";
        cin >> p;
        cout << "\nEnter the value of 'q': ";
        cin >> q;
        cout << "\nEnter the value of 'r': ";
        cin >> r;

        if ((p == 1 || p == 0) && (q == 1 || q == 0) && (r == 1 || r == 0)) {
            int r1, r2, r3, r4;

            r1 = evaluate1(p, q);
            r2 = evaluate2(p, q);
            r3 = evaluate3(p, q, r);
            r4 = evaluate4(p, q, r);

            cout << "\np\tq\tr\t!(p || q)\t!p && !q\t(p || q) && !r\t!(p || (!q && r))";
            cout << "\n\n" << p << "\t" << q << "\t" << r << "\t " << r1 << "\t\t " << r2 << "\t\t " << r3 << "\t\t " << r4;

            cout << "\nEnter Y/y to run the program again: ";
            cin >> ch;
        }
        else {
            cout << "\nInvalid Input!!\nProgram Terminated";
            exit(0);
        }
    } while (ch == 'Y' || ch == 'y');

    return 0;
}
