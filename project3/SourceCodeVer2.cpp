#include <bits/stdc++.h>
using namespace std;

int n, m;
bool down[55], r;
void dfs(int x, int y, auto &graph, auto &ans) {
  bool tmpd = down[y], tmpr = r;
  if (x == n) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        if (graph[i][j]) {
          cout << i + 1 << " " << j + 1 << " ";
          for (int k = 0; k < 4; ++k) cout << ans[i][j][k] << " ";
          cout << endl;
        }
      }
    }
    exit(0);
  }
  int deg = graph[x][y];
  if (deg) {
    if (down[y]) deg--, ans[x][y][0] = 1;
    if (r) deg--, ans[x][y][2] = 1;
    if (deg == 0) {
      down[y] = 0, r = 0;
      if (y == m - 1)
        dfs(x + 1, 0, graph, ans);
      else
        dfs(x, y + 1, graph, ans);
    }
    if (deg == 1 && y != m - 1) {
      down[y] = 0, r = 1;
      ans[x][y][3] = 1;
      dfs(x, y + 1, graph, ans);
    }
    if (deg == 1 && x != n - 1) {
      down[y] = 1, r = 0;
      ans[x][y][1] = 1;
      if (y == m - 1)
        dfs(x + 1, 0, graph, ans);
      else
        dfs(x, y + 1, graph, ans);
    }
    if (deg == 2) {
      if (x != n - 1 && y != m - 1) {
        ans[x][y][1] = ans[x][y][3] = 1;
        down[y] = 1, r = 1;
        dfs(x, y + 1, graph, ans);
      }
    }
    down[y] = tmpd, r = tmpr;
    for (int i = 0; i < 4; ++i) ans[x][y][i] = 0;
  } else {
    if (down[y] && r) return;
    if (y == m - 1 && r) return;
    if (x == n - 1 && down[y]) return;
    if (y == m - 1)
      dfs(x + 1, 0, graph, ans);
    else
      dfs(x, y + 1, graph, ans);
  }
}
int main() {
  cin >> n >> m;
  vector graph(n, vector<int>(m));
  vector ans(n, vector(m, vector<int>(4)));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cin >> graph[i][j];
    }
  }
  dfs(0, 0, graph, ans);
  cout << "No Solution" << endl;
  return 0;
}