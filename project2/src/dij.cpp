#include "heap.cpp"  // 包含堆的实现代码，这里假设 Heap 类已经定义好，并且有 insert 和 deleteMin 等方法。

template <class T>  // 定义一个模板函数，可以处理任何类型的图的边权重。
void dij(auto &Graph, Heap<T> &heap, int sr,
         int dt) {  // dij 函数接受一个图，一个堆对象，源点 sr 和目标点 dt。
  vector<int> dis(Graph.size(),
                  -1);  // 初始化一个距离数组，所有值设为 -1，表示未访问。
  vector<bool> vis(Graph.size(),
                   0);  // 初始化一个访问标记数组，所有值设为 0，表示未访问。
  dis[sr] = 0;                    // 源点到自己的距离设为 0。
  heap.insert(make_pair(0, sr));  // 将源点和它的距离（0）插入堆中。
  while (!heap.isEmpty()) {       // 当堆不为空时循环。
    auto [d, x] =
        heap.deleteMin();  // 从堆中取出距离最小的顶点，即当前最短路径的顶点。
    if (vis[x]) continue;  // 如果这个顶点已经被访问过，则跳过。
    vis[x] = 2;            // 标记这个顶点为已访问。
    for (auto [y, w] : Graph[x]) {  // 遍历当前顶点的所有邻接点。
      if (dis[y] == -1 || dis[y] > dis[x] + w) {
        // 如果邻接点未访问或者通过当前顶点到达邻接点的距离更短。
        dis[y] = dis[x] + w;                // 更新到邻接点的距离。
        heap.insert(make_pair(dis[y], y));  // 将邻接点和它的新距离插入堆中。
      }
    }
  }
}