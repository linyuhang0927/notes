#include <bits/stdc++.h>
using namespace std;
const int N = 5e5 + 5;
int fa[N][25]; // fa[x][i] 表示节点x的第2^i个祖先

struct node {
	int to, nt;
} dege[N * 2];

int head[N], tot = 0;
int n, m, s;
int dep[N]; // dep[x] 表示节点x的深度

// 加边函数，建立邻接表
void add(long long u, long long v) {
	dege[++tot].nt = head[u];
	dege[tot].to = v;
	head[u] = tot;
}

// 深度优先遍历，初始化每个点的深度以及父节点
void dfs(long long u, long long deep) {
	dep[u] = deep;
	for (int i = head[u]; i; i = dege[i].nt) {
		long long v = dege[i].to;
		if (v != fa[u][0]) { // 避免走回父节点
			fa[v][0] = u; // 记录父节点
			dfs(v, deep + 1);
		}
	}
}

// 最近公共祖先（LCA）倍增求法
long long lca(long long a, long long b) {
	if (dep[a] < dep[b]) {
		swap(a, b); // 让a比b更深
	}
	long long c = dep[a] - dep[b];
	// 将a提到和b同一深度
	for (int i = 0; c; i++) {
		if (c % 2 == 1) {
			a = fa[a][i];
		}
		c = c >> 1;
	}
	if (a == b) {
		return a;
	}
	// 上提a和b，直到它们的父节点相同
	for (int j = 20; j >= 0; j--) {
		if (fa[a][j] != fa[b][j]) {
			a = fa[a][j];
			b = fa[b][j];
		}
	}
	return fa[a][0];
}

int main() {
	cin >> n >> m >> s; // 输入点数、询问次数、根节点
	for (int i = 1; i < n; i++) {
		long long x, y;
		cin >> x >> y;
		add(x, y); // 建立无向边
		add(y, x);
	}
	dfs(s, 0); // 从根节点开始DFS
	// 预处理每个点的2^i祖先
	for (int i = 1; i <= 20; i++) {
		for (int j = 1; j <= n; j++) {
			fa[j][i] = fa[fa[j][i - 1]][i - 1];
		}
	}
	// 回答询问
	while (m--) {
		long long x, y;
		cin >> x >> y;
		cout << lca(x, y) << '\n'; // 输出x和y的最近公共祖先
	}
	return 0;
}