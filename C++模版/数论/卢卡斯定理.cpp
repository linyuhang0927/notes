#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
long long t;
long long n, m, p;    // n,m 为组合数参数 C(n+m, n)，p 为模数（要求是质数）
long long mul[N];     // mul[i] 存储 i! mod p 的结果（阶乘预处理数组）

long long quickpow(long long x, long long y, long long mod) {
	long long ans = 1;    // 初始化结果为 1（乘法单位元）
	x = x % mod;          // 先对底数取模，避免溢出
	while (y != 0) {      // 指数不为 0 时循环
		if (y % 2 != 0) { // 如果指数是奇数，将当前底数乘到结果中
			ans = ans * x % mod;
		}
		y = y / 2;        // 指数除以 2（二进制右移一位）
		x = x * x % mod;  // 底数平方（对应指数的二进制位权重）
	}
	return ans;
}

/**
 * 计算小范围组合数 C(n, m) mod p（要求 n, m < p，且 p 是质数）
 * 组合数公式：C(n,m) = n!/(m! * (n-m)!) mod p
 * 由于模运算中除法不能直接计算，利用费马小定理将除法转为乘法：
 * a^(p-2) mod p 是 a 的乘法逆元（当 p 为质数且 a 与 p 互质时）
 * 因此 C(n,m) = (n! * (m!)^(p-2) * (n-m)!^(p-2)) mod p
 * @param n 组合数的上标
 * @param m 组合数的下标
 * @param p 模数（质数）
 * @return C(n,m) mod p 的结果
 */
long long C(long long n, long long m, long long p) {
	long long res;        // 存储最终结果
	if (m > n) {          // 边界条件：如果 m > n，组合数 C(n,m) = 0
		res = 0;
	} else {
		// 计算 m! 的逆元：(m!)^(p-2) mod p
		long long inv_m = quickpow(mul[m], p - 2, p);
		// 计算 (n-m)! 的逆元：(n-m)!^(p-2) mod p
		long long inv_nm = quickpow(mul[n - m], p - 2, p);
		// 第一步：n! * 逆元(m!) mod p，避免溢出
		res = (mul[n] * inv_m) % p;
		// 第二步：再乘以逆元(n-m)! 并取模，得到最终组合数
		res = (res * inv_nm) % p;
	}
	return res;
}

/**
 * 卢卡斯定理（Lucas）：计算大范围组合数 C(n, m) mod p（p 是质数，n,m 可远大于 p）
 * 核心原理（分治思想）：
 * C(n, m) ≡ C(n%p, m%p) * C(n/p, m/p) (mod p)
 * 递归终止条件：当 m = 0 时，C(n, 0) = 1（任何数选 0 个的组合数都是 1）
 *
 * 举例理解（p=5）：
 * C(7,3) mod 5 = C(7%5,3%5) * C(7/5,3/5) mod 5 = C(2,3) * C(1,0) mod 5
 * 其中 C(2,3)=0，因此最终结果为 0
 *
 * @param n 组合数的上标（本题中实际是 n+m）
 * @param m 组合数的下标（本题中实际是 n）
 * @param p 模数（质数）
 * @return C(n,m) mod p 的结果
 */
long long lucas(long long n, long long m, long long p) {
	long long res = 0;    // 初始化结果变量
	if (m == 0) {         // 递归终止条件：m=0 时，组合数为 1
		res = 1;
	} else {
		// 卢卡斯核心公式拆解：
		// 1. C(n%p, m%p)：计算 n、m 对 p 取模后的小范围组合数（调用上面的 C 函数）
		// 2. lucas(n/p, m/p, p)：递归计算 n/p、m/p 对应的组合数
		// 3. 两者相乘后取模，得到当前层的结果
		res = C(n % p, m % p, p) * lucas(n / p, m / p, p) % p;
	}
	return res;
}

int main() {
	cin >> t;
	while (t--) {
		cin >> n >> m >> p;
		mul[0] = 1;       // 0! = 1（阶乘的初始条件）
		for (int i = 1; i <= p; i++) { // 只需预处理到 p-1 即可（因为卢卡斯会取模）
			mul[i] = (mul[i - 1] * i) % p; // 递推计算 i! = (i-1)! * i mod p
		}
		// 调用卢卡斯定理计算 C(n+m, n) mod p 并输出
		// 注：C(n+m, n) = C(n+m, m)，是组合数的对称性
		cout << lucas(n + m, n, p) << '\n';
	}
	return 0;
}