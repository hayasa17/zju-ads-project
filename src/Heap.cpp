#include <vector>
#include <limits>
#include <queue>
#include <deque>
#include <list>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <memory>
using namespace std;

template<typename T>
class Heap {
public:
    virtual void insert(T value) = 0;
    virtual T deleteMin() = 0;
    virtual bool isEmpty() const = 0;
    virtual ~Heap() {}
};


template<typename T>
class STLVectorHeap : public Heap<T> {
private:
    std::priority_queue<T, std::vector<T>, std::greater<T>> pq;

public:
    void insert(T value) override {
        pq.push(value);
    }

    T deleteMin() override {
        T minValue = pq.top();
        pq.pop();
        return minValue;
    }

    bool isEmpty() const override {
        return pq.empty();
    }

    ~STLVectorHeap() {
        while (!pq.empty()) {
            pq.pop();
        }
    }


};

template<typename T>
class STLDequeHeap : public Heap<T> {
private:
    std::priority_queue<T, std::deque<T>, std::greater<T>> pq;

public:
    void insert(T value) override {
        pq.push(value);
    }

    T deleteMin() override {
        T minValue = pq.top();
        pq.pop();
        return minValue;
    }

    bool isEmpty() const override {
        return pq.empty();
    }

};




template<typename T>
struct LeftistNode {
    T value;              // 节点值
    LeftistNode* left;     // 左子树
    LeftistNode* right;    // 右子树
    int npl;              // 最短路径长度（Null Path Length）

    LeftistNode(T val) : value(val), left(nullptr), right(nullptr), npl(0) {}
};

// 左偏堆实现，继承自抽象堆类
template<typename T>
class LeftistHeap : public Heap<T> {
private:
    LeftistNode<T>* root;  // 左偏堆的根节点

    // 合并两个左偏堆的辅助函数
    LeftistNode<T>* _merge(LeftistNode<T>* h1, LeftistNode<T>* h2) {
        if (h1 == nullptr) return h2;
        if (h2 == nullptr) return h1;

        // 保证 h1 的根节点小于 h2 的根节点
        if (h1->value > h2->value)
            std::swap(h1, h2);

        // 递归合并 h1 的右子堆和 h2
        h1->right = _merge(h1->right, h2);

        // 检查左偏堆性质，调整左右子树
        if (h1->left == nullptr || (h1->right && h1->left->npl < h1->right->npl))
            std::swap(h1->left, h1->right);

        // 更新最短路径长度
        if (h1->right == nullptr)
            h1->npl = 0;
        else
            h1->npl = h1->right->npl + 1;

        return h1;
    }

public:
    // 构造函数
    LeftistHeap() : root(nullptr) {}

    // 插入操作
    void insert(T value) override {
        // 创建一个新节点并合并到堆中
        LeftistNode<T>* newNode = new LeftistNode<T>(value);
        root = _merge(root, newNode);
    }

    // 删除最小值操作
    T deleteMin() override {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }

        T minValue = root->value;   // 根节点就是最小值
        LeftistNode<T>* oldRoot = root;

        // 合并左右子堆
        root = _merge(root->left, root->right);

        delete oldRoot;  // 删除旧根节点
        return minValue;
    }

    // 判断堆是否为空
    bool isEmpty() const override {
        return root == nullptr;
    }

    LeftistHeap<T> merge(LeftistHeap<T>& other) {
        _merge(root, other.root);
        other.root = nullptr;
        return *this;
    }

    // 销毁堆的所有节点
    ~LeftistHeap() {
        while (!isEmpty()) {
            deleteMin();  // 清空堆
        }
    }
};





template<typename T>
class FibonacciHeap : public Heap<T> {
private:
    struct Node {
        T value;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        int degree;   // 子树的数量
        bool mark;    // 标记位

