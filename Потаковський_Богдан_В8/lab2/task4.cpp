#include <iostream>

using namespace std;

long long divPow2(long long s, int k) {
    s = s + ((s >> 63) & ((1LL << k) - 1));
    return s >> k;
}

long long modPow2(long long s, int k) {
    long long r = s & ((1LL << k) - 1);
    if (s < 0 && r != 0) {
        r = r - (1LL << k);
    }
    return r;
}

int main() {
    long long x, y;
    cout << "Введіть цілі числа x, y: ";
    cin >> x >> y;

    long long first = divPow2((y << 4) + (y << 2) - (x << 7) + (x << 3), 5);
    long long second = divPow2(x + (y << 5), 7);
    long long third = modPow2(x + y, 4);
    long long r = first + second + third;

    long long check = (20 * y - x * 120) / 32 + (x + 32 * y) / 128 + (x + y) % 16;

    cout << "x=" << x << " y=" << y << endl;
    cout << "Результат через зсуви: " << r << endl;
    cout << "Перевірка через * та /: " << check << endl;
    if (r == check) {
        cout << "Результати збігаються" << endl;
    } else {
        cout << "Результати відрізняються!" << endl;
    }
    return 0;
}
