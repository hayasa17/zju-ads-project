#include <bits/stdc++.h>
using namespace std;

// dp[number][blackpath][isblack]
const int mod = 1e9 + 7;
// 第一个动态规划函数，时间O(n^2 logn),空间O(nlogn)
void DP1(int n) {
  // 初始化动态规划数组，大小为 (n+1) x (n+2) x 2，所有值初始化为0
  vector dp(n + 1,
            vector<vector<int>>((int)(2 * log2(n + 1)) + 1, vector<int>(2, 0)));
  // 初始状态，只有NIL的情况
  dp[0][1][1] = 1;
  // 枚举当前考虑的树的大小
  for (int i = 1; i <= n; ++i) {
    // 枚举blackpath长度
    int lim = 2 * log2(n + 1);
    for (int j = 1; j <= lim; ++j) {
      // 枚举左子树大小
      for (int k = 0; k < i; ++k) {
        // 更新当前条件下，根节点为黑色的树的个数
        // 黑色节点的儿子无颜色限制
        dp[i][j][1] = (dp[i][j][1] + 1ll * (dp[k][j - 1][0] + dp[k][j - 1][1]) *
                                         (dp[i - k - 1][j - 1][0] +
                                          dp[i - k - 1][j - 1][1])) %
                      mod;
        // 更新当前条件下，根节点为红色的树的个数
        // 红色节点的儿子必须为红色
        dp[i][j][0] =
            (dp[i][j][0] + 1ll * dp[k][j][1] * dp[i - k - 1][j][1]) % mod;
      }
    }
  }
  int ans = 0, lim = 2 * log2(n + 1);
  // 统计n个节点,blackpath长度为i，根节点为黑色的红黑树数量
  for (int i = 1; i <= lim; ++i) ans = (ans + dp[n][i][1]) % mod;
  cout << ans << endl;
  return;
}
// 第二个动态规划函数，时间O(n^2 logn),空间O(n)
void DP2(int n) {
  int ans = 0;
  // 初始化动态规划数组，大小为 (n+1) x 2，所有值初始化为0
  vector dp(n + 1, vector<int>(2, 0));
  // 初始状态，当blackpath长度为1时，存在两种情况
  // 只存在NIL，只存在一个红色节点
  dp[0][1] = dp[1][0] = 1;
  // 遍历每一列
  int lim = 2 * log2(n + 1);
  for (int j = 2; j <= lim; ++j) {
    // 遍历每一行
    for (int i = n; i >= 0; --i) {
      dp[i][1] = 0;
      // 更新当前根节点为黑色的红黑树数量
      for (int k = 0; k < i; ++k)
        // 黑色节点的儿子无颜色要求
        dp[i][1] = (dp[i][1] + 1ll * (dp[k][0] + dp[k][1]) *
                                   (dp[i - k - 1][0] + dp[i - k - 1][1])) %
                   mod;
    }
    // 更新当前根节点为红色的红黑树数量
    for (int i = 0; i <= n; ++i) {
      dp[i][0] = 0;
      for (int k = 0; k < i; ++k)
        // 红色节点的儿子必须为黑色节点
        dp[i][0] = (dp[i][0] + 1ll * dp[k][1] * dp[i - k - 1][1]) % mod;
    }
    // 累加黑色路径长度为j切根节点为黑色的红黑树数量
    ans = (ans + dp[n][1]) % mod;
  }
  cout << ans << endl;
  return;
}
/*
  由DP1优化至DP2可以观察到，DP2实际上为一个卷积形式
  若模数更为理想，可以使用NTT进一步优化至时间O(n log^2n)
*/
int main() {
  int n;
  cin >> n;
  DP1(n);
  DP2(n);
}