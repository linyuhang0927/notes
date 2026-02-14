#include <bits/stdc++.h>
using namespace std;
const long long mod = 19260817;
// n: 被除数/分子
// m: 除数/分母
// ans: 最终用于存储m的模mod乘法逆元
// b: 扩展欧几里得算法中的辅助变量
long long n, m, ans, b;

/**
 * @brief 快速读取整数并对mod取模，处理大数输入
 * @return 读取到的整数（已对mod取模）
 */
int getint() {
	// res存储读取的结果，ch存储当前读取的字符
	long long res = 0, ch = getchar();
	// 跳过非数字字符，直到遇到数字或文件结束符EOF
	while (!isdigit(ch) and ch != EOF)
		ch = getchar();
	// 读取数字字符并转换为整数，同时对mod取模防止溢出
	while (isdigit(ch)) {
		// res = res * 10 + (ch - '0') 的位运算优化：(res << 3)=res*8, (res << 1)=res*2，合计res*10
		res = (res << 3) + (res << 1) + (ch - '0');
		// 每一步都取模，避免数值过大超出long long范围
		res %= mod;
		ch = getchar();
	}
	return res;
}

/**
 * @brief 扩展欧几里得算法，求解 ax + by = gcd(a,b) 的一组整数解
 *        此处用于求解 m 在模 mod 下的乘法逆元（即满足 m*ans ≡ 1 (mod mod) 的ans）
 * @param x 对应本题中的m
 * @param y 对应本题中的mod
 */
void exgcd(long long x, long long y) {
	// 递归终止条件：当y=0时，gcd(x,0)=x，此时解为x*1 + 0*0 = x，即ans=1, b=0
	if (y == 0) {
		ans = 1;
		b = 0;
		return;
	}
	// 递归调用扩展欧几里得算法，求解 gcd(y, x%y)
	exgcd(y, x % y);
	// 回溯计算当前层的解：
	// 设递归返回后，ans是y的系数，b是x%y的系数
	// 则 y*ans + (x%y)*b = gcd
	// 而 x%y = x - (x/y)*y，代入得：x*b + y*(ans - (x/y)*b) = gcd
	// 因此当前层x的系数是b，y的系数是 ans - (x/y)*b
	long long d = ans;  // 暂存上一层的ans（y的系数）
	ans = b;            // 当前层x的系数 = 上一层的b（x%y的系数）
	b = d - (x / y) * b;// 当前层y的系数 = 上一层的ans - (x/y)*上一层的b
}

int main() {
	// 读取输入的n和m，使用自定义的getint函数，自动处理大数和取模
	n = getint();
	m = getint();

	// 如果m=0，分母为0无意义，输出指定提示信息
	if (m == 0) {
		cout << "Angry!";
		return 0;
	}

	// 调用扩展欧几里得算法，求解 m*ans + mod*b = gcd(m, mod)
	// 由于mod=19260817是质数，且m≠0，因此gcd(m,mod)=1，此时ans就是m的模mod逆元
	exgcd(m, mod);

	// 将逆元调整到正数范围内（因为扩展欧几里得的解可能为负）
	ans = (ans % mod + mod) % mod;

	// 计算 (n * 逆元) % mod，即 n/m mod mod 的结果（除法取模转换为乘逆元取模）
	cout << n *ans % mod;
	return 0;
}