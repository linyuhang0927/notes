#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

long long n, m;
long long dist[N];            // dist[i]：左偏树中节点i的距离（空节点的距离为-1，这里初始为0）
long long k[N];               // k[i]：节点i的权值（每个节点存储的数值）
long long ls[N], rs[N];       // ls[i]: 节点i的左孩子；rs[i]: 节点i的右孩子
bool del[N];                  // del[i]: 标记节点i是否被删除（出堆）
long long fa[N];              // fa[i]: 并查集的父节点，用于维护左偏树的根节点

// 并查集查找函数（带路径压缩）：找到节点x所在集合的根节点
// 作用：快速定位x所在左偏树的根
int find(long long x) {
	if (fa[x] == x) {         // 递归出口：x是根节点
		return x;
	} else {
		// 路径压缩：将x的父节点直接指向根，加速后续查找
		return fa[x] = find(fa[x]);
	}
}

// 合并两个左偏树（以a和b为根），返回合并后的新根节点
// 左偏树合并规则：保持左偏性质（左孩子的距离 ≥ 右孩子的距离），且小根堆性质（父节点权值 ≤ 子节点）
int merge(int a, int b) {
	if (!a) {                 // 如果a是空树，合并结果为b
		return b;
	}
	if (!b) {                 // 如果b是空树，合并结果为a
		return a;
	}
	// 维护小根堆性质：保证a的权值 ≤ b的权值，否则交换a和b
	if (k[a] > k[b]) {
		swap(a, b);
	}
	// 递归合并a的右孩子和b（左偏树优先合并右子树）
	rs[a] = merge(rs[a], b);
	// 维护左偏性质：如果右孩子的距离 > 左孩子，交换左右孩子
	if (dist[rs[a]] > dist[ls[a]]) {
		swap(rs[a], ls[a]);
	}
	// 更新当前节点a的距离：等于右孩子的距离 + 1（左偏树距离定义为到最近空节点的距离）
	dist[a] = dist[rs[a]] + 1;
	return a;                 // 返回合并后的根节点
}

int main() {
	cin >> n >> m;
	// 初始化每个节点（每个节点初始是一棵独立的左偏树）
	for (int i = 1; i <= n; i++) {
		cin >> k[i];          // 输入第i个节点的权值
		fa[i] = i;            // 并查集初始化：每个节点的父节点是自己
		// dist、ls、rs默认初始化为0，del默认初始化为false（未删除）
	}

	// 处理m次操作
	while (m--) {
		long long op;         // op: 操作类型（1：合并；2：删除最小值）
		cin >> op;

		// 操作1：合并两个节点x和y所在的左偏树
		if (op == 1) {
			int x, y;
			cin >> x >> y;
			// 跳过已被删除的节点
			if (!del[x] && !del[y]) {
				// 找到x和y所在左偏树的根节点
				x = find(x);
				y = find(y);
				// 如果不在同一棵树，才需要合并
				if (x != y) {
					// 合并两棵树，并将新根设为两个根的父节点（并查集维护）
					fa[x] = fa[y] = merge(x, y);
				}
			}
		}

		// 操作2：删除并输出x所在左偏树的最小值
		if (op == 2) {
			int x;
			cin >> x;
			// 如果x已被删除，输出-1
			if (del[x]) {
				cout << -1 << '\n';
				continue;
			}
			// 找到x所在左偏树的根节点（最小值所在节点）
			int tf = find(x);
			// 输出当前堆的最小值（根节点的权值）
			cout << k[tf] << '\n';
			// 合并根节点的左右子树，作为新的堆，并更新并查集
			// 原根节点tf被删除，新根为左右子树合并后的结果
			fa[ls[tf]] = fa[rs[tf]] = fa[tf] = merge(ls[tf], rs[tf]);
			// 标记原根节点tf为已删除
			del[tf] = 1;
		}
	}
	return 0;
}