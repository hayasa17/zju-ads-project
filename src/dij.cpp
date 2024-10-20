#include "heap.cpp"

template <class T>
void dij(auto &Graph, Heap<T> &heap, int sr, int dt) {
  for (int i = 0; i < n; ++i) dis[i] = -1, vis[i] = 0;
  dis[sr] = 0;
  heap.insert(sr);
  while (!heap.isEmpty()) {
    auto [d, x] = heap.deleteMin();
    if (vis[x]) continue;
    for (auto [y, w] : graph[x]) {
      if (dis[y] == -1 || dis[y] > dis[x] + val) {
        dis[y] = dis[x] + val;
        heap.insert(make_pair(-dis[y], x));
      }
    }
  }
}
template <class T>
void testHeap(auto &graph, Heap<T> &heap, int sr, int dt) {
  int t = clock();
  dij(graph, heap, sr, dt);
  cout << clock() - t << endl;
}
int main() {
  int n, m;
  cin >> n >> m;
  vector graph(n, vector<pair<int, int>>());
  auto addedge = [&](int u, int v, int w) {
    graph[u].emplace_back(v, w);
    graph[v].emplace_back(u, w);
  };
  for (int i = 1; i <= m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    addedge(u, v, w);
  }
  int sr, dt;
  cin >> sr >> dt;
  STLVectorHeap<pair<int, int>> stlVectorHeap;
  testHeap(graph, stlVectorHeap, sr, dt);

  STLDequeHeap<pair<int, int>> stlDequeHeap;
  testHeap(graph, stlDequeHeap, sr, dt);

  LeftistHeap<pair<int, int>> leftistHeap;
  testHeap(graph, leftistHeap, sr, dt);

  FibonacciHeap<pair<int, int>> fibonacciHeap;
  testHeap(graph, fibonacciHeap, sr, dt);
}