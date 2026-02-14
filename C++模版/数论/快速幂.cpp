#include <bits/stdc++.h>
using namespace std;
long long x, y, mod, ans = 1;

int main() {
	cin >> x >> y >> mod;
	//cout << x << "^" << y << " mod " << mod << "=";
	while (y != 0) {
		if (y % 2 != 0) {
			ans = ans * x % mod;
		}
		y = y / 2;
		x = x * x % mod;
	}
	cout << ans << '\n';
	return 0;
}