#include <bits/stdc++.h>
using namespace std;

// 常量定义：N为最大边数/点数（4e6+5适配题目大规模数据范围）
const int N = 4e6 + 5;

// n：节点总数，m：边总数
long long n, m;

// 链式前向星存储边的结构体
struct node {
	long long to;  // 边的终点
	long long nt;  // 下一条边的索引（next的缩写）
} edge[N];         // 边数组，存储所有边

// 链式前向星核心变量
long long tot;     // 边的计数器（初始未赋值，运行时从1开始）
long long head[N]; // head[u]表示以u为起点的第一条边的索引

// Tarjan算法核心变量（求解边双连通分量）
long long low[N];  // low[u]：u能到达的最早时间戳的节点（仅通过树边+回边）
long long dfn[N];  // dfn[u]：节点u的时间戳（首次访问顺序）
long long cnt = 0; // 时间戳计数器

// 栈相关：存储当前遍历路径上的节点，用于提取边双连通分量
long long st[N];   // 栈数组
long long top = 0; // 栈顶指针

// 未使用变量（保留但无实际作用）
long long deg[N];  // 节点度数（代码中未用到）

// 边双连通分量相关
long long bcc;     // 边双连通分量的数量
vector<long long> ans[N]; // 存储每个边双连通分量的节点集合

// 链式前向星加边函数：添加一条从u到v的有向边（无向图需调用两次）
void add(long long u, long long v) {
	edge[++tot].nt = head[u]; // 新边的下一条边指向原head[u]
	edge[tot].to = v;         // 新边的终点设为v
	head[u] = tot;            // 更新head[u]为新边的索引
}

// Tarjan算法求边双连通分量
// u：当前遍历的节点，fa：u的父节点（避免回边误判为父边）
void dfs(long long u, long long fa) {
	cnt++;                      // 时间戳+1
	low[u] = cnt;               // 初始化u的low值为自身时间戳
	dfn[u] = cnt;               // 初始化u的时间戳
	st[++top] = u;              // 将u入栈（记录当前路径节点）

	bool flag = 0;              // 标记是否跳过了父边（处理无向图双向边）
	// 遍历u的所有邻边
	for (int i = head[u]; i; i = edge[i].nt) {
		long long v = edge[i].to; // 邻边的终点v

		// 处理无向图的父边：只跳过一次（避免重复处理双向边）
		if (v == fa && !flag) {
			flag = 1;
			continue;
		}

		// 如果v未被访问过（未分配时间戳）
		if (!dfn[v]) {
			dfs(v, u); // 递归遍历子节点v
			// 回溯：更新u的low值（取u和v的low最小值）
			low[u] = min(low[u], low[v]);
		}
		// 如果v已访问过（非父边，是回边），更新u的low值
		else {
			low[u] = min(low[u], dfn[v]);
		}
	}

	// 核心条件：判断u是边双连通分量的“根”
	// low[u] == dfn[u] 表示u无法通过回边到达更早的节点，栈中从u到栈顶的节点构成一个边双
	if (low[u] == dfn[u]) {
		bcc++; // 边双连通分量数量+1
		// 出栈：直到u被弹出（栈内从u到栈顶的节点构成一个边双）
		while (st[top + 1] != u) {
			ans[bcc].push_back(st[top--]);
		}
	}
}

int main() {
	// 输入节点数n和边数m
	cin >> n >> m;

	// 输入m条边，构建无向图
	while (m--) {
		long long x, y;
		cin >> x >> y;
		// 跳过自环边（自环不影响边双连通分量）
		if (x == y) {
			continue;
		}
		// 无向图双向加边
		add(x, y);
		add(y, x);
	}

	// 遍历所有节点，处理非连通图（每个连通块单独求边双）
	for (int i = 1; i <= n; i++) {
		if (!dfn[i]) { // 节点i未被访问过
			dfs(i, 0); // 从i开始DFS，父节点为0（根节点）
			top = 0;   // 处理完一个连通块后清空栈
		}
	}

	// 输出边双连通分量的总数
	cout << bcc << '\n';

	// 依次输出每个边双连通分量的信息
	for (int i = 1; i <= bcc; i++) {
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