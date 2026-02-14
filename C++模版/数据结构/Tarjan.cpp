//割点
#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5; // 定义数组最大容量，2e5+5 适配题目中节点数量的上限

// 邻接表存储边的结构体
struct node {
	int to;  // 边的终点
	int nt;  // 下一条边的索引（邻接表的链式存储）
} edge[N]; // 存储所有边的数组
int head[N], tot; // head[u]表示u节点的第一条边的索引，tot是边的计数（初始化0）

// Tarjan算法核心变量
int dfn[N];  // dfn[u]表示u节点的时间戳（首次访问顺序）
int low[N];  // low[u]表示u节点能回溯到的最早时间戳
int st[N], top; // 模拟栈，存储当前强连通分量/双连通分量的节点，top是栈顶指针
int deg[N], cnt; // deg[u]统计u节点作为割点的"度数"，cnt是时间戳计数器（初始化0）
int n, m, ans;   // n节点数，m边数，ans统计割点数量

// 邻接表添加边的函数（无向图需要加双向边）
void add(int u, int v) {
	tot++;  // 边计数+1
	edge[tot] = {v, head[u]}; // 新建边：终点v，上一条边是head[u]
	head[u] = tot; // 更新u节点的第一条边为当前边
}

// Tarjan算法的深度优先搜索函数，找割点
void dfs(int u) {
	cnt++;  // 时间戳+1
	dfn[u] = cnt; // 记录u的首次访问时间戳
	low[u] = cnt; // 初始时，low[u]等于自身时间戳
	st[++top] = u; // 将u入栈

	// 遍历u的所有邻边
	for (int i = head[u]; i; i = edge[i].nt) {
		int v = edge[i].to; // 获取邻接节点v
		if (!dfn[v]) { // v未被访问过（树边）
			dfs(v);  // 递归访问v
			// 回溯时更新u的low值：取u和v的low最小值
			low[u] = min(low[u], low[v]);

			// 核心判断：如果v的low值 >= u的dfn值，说明u是割点
			// （删除u后，v所在子树无法回到u的祖先）
			if (low[v] >= dfn[u]) {
				++deg[u];  // u的割点度数+1
				int x;
				// 弹出栈中节点直到v（处理双连通分量）
				do {
					x = st[top--]; // 栈顶节点出栈
					++deg[x];     // 标记该节点属于当前双连通分量
				} while (v != x);  // 直到弹出v为止
			}
		} else { // v已被访问过（回边）
			// 更新u的low值：取u的low和v的dfn最小值（注意是dfn[v]不是low[v]，避免跨分量）
			low[u] = min(low[u], dfn[v]);
		}
	}
}

int main() {
	int n, m;
	cin >> n >> m; // 输入节点数n和边数m

	// 输入m条无向边，添加双向边
	while (m--) {
		int x, y;
		cin >> x >> y;
		add(x, y); // 添加x->y的边
		add(y, x); // 添加y->x的边（无向图）
	}

	// 遍历所有节点，处理非连通图的情况
	for (int i = 1; i <= n; i++) {
		if (!dfn[i]) { // 节点i未被访问过（新的连通分量）
			dfs(i);   // 对该连通分量执行Tarjan
			top = 0;  // 处理完一个连通分量后清空栈
		}
	}

	// 统计割点数量：deg[i]>1的节点是割点
	// （注：根节点需要deg[i]>1才是割点，非根节点deg[i]>=1即是）
	for (int i = 1; i <= n; i++) {
		if (deg[i] > 1) {
			ans++;  // 割点计数+1
		}
	}

	// 输出割点总数
	cout << ans << "\n";
	// 输出所有割点
	for (int i = 1; i <= n; i++) {
		if (deg[i] > 1) {
			cout << i << " ";
		}
	}
	return 0;
}