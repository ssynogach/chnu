#include <iostream>

using namespace std;

long long divPow2(long long s, int k) {
    s = s + ((s >> 63) & ((1LL << k) - 1));
    return s >> k;
}

int main() {
    long long a, b, c, d;
    cout << "Введіть цілі числа a, b, c, d: ";
    cin >> a >> b >> c >> d;

    long long first = (b << 5) + (b << 2) + b;
    long long sum = (d << 5) - d + (a << 3) + (a << 2);
    long long second = divPow2(sum, 11);
    long long third = (c << 6) + c;
    long long fourth = (d << 4) - (d << 1);
    long long x = first + second - third + fourth;

    long long y = 37 * b + (d * 31 + 12 * a) / 2048 - 65 * c + d * 14;

    cout << "a=" << a << " b=" << b << " c=" << c << " d=" << d << endl;
    cout << "Результат через зсуви: x = " << x << endl;
    cout << "Перевірка через * та /: y = " << y << endl;
    if (x == y) {
        cout << "Результати збігаються" << endl;
    } else {
        cout << "Результати відрізняються!" << endl;
    }
    return 0;
}