        explicit Node(T val) : value(val), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false) {}
    };

    Node* minNode;    // 指向当前最小值节点
    int numNodes;     // 堆中节点的总数量

    // 辅助函数：将节点插入根列表
    void insertNodeIntoRootList(Node* node) {
        if (!minNode) {
            minNode = node;
        } else {
            // 插入节点到根链表（双向循环链表）
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;

            if (node->value < minNode->value) {
                minNode = node;
            }
        }
    }

    // 辅助函数：合并同样度数的树
    void consolidate() {
        int maxDegree = static_cast<int>(std::log2(numNodes)) + 1;
        std::vector<Node*> degreeTable(maxDegree, nullptr);

        std::list<Node*> rootNodes;
        Node* current = minNode;
        if (current) {
            do {
                rootNodes.push_back(current);
                current = current->right;
            } while (current != minNode);
        }

        for (Node* root : rootNodes) {
            int d = root->degree;
            while (degreeTable[d]) {
                Node* other = degreeTable[d];
                if (root->value > other->value) {
                    std::swap(root, other);
                }
                link(other, root);  // 合并两个树
                degreeTable[d] = nullptr;
                d++;
            }
            degreeTable[d] = root;
        }

        minNode = nullptr;
        for (Node* node : degreeTable) {
            if (node) {
                if (!minNode) {
                    minNode = node;
                } else {
                    insertNodeIntoRootList(node);
                    if (node->value < minNode->value) {
                        minNode = node;
                    }
                }
            }
        }
    }

    // 辅助函数：将子树 `child` 链接到树 `parent` 中
    void link(Node* child, Node* parent) {
        // 从根列表中移除 child
        child->left->right = child->right;
        child->right->left = child->left;
        child->parent = parent;

        // 插入 child 到 parent 的子列表中
        if (!parent->child) {
            parent->child = child;
            child->left = child;
            child->right = child;
        } else {
            child->left = parent->child;
            child->right = parent->child->right;
            parent->child->right->left = child;
            parent->child->right = child;
        }

        parent->degree++;
        child->mark = false;
    }

    // 辅助函数：切割节点并移到根列表中
    void cut(Node* node, Node* parent) {
        if (node->right == node) {
            parent->child = nullptr;
        } else {
            node->left->right = node->right;
            node->right->left = node->left;
            if (parent->child == node) {
                parent->child = node->right;
            }
        }
        parent->degree--;

        insertNodeIntoRootList(node);
        node->parent = nullptr;
        node->mark = false;
    }

    // 辅助函数：级联剪切
    void cascadingCut(Node* node) {
        Node* parent = node->parent;
        if (parent) {
            if (!node->mark) {
                node->mark = true;
            } else {
                cut(node, parent);
                cascadingCut(parent);
            }
        }
    }

public:
    // 构造函数
    FibonacciHeap() : minNode(nullptr), numNodes(0) {}

    // 插入一个元素到堆中
    void insert(T value) override {
        Node* newNode = new Node(value);
        insertNodeIntoRootList(newNode);
        numNodes++;
    }

    // 删除并返回最小元素
    T deleteMin() override {
        if (!minNode) throw std::runtime_error("Heap is empty");

        Node* oldMin = minNode;
        T minValue = oldMin->value;

        // 将最小节点的子节点移到根列表
        if (oldMin->child) {
            Node* child = oldMin->child;
            do {
                Node* nextChild = child->right;
                insertNodeIntoRootList(child);
                child->parent = nullptr;
                child = nextChild;
            } while (child != oldMin->child);
        }

        // 从根列表中移除最小节点
        if (oldMin == oldMin->right) {
            minNode = nullptr;
        } else {
            minNode = oldMin->right;
            oldMin->left->right = oldMin->right;
            oldMin->right->left = oldMin->left;
            consolidate();  // 重组树结构
        }

        numNodes--;
        delete oldMin;
        return minValue;
    }

    // 判断堆是否为空
    bool isEmpty() const override {
        return minNode == nullptr;
    }

    // 减少节点的值
    void decreaseKey(Node* node, T newValue) {
        if (newValue > node->value) {
            throw std::invalid_argument("New key is greater than current key");
        }

        node->value = newValue;
        Node* parent = node->parent;

        if (parent && node->value < parent->value) {
            cut(node, parent);
            cascadingCut(parent);
        }

        if (node->value < minNode->value) {
            minNode = node;
        }
    }

    // 删除节点
    void deleteNode(Node* node) {
        decreaseKey(node, std::numeric_limits<T>::min());
        deleteMin();
    }

    ~FibonacciHeap() override {
    }
};

