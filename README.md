# 0-1背包问题求解器：分支限界法实现

## 📌 项目概述

本项目采用**分支限界法**实现了经典的0-1背包问题的高效求解方案。使用C语言编写，能够在给定背包容量和物品集合的情况下，找出总重量不超过容量且总价值最大的物品组合。

**核心特性：**
- 🚀 优化的分支限界算法，带有贪心策略的上界估计
- 📊 支持动态输入物品的重量、价值和计算性价比
- 🧠 智能剪枝减少搜索空间
- 📝 清晰的选中物品及总价值/重量输出

## 🛠 安装与使用

### 环境要求
- GCC或其他C编译器(CLion,vscode)
- GNU Make（可选）

### 编译项目
```bash
git clone https://github.com/yourusername/knapsack-branch-and-bound.git
cd 0-1-Knapsack-problem
make  # 或手动编译：gcc -o knapsack main.c
```

### 运行程序
```bash
./0-1-Knapsack-problem
```
按照交互提示：
1. 输入物品数量和背包容量
2. 输入每个物品的详细信息（ID、重量、价值）

## 🧮 算法详解

### 分支限界法实现
求解器包含：
1. **深度优先搜索**的递归分支
2. **贪心上界**计算用于剪枝
3. **回溯法**探索所有有效组合

```c
void knapsack_branch_and_bound(int level, int current_weight, 
                             int current_value, Item *sorted_items,
                             int n, int W, int *current_select_item) {
}
```

### 优化技术
- 物品按价值重量比预排序
- 提前终止无潜力的分支
- 高效的内存管理

## 📊 输入输出示例

**输入：**
```
输入物品数量和背包容量(n,W): 3,50
输入物品(id,重量,价值,性价比):
0,10,60,0
0,20,100,0
0,30,120,0
```

**输出：**
```
最大价值：220
选中的物品：
ID 1(第2个商品)：重量=20，价值=100
ID 2(第3个商品)：重量=30，价值=120
总重量：50
```

## 📂 项目结构

```
.
├── knapsack.c           # 完整代码
├── Makefile             # 构建配置
└── README.md            # 本文档
```

## 🤝 贡献指南

欢迎贡献！请：
1. Fork本仓库
2. 创建您的特性分支
3. 提交Pull Request
