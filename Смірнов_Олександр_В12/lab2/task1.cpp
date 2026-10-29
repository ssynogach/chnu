#include <iostream>

using namespace std;

int main() {
    long long a, b, c, d;
    cout << "Введіть цілі числа a, b, c, d: ";
    cin >> a >> b >> c >> d;

    long long s = (d << 4) - d + (a << 3) + (a << 2);
    s = s + ((s >> 63) & 1023);
    long long first = s >> 10;
    long long second = (b << 5) + (b << 1);
    long long third = (c << 13) + (c << 7) + (c << 5) + (c << 3) + (c << 2) + c;
    long long fourth = (d << 4) - (d << 1);
    long long x = second + first - third + fourth;

    long long y = 34 * b + (d * 15 + 12 * a) / 1024 - 8365 * c + d * 14;

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
