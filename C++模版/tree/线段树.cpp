#include <bits/stdc++.h>  // 包含所有常用标准库，竞赛中常用
using namespace std;

const int N = 1e5 + 10;  // 数组最大长度，1e5是常见的竞赛数据范围
#define int long long     // 将int重定义为long long，避免溢出

// 线段树节点结构体
struct node {
	int ls, rs;   // 该节点管辖的区间左、右端点 (left/right segment)
	int val;      // 该节点存储的区间和值
	int tag;      // 懒标记，用于区间更新的延迟操作
} tree[N * 4];    // 线段树数组，大小通常开4倍原数组以避免越界

int a[N];  // 原始数组，存储输入数据
int n, m;  // n：数组长度，m：操作次数

// 向上更新：由子节点的值更新父节点的值
void pushup(int u) {
	// 当前节点的值 = 左子节点值 + 右子节点值
	tree[u].val = tree[u * 2].val + tree[u * 2 + 1].val;
}

// 打标记：将更新操作暂存到当前节点的懒标记中
// u：当前节点编号，x：需要区间增加的值
void maketag(int u, int x) {
	tree[u].tag += x;  // 更新当前节点的懒标记
	// 当前节点的区间和 += 区间长度 * 增加的值
	tree[u].val += (tree[u].rs - tree[u].ls + 1) * x;
}

// 向下传递：将当前节点的懒标记传递给子节点（延迟更新）
void pushdown(int u) {
	// 如果懒标记不为0，才需要传递（避免无效操作）
	if (tree[u].tag) {
		// 给左子节点打标记
		maketag(u * 2, tree[u].tag);
		// 给右子节点打标记
		maketag(u * 2 + 1, tree[u].tag);
		// 传递完成后，清空当前节点的懒标记
		tree[u].tag = 0;
	}
}

// 构建线段树
// u：当前节点编号，l/r：当前节点管辖的区间左右端点
void bt(int u, int l, int r) {
	// 初始化当前节点的管辖区间
	tree[u].ls = l;
	tree[u].rs = r;

	// 递归终止条件：到达叶子节点（区间长度为1）
	if (l == r) {
		tree[u].val = a[l];  // 叶子节点的值等于原始数组对应位置的值
		return;
	}

	// 分治：将区间拆分为左右两部分
	int mid = (l + r) / 2;
	bt(u * 2, l, mid);          // 构建左子树（节点编号2u）
	bt(u * 2 + 1, mid + 1, r);  // 构建右子树（节点编号2u+1）

	pushup(u);  // 向上更新当前节点的值
}

// 区间更新操作：将区间[l, r]内的所有数增加x
// u：当前节点编号，l/r：目标更新区间，x：增加的值
void update(int u, int l, int r, int x) {
	// 情况1：当前节点的区间完全包含在目标区间内（直接打标记）
	if (l <= tree[u].ls && tree[u].rs <= r) {
		maketag(u, x);
	}
	// 情况2：当前节点的区间与目标区间有交集（需要递归更新）
	else if (!(tree[u].ls > r || l > tree[u].rs)) {
		pushdown(u);  // 先传递懒标记，保证子节点数据正确
		update(u * 2, l, r, x);          // 递归更新左子树
		update(u * 2 + 1, l, r, x);      // 递归更新右子树
		pushup(u);                       // 向上更新当前节点的值
	}
	// 情况3：无交集，直接返回（无需处理）
}

// 区间查询操作：查询区间[l, r]的和
// u：当前节点编号，l/r：目标查询区间
int query(int u, int l, int r) {
	// 情况1：无交集，返回0（不影响求和）
	if (tree[u].ls > r || l > tree[u].rs) {
		return 0;
	}
	// 情况2：完全包含，直接返回当前节点的值
	else if (l <= tree[u].ls && tree[u].rs <= r) {
		return tree[u].val;
	}
	// 情况3：有交集，递归查询
	else {
		pushdown(u);  // 先传递懒标记，保证子节点数据正确
		// 返回左右子树查询结果的和
		return query(u * 2, l, r) + query(u * 2 + 1, l, r);
	}
}

// 主函数（signed main等价于int main，适配long long）
signed main() {
	ios::sync_with_stdio(false);  // 加速cin/cout（可选，竞赛常用优化）
	cin.tie(0);                   // 解绑cin和cout（可选优化）

	cin >> n >> m;  // 输入数组长度n和操作次数m
	// 输入原始数组
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
	}

	bt(1, 1, n);  // 构建线段树，根节点编号为1，管辖区间[1, n]

	// 处理m次操作
	for (int i = 1; i <= m; i++) {
		int opt;
		cin >> opt;  // 操作类型：1表示区间更新，2表示区间查询
		if (opt == 1) {
			// 区间更新：将[l, r]的数都加k
			int l, r, k;
			cin >> l >> r >> k;
			update(1, l, r, k);
		} else {
			// 区间查询：查询[l, r]的和并输出
			int l, r;
			cin >> l >> r;
			cout << query(1, l, r) << "\n";
		}
	}

	return 0;
}