template<typename T>
class BinomialQueue : public Heap<T> {
private:
    struct Node {
        T element;
        int degree;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> child;
        std::shared_ptr<Node> sibling;

        Node(T elem) : element(elem), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    std::shared_ptr<Node> root; // 指向根节点的指针
    int size; // 队列中的元素个数

    // Helper functions
    std::shared_ptr<Node> mergeTrees(std::shared_ptr<Node> t1, std::shared_ptr<Node> t2) {
        if (!t1 || !t2) {
            //std::cout << "Warning: Trying to merge null trees!" << std::endl;
            return t1 ? t1 : t2;
        }

        // 合并两棵度数相同的树，较小的根成为新的根
        if (t1->element > t2->element) {
            std::swap(t1, t2);
        }

        // 打印调试信息
        //std::cout << "Merging trees with roots " << t1->element << " and " << t2->element << std::endl;

        // t2 变成 t1 的子树
        t2->sibling = t1->child;
        t1->child = t2;
        t1->degree++;
        return t1;
    }

    void linkTrees(std::vector<std::shared_ptr<Node>>& trees) {
        for (size_t i = 0; i < trees.size(); ++i) {
            if (trees[i]) {
                // 打印调试信息
                //std::cout << "Linking tree with root: " << trees[i]->element << std::endl;
                
                // 找到相同度数的树进行合并
                for (size_t j = i + 1; j < trees.size(); ++j) {
                    if (trees[j]) {
                        if (trees[i]->degree == trees[j]->degree) {
                            trees[i] = mergeTrees(trees[i], trees[j]);
                            trees[j].reset(); // 清空已合并的树
                        }
                    }
                }
            }
        }
    }

public:
    BinomialQueue() : root(nullptr), size(0) {}

    // 插入值
    void insert(T value) override {
        auto newNode = std::make_shared<Node>(value);

        // 打印调试信息
        //std::cout << "Inserting value: " << value << std::endl;

        std::vector<std::shared_ptr<Node>> trees(1, newNode);
        linkTrees(trees);

        // 合并新树与当前队列的树
        if (!root) {
            root = newNode;
        } else {
            root = mergeTrees(root, newNode);
        }

        size++;

        // 打印调试信息
        //std::cout << "Insert complete. Current root is: " << root->element << std::endl;
    }

    // 删除最小值
    T deleteMin() override {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }

        // 找到最小的根节点
        std::shared_ptr<Node> minNode = root;
        std::shared_ptr<Node> minNodePrev = nullptr;
        std::shared_ptr<Node> current = root;
        std::shared_ptr<Node> prev = nullptr;

        while (current) {
            if (current->element < minNode->element) {
                minNode = current;
                minNodePrev = prev;
            }
            prev = current;
            current = current->sibling;
        }

        // 打印调试信息
        //std::cout << "Minimum value found: " << minNode->element << std::endl;

        // 移除最小节点
        if (minNodePrev) {
            minNodePrev->sibling = minNode->sibling;
        } else {
            root = minNode->sibling;
        }

        // 将最小节点的孩子节点加入到根列表中
        std::shared_ptr<Node> child = minNode->child;
        std::vector<std::shared_ptr<Node>> trees;
        while (child) {
            auto next = child->sibling; // 记录下一个孩子
            child->sibling = nullptr; // 断开链接
            trees.push_back(child);
            child = next;
        }

        // 打印调试信息
        //std::cout << "Merging child trees after deleting minimum node." << std::endl;

        // 合并树
        for (auto tree : trees) {
            if (tree) {
                root = mergeTrees(root, tree);
            }
        }

        T minValue = minNode->element;
        size--;

        // 打印调试信息
        //std::cout << "DeleteMin complete. New root is: " << (root ? std::to_string(root->element) : "null") << std::endl;

        return minValue;
    }

    // 检查是否为空
    bool isEmpty() const override {
        return size == 0;
    }

    // 获取当前堆的大小
    int getSize() const {
        return size;
    }

    ~BinomialQueue() override {
    }
};



