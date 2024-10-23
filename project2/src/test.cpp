#include "dij.cpp"  // 包含Dijkstra算法的实现

const int SIZE = 12;             // 图的总数
const int QUERY_SIZE = 2000;     // 查询的总数
const int HEAP_TYPE = 5;         // 堆的类型数量
const string path = "../data/";  // 数据文件的路径
const string graph[] = {
    // 不同的图文件名
    "USA-road-d.BAY.gr", "USA-road-d.CAL.gr", "USA-road-d.COL.gr",
    "USA-road-d.CTR.gr", "USA-road-d.E.gr",   "USA-road-d.FLA.gr",
    "USA-road-d.LKS.gr", "USA-road-d.NE.gr",  "USA-road-d.NW.gr",
    "USA-road-d.NY.gr",  "USA-road-d.USA.gr", "USA-road-d.W.gr",
};
const string query[] = {  // 对应的查询文件名
    "test_data_BAY.txt", "test_data_CAL.txt", "test_data_COL.txt",
    "test_data_CTR.txt", "test_data_E.txt",   "test_data_FLA.txt",
    "test_data_LKS.txt", "test_data_NE.txt",  "test_data_NW.txt",
    "test_data_NY.txt",  "test_data_W.txt",   "test_data_USA.txt"};
const string heap_type[] = {  // 不同的堆类型名称
    "STLVectorHeap", "STLDequeHeap", "LeftistHeap", "FibonacciHeap",
    "BinomialQueue"};
int time_total[] = {0, 0, 0, 0, 0};  // 记录每种堆的总时间
ofstream outFile("output.txt");      // 输出文件

// 解决一个图和一个查询文件的函数
void solve(string gr, string qr) {
  int tmp_time[] = {0, 0, 0, 0, 0};  // 记录每种堆在当前图上的总时间
  ifstream inGr(gr), inQr(qr);       // 打开图文件和查询文件
  string op;                         // 用于读取图文件中的操作类型
  int n, m;                          // 图的节点数和边数
  // 读取图文件中的节点数和边数
  while (inGr >> op) {
    if (op == "p") {
      inGr >> op;
      inGr >> n >> m;
      break;
    }
  }
  vector Graph(n, vector<pair<int, int>>());  // 创建邻接表表示图
  // 读取图文件中的边信息
  while (inGr >> op) {
    if (op == "a") {
      int u, v, w;
      inGr >> u >> v >> w;
      u--, v--;  // 将节点编号减1，以适应C++的0-based索引
      Graph[u].emplace_back(v, w);  // 添加边(u, v)和权重w
      Graph[v].emplace_back(u, w);  // 添加边(v, u)和权重w，因为是无向图
    }
  }
  // 对每个查询进行Dijkstra算法
  for (int i = 0; i < QUERY_SIZE; ++i) {
    int sr, dt;
    inQr >> sr >> dt;  // 读取源节点和目标节点
    sr--, dt--;        // 将节点编号减1，以适应C++的0-based索引
    int t = clock();   // 开始计时
    STLVectorHeap<pair<int, int>> stlVectorHeap;  // 创建STLVectorHeap堆
    dij(Graph, stlVectorHeap, sr, dt);            // 执行Dijkstra算法
    tmp_time[0] += clock() - t;                   // 记录时间
    t = clock();

    STLDequeHeap<pair<int, int>> stlDequeHeap;  // 创建STLDequeHeap堆
    dij(Graph, stlDequeHeap, sr, dt);           // 执行Dijkstra算法
    tmp_time[1] += clock() - t;

    t = clock();
    LeftistHeap<pair<int, int>> leftistHeap;  // 创建LeftistHeap堆
    dij(Graph, leftistHeap, sr, dt);          // 执行Dijkstra算法
    tmp_time[2] += clock() - t;

    t = clock();
    FibonacciHeap<pair<int, int>> fibonacciHeap;  // 创建FibonacciHeap堆
    dij(Graph, fibonacciHeap, sr, dt);            // 执行Dijkstra算法
    tmp_time[3] += clock() - t;

    t = clock();
    BinomialQueue<pair<int, int>> binomialQueue;  // 创建BinomialQueue堆
    dij(Graph, binomialQueue, sr, dt);            // 执行Dijkstra算法
    tmp_time[4] += clock() - t;
  }
  // 输出当前图的每种堆的时间
  outFile << gr << ":" << endl;
  for (int i = 0; i < HEAP_TYPE; ++i) {
    outFile << heap_type[i] << " " << tmp_time[i] << endl;
    time_total[i] += tmp_time[i];  // 累加到总时间
  }
}

// 主函数
int main() {
  for (int i = 0; i < SIZE; ++i) {  // 对每个图和查询文件执行solve函数
    solve(path + graph[i], path + query[i]);
  }
  cout << "total_time:" << endl;         // 输出总时间
  for (int i = 0; i < HEAP_TYPE; ++i) {  // 输出每种堆的总时间
    outFile << heap_type[i] << " " << time_total[i] << endl;
  }
}