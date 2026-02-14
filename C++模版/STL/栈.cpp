#include <bits/stdc++.h>
using namespace std;
long long t;
long long n;

int main() {
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
	cin >> t;
	while (t--) {
		stack<string>s;
		cin >> n;
		while (n--) {
			string x;
			cin >> x;
			if (x == "push") { //添加
				string y;
				cin >> y;
				s.push(y);
			}
			if (x == "pop") {
				if (s.empty()) {
					cout << "Empty" << '\n';
				} else {
					s.pop();//弹出栈首
				}
			}
			if (x == "query") { //查询栈是否为空
				if (s.empty()) {
					cout << "Anguei!" << '\n';
				} else {
					string y = s.top(); //查询栈首
					cout << y << '\n';
				}
			}
			if (x == "size") { //查询元素个数
				cout << s.size() << '\n';
			}
		}
	}
	return 0;
}