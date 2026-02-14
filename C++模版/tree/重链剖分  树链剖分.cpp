#include <bits/stdc++.h>
using namespace std;
const long long N = 1e5 + 5;  // 数据范围，点数上限
long long n, m, r, p;         // n:节点数 m:操作数 r:根节点 p:取模值
long long w[N];               // 每个节点的初始权值

// 链式前向星存储树的边
struct node {
	long long to, nt;  // to:边的终点 nt:下一条边的索引
} edge[N * 2];         // 无向树，边数是2*(n-1)
long long head[N], tot = 0;  // head[u]:u的第一条边索引 tot:边的计数

// 线段树节点结构
struct smt {
	long long val;  // 当前区间的权值和
	long long tag;  // 懒标记，用于区间更新延迟操作
	long long l, r; // 当前节点代表的区间[l, r]
} tree[N * 4];      // 线段树数组，大小开4倍

// 链式前向星加边函数
// u:起点 v:终点
void add(long long u, long long v) {
	edge[++tot].nt = head[u];  // 新边的下一条边指向当前u的第一条边
	edge[tot].to = v;          // 新边的终点是v
	head[u] = tot;             // 更新u的第一条边为当前新边
}

// 树链剖分需要的数组
long long dep[N];  // 节点深度
long long fa[N];   // 父节点
long long siz[N];  // 子树大小
long long son[N];  // 重儿子（子树最大的子节点）
long long top[N];  // 节点所在重链的顶端节点
long long dfn[N];  // 节点的dfs序（时间戳）
long long wt[N];   // dfs序对应的权值（把树映射到线性数组）
long long cnt = 0; // dfs序计数器

// 第一次DFS：初始化dep, fa, siz, son
// u:当前节点 f:父节点 deep:当前深度
void dfs1(long long u, long long f, long long deep) {
	fa[u] = f;     // 记录父节点
	dep[u] = deep; // 记录深度
	siz[u] = 1;    // 子树大小初始化为1（自身）
	son[u] = 0;    // 重儿子初始化为0
	long long Max = 0; // 记录最大子树大小
	// 遍历u的所有邻边
	for (long long i = head[u]; i; i = edge[i].nt) {
		long long v = edge[i].to;
		if (v != f) { // 避免走回父节点
			dfs1(v, u, deep + 1); // 递归处理子节点
			siz[u] += siz[v];     // 更新子树大小
			// 更新重儿子
			if (siz[v] > Max) {
				Max = siz[v];
				son[u] = v;
			}
		}
	}
}

// 第二次DFS：初始化top, dfn, wt
// u:当前节点 tf:当前重链的顶端节点
void dfs2(long long u, long long tf) {
	dfn[u] = ++cnt;  // 给当前节点分配dfs序
	top[u] = tf;     // 记录所在重链的顶端
	wt[cnt] = w[u];  // 把节点权值映射到dfs序对应的位置
	if (!son[u]) {   // 没有重儿子（叶子节点），直接返回
		return;
	}
	dfs2(son[u], tf); // 优先处理重儿子，保持重链dfs序连续
	// 处理轻儿子（每条轻儿子都是新重链的起点）
	for (long long i = head[u]; i; i = edge[i].nt) {
		long long v = edge[i].to;
		if (v == fa[u] || v == son[u]) { // 跳过父节点和重儿子
			continue;
		}
		dfs2(v, v); // 轻儿子作为新重链的顶端
	}
}

// 线段树向上更新：由子节点更新父节点的权值和
void pushup(long long u) {
	tree[u].val = tree[u * 2].val + tree[u * 2 + 1].val;
	tree[u].val = tree[u].val % p; // 取模防止溢出
}

// 线段树向下传递懒标记：将当前节点的标记传递给子节点
void pushdown(long long u) {
	if (tree[u].tag) { // 如果当前节点有未传递的标记
		// 更新左儿子
		tree[u * 2].val += (tree[u * 2].r - tree[u * 2].l + 1) * tree[u].tag;
		tree[u * 2].val %= p;
		// 更新右儿子
		tree[u * 2 + 1].val += (tree[u * 2 + 1].r - tree[u * 2 + 1].l + 1) * tree[u].tag;
		tree[u * 2 + 1].val %= p;
		// 传递标记给子节点
		tree[u * 2].tag += tree[u].tag;
		tree[u * 2].tag %= p;
		tree[u * 2 + 1].tag += tree[u].tag;
		tree[u * 2 + 1].tag %= p;
		// 清除当前节点的标记
		tree[u].tag = 0;
	}
}

