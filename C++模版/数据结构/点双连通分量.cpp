#include <bits/stdc++.h>
using namespace std;

// 常量定义：N为最大边数/点数（4e6+5适配题目数据范围）
const int N = 4e6 + 5;

// n：节点总数，m：边总数
long long n, m;

// 链式前向星存储边的结构体
struct node {
	long long to;   // 边的终点
	long long nt;   // 下一条边的索引（next的缩写）
} edge[N];          // 边数组，存储所有边

// 链式前向星核心变量
long long head[N];  // head[u]表示以u为起点的第一条边的索引
long long tot = 1;  // 边的计数器（从1开始，避免0作为空指针）

// Tarjan算法核心变量
long long dfn[N];   // dfn[u]表示节点u的时间戳（首次访问顺序）
long long low[N];   // low[u]表示u能到达的最早时间戳的节点
long long cnt;      // 时间戳计数器

// 栈相关：存储当前遍历路径上的节点，用于提取点双连通分量
long long st[N];    // 栈数组
long long top = 0;  // 栈顶指针

// 点双连通分量相关
long long b = 0;                // 点双连通分量的数量
vector<long long> ans[N];       // 存储每个点双连通分量的节点集合

// 链式前向星加边函数：添加一条从u到v的有向边（无向图需调用两次）
void add(long long u, long long v) {
	edge[++tot].to = v;         // 新边的终点设为v
	edge[tot].nt = head[u];     // 新边的下一条边指向原head[u]
	head[u] = tot;              // 更新head[u]为新边的索引
}

// Tarjan算法求点双连通分量
// u：当前遍历的节点，fa：u的父节点（避免回边误判）
void dfs(long long u, long long fa) {
	long long son = 0;          // 统计u的子节点数量（用于判断根节点是否为割点）
	cnt++;                      // 时间戳+1
	dfn[u] = cnt;               // 初始化u的时间戳
	low[u] = cnt;               // 初始化u的low值为自身时间戳
	st[++top] = u;              // 将u入栈（当前路径节点）

	// 遍历u的所有邻边
	for (int i = head[u]; i; i = edge[i].nt) {
		int v = edge[i].to;     // 邻边的终点v

		// 如果v未被访问过（未分配时间戳）
		if (!dfn[v]) {
			son++;              // 子节点数+1
			dfs(v, u);          // 递归遍历子节点v

			// 回溯：更新u的low值（取u和v的low最小值）
			low[u] = min(low[u], low[v]);

			// 核心条件：判断u是割点，且v所在子树构成一个点双连通分量
			// low[v] >= dfn[u] 表示v无法通过回边到达u的祖先，u是割点
			if (low[v] >= dfn[u]) {
				b++;            // 点双连通分量数量+1

				// 出栈：直到v被弹出（此时栈内从v到栈顶的节点 + u 构成一个点双）
				// 注意：st[top+1] != v 是因为栈顶先弹出，直到v被弹出为止
				while (st[top + 1] != v) {
					ans[b].push_back(st[top--]);
				}
				// 将u加入当前点双连通分量（割点会被多个点双包含）
				ans[b].push_back(u);
			}
		}
		// 如果v已访问过，且不是父节点（回边），更新u的low值
		else if (v != fa) {
			low[u] = min(low[u], dfn[v]);
		}
	}

	// 特殊情况：处理孤立节点（根节点且无子节点）
	// 孤立节点自身构成一个点双连通分量
	if (fa == 0 && son == 0) {
		ans[++b].push_back(u);
	}
}

int main() {
	// 输入节点数n和边数m
	cin >> n >> m;

	// 输入m条边，无向图需双向加边
	while (m--) {
		long long x, y;
		cin >> x >> y;
		add(x, y);
		add(y, x);
	}

	// 遍历所有节点，处理非连通图（每个连通块单独求点双）
	for (int i = 1; i <= n; i++) {
		if (!dfn[i]) {          // 节点i未被访问过
			dfs(i, 0);          // 从i开始DFS，父节点为0（根节点）
			top = 0;            // 处理完一个连通块后清空栈
		}
	}

	// 输出点双连通分量的总数
	cout << b << '\n';

	// 依次输出每个点双连通分量的信息
	for (int i = 1; i <= b; i++) {
		// 先输出当前分量的节点数量
		cout << ans[i].size() << " ";
		// 输出分量内的所有节点
		for (int j = 0; j < ans[i].size(); j++) {
			cout << ans[i][j] << " ";
		}
		cout << '\n';
	}

	return 0;
}