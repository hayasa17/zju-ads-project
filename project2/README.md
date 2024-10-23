## 简介

本实验实现了 Dijkstra 算法，并使用不同的堆数据结构作为其底层实现，然后对其进行性能比较。

- STLVectorHeap：使用 STL 中的 vector 作为堆，实现 Dijkstra 算法。
- STLDequeHeap：使用 STL 中的 deque 作为堆，实现 Dijkstra 算法。
- leftistHeap：实现了左式堆，并使用它作为堆，实现 Dijkstra 算法。
- fibonacciHeap：实现了斐波那契堆，并使用它作为堆
- binomial queue: 实现了二项队列，并使用它作为堆

## 文件构成

- src/：源代码目录
  - Heap.cpp: 堆的实现
  - dij.cpp: Dijkstra 算法的实现
  - test.cpp: 测试代码
  - README.md: 本文档
- data/：测试数据集

## 运行方法

1. 编译：在命令行中进入 src 目录，输入命令`g++ test.cpp dij.cpp Heap.cpp -o dij`，编译生成可执行文件`dij`。
2. 运行：在命令行中输入命令`./dij`，运行程序。

## 测试结果

程序会输出测试中各个堆实现 dijkstra 算法的运行时间。

## 错误处理

在执行过程中，如果出现错误，请检查以下内容：

- 确保您在正确的目录下执行编译和运行命令。
- 检查源代码文件是否完整。
- 确保在您的系统中已安装所需的编译器（如 g++）。