// 构建线段树
// u:当前线段树节点 l:区间左端点 r:区间右端点
void bt(long long u, long long l, long long r) {
	tree[u].l = l;
	tree[u].r = r;
	tree[u].tag = 0; // 懒标记初始化为0
	if (l == r) {    // 叶子节点，赋值
		tree[u].val = wt[l];
		return;
	}
	long long mid = (l + r) / 2;
	bt(u * 2, l, mid);       // 构建左子树
	bt(u * 2 + 1, mid + 1, r); // 构建右子树
	pushup(u);               // 向上更新当前节点值
}

// 线段树区间更新：给[L, R]区间每个数加上k
// u:当前线段树节点 L/R:目标区间 k:增量
void update(long long u, long long L, long long R, long long k) {
	if (tree[u].l > R || tree[u].r < L) { // 当前区间与目标区间无交集，直接返回
		return ;
	}
	if (L <= tree[u].l && R >= tree[u].r) { // 当前区间完全在目标区间内
		tree[u].tag += k;                   // 更新懒标记
		tree[u].tag = tree[u].tag % p;
		// 更新当前区间值（区间长度 * 增量）
		tree[u].val += (tree[u].r - tree[u].l + 1) * k;
		tree[u].val = tree[u].val % p;
	} else { // 当前区间与目标区间部分交集
		pushdown(u);                       // 先传递标记
		update(u * 2, L, R, k);            // 递归更新左子树
		update(u * 2 + 1, L, R, k);        // 递归更新右子树
		pushup(u);                         // 向上更新
	}
}

// 线段树区间查询：查询[L, R]区间的和
long long query(long long u, long long L, long long R) {
	if (tree[u].l > R || tree[u].r < L) { // 无交集，返回0
		return 0;
	}
	if (L <= tree[u].l && R >= tree[u].r) { // 完全包含，返回当前值
		return tree[u].val % p;
	} else { // 部分交集
		pushdown(u); // 先传递标记
		// 递归查询左右子树并求和
		return (query(u * 2, L, R) + query(u * 2 + 1, L, R)) % p;
	}
}

// 操作1：路径更新 - 给x到y的路径上所有节点权值加z
void pos1(long long x, long long y, long long z) {
	// 把x和y跳到同一条重链上
	while (top[x] != top[y]) {
		// 始终让深度大的链先处理（保证向上跳）
		if (dep[top[x]] < dep[top[y]]) {
			swap(x, y);
		}
		// 更新x到其重链顶端的区间
		update(1, dfn[top[x]], dfn[x], z);
		x = fa[top[x]]; // x跳到重链顶端的父节点
	}
	// 此时x和y在同一条重链上，处理深度小到深度大的区间
	if (dep[x] > dep[y]) {
		swap(x, y);
	}
	update(1, dfn[x], dfn[y], z);
}

// 操作2：路径查询 - 查询x到y的路径上所有节点权值和
long long pos2(long long x, long long y) {
	long long ans = 0;
	// 把x和y跳到同一条重链上
	while (top[x] != top[y]) {
		// 始终让深度大的链先查询
		if (dep[top[x]] < dep[top[y]]) {
			swap(x, y);
		}
		// 查询x到其重链顶端的区间和
		ans += query(1, dfn[top[x]], dfn[x]);
		ans = ans % p; // 及时取模防止溢出
		x = fa[top[x]]; // x跳到重链顶端的父节点
	}
	// 此时x和y在同一条重链上，查询深度小到深度大的区间
	if (dep[x] > dep[y]) {
		swap(x, y);
	}
	ans += query(1, dfn[x], dfn[y]);
	ans = ans % p;
	return ans;
}

signed main() {
	cin >> n >> m >> r >> p;
	// 输入每个节点的初始权值
	for (long long i = 1; i <= n; i++) {
		cin >> w[i];
	}
	// 输入树的边
	for (long long i = 1; i < n; i++) {
		long long x, y;
		cin >> x >> y;
		add(x, y);
		add(y, x); // 无向边，双向添加
	}
	// 第一次DFS：处理父子关系、子树大小、重儿子
	dfs1(r, 0, 0);
	// 第二次DFS：处理dfs序、重链顶端
	dfs2(r, r);
	// 构建线段树
	bt(1, 1, n);

	// 处理m个操作
	while (m--) {
		long long i;
		cin >> i;
		if (i == 1) { // 操作1：路径加
			long long x, y, z;
			cin >> x >> y >> z;
			pos1(x, y, z);
		}
		if (i == 2) { // 操作2：路径求和
			long long x, y;
			cin >> x >> y;
			cout << pos2(x, y) << '\n';
		}
		if (i == 3) { // 操作3：子树加
			long long x, y;
			cin >> x >> y;
			// 子树对应的dfs序区间是[dfn[x], dfn[x]+siz[x]-1]
			update(1, dfn[x], dfn[x] + siz[x] - 1, y);
		}
		if (i == 4) { // 操作4：子树求和
			long long x;
			cin >> x;
			cout << query(1, dfn[x], dfn[x] + siz[x] - 1) << '\n';
		}
	}
	return 0;
}