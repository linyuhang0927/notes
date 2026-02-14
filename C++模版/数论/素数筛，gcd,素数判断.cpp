#include <bits/stdc++.h>
using namespace std;

// 定义数组最大长度，1e5+5 可处理 10 万以内的数值筛选
const int N = 1e5 + 5;

// 全局变量声明
long long n;          // 输入的目标数值，用于质数相关操作
long long vis[N];     // 标记数组：vis[i]=1 表示 i 不是质数，vis[i]=0 表示 i 是质数
long long cnt;        // 质数计数：记录筛选出的质数个数
long long primes[N];  // 质数存储数组：primes[cnt] 存储第 cnt 个质数

/**
 * 质数判断函数（试除法）：判断一个数 x 是否为质数
 * 原理：若 x 有大于 1 且小于等于 sqrt(x) 的因数，则 x 不是质数
 * 注意：函数存在笔误（下文会标注），需注意修正
 * @param x 待判断的数
 * @return 0（非质数）或 1（质数）
 */
bool sp(long long x) {
    if (x < 2) {                  // 边界条件：小于 2 的数都不是质数
        return 0;
    }
    // 【注意】此处存在笔误：循环条件应为 i*i <= x，而非 i*i <= n
    // 原代码用全局变量 n 代替了参数 x，会导致判断错误
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {         // 若 n 能被 i 整除，说明有因数，非质数
            return 0;
        }
    }
    return 1;                     // 无因数，是质数
}

/**
 * 最大公约数（GCD）函数：递归实现欧几里得算法
 * 原理：gcd(x, y) = gcd(y, x%y)，终止条件为 y=0 时返回 x
 * 注意：函数存在笔误（下文会标注），需注意修正
 * @param x 第一个数
 * @param y 第二个数
 * @return x 和 y 的最大公约数
 */
long long gcd(long long x, long long y) {
    if (y) {                      // 若 y 不为 0，递归计算 gcd(y, x%y)
        // 【注意】此处存在笔误：应为 gcd(y, x%y)，而非 gcd(x, x%y)
        // 原代码第一个参数错误，会导致递归逻辑错误
        return gcd(x, x % y);
    }
    return x;                     // 终止条件：y=0 时，x 是最大公约数
}

/**
 * 埃拉托斯特尼筛法（埃氏筛）：筛选 2~n 范围内的所有质数
 * 原理：先标记所有数为质数（vis=0），再从 2 开始，将每个质数的倍数标记为非质数（vis=1）
 * 时间复杂度：O(n log log n)
 */
void ass() {
    // 遍历 2~n 的所有数
    for (int i = 2; i <= n; i++) {
        if (vis[i]) {             // 若已被标记为非质数，跳过
            continue;
        }
        primes[++cnt] = i;        // 未被标记，说明是质数，存入质数数组
        // 标记当前质数的所有倍数为非质数（从 2*i 开始，步长 i）
        for (int j = 2 * i; j <= n; j += i) {
            vis[j] = 1;
        }
    }
}

/**
 * 线性筛（欧拉筛）：筛选 2~n 范围内的所有质数（更高效的筛法）
 * 原理：保证每个合数仅被其最小质因数筛除一次，避免重复标记
 * 时间复杂度：O(n)（线性时间），比埃氏筛效率更高
 */
void xxs() {
    // 遍历 2~n 的所有数
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) {            // 若未被标记，说明是质数，存入质数数组
            primes[++cnt] = i;
        }
        // 遍历已找到的质数，标记 i*primes[j] 为非质数
        for (int j = 0; primes[j]*i <= n; j++) {
            vis[primes[j]*i] = 1; // 标记合数为非质数
            // 关键优化：若 i 能被当前质数整除，说明 primes[j] 是 i 的最小质因数
            // 后续的合数会由更小的质因数筛除，无需继续循环，避免重复标记
            if (i % primes[j] == 0) {
                break;
            }
        }
    }
}

int main() {
    cin >> n;                     // 输入目标数值 n
    // 注：原主函数仅输入 n 未调用任何函数，可根据需求添加函数调用
    // 例如：ass(); // 调用埃氏筛
    //       xxs(); // 调用线性筛
    return 0;
}