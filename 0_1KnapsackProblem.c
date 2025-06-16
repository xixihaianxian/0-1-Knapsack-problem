//
// Created by lisisi on 2025/6/15.
//

#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int id; // 物体的原始索引，初始的id是0
    int weight;
    int value;
    double ratio; // 商品的性价比 (value/weight)
}Item;

// 记录当前状态下的最大值
int max_profit_global = 0;
// 动态数字标记每个商品是否被选中
int* best_selection_global = NULL;
// 物品的总数，目的是管理best_selection_global的大小
int num_items_global = 0;

// compareItem 函数用于根据价值密度对，商品进行排序
int compareItems(const void *a, const void *b) {
    Item *itemA=(Item *)a;
    Item *itemB=(Item *)b;
    if (itemA->ratio>itemB->ratio) {
        return -1;
    }
    else if (itemA->ratio<itemB->ratio) {
        return 1;
    }
    else {
        return 0;
    }
}

double calculate_bound(int level, int current_weight, int current_value, Item *sorted_items, int n, int W) {
    if (current_weight>=W) {
        return 0;
    }

    double bound = (double)current_value;
    int remaining_capacity=W-current_weight;
    int k=level;

    while (k<n && (sorted_items+k)->weight<remaining_capacity) {
        remaining_capacity-=(sorted_items+k)->weight;
        bound+=(double)(sorted_items+k)->value;
        k++;
    }

    if (k<n && remaining_capacity>0) {
        bound+=(double)(sorted_items+k)->ratio*remaining_capacity;
    }

    return bound;
}

void knapsack_branch_and_bound(int level, int current_weight,
    int current_value, Item *sorted_items,
    int n, int W, int *current_select_item) {
    if (W>=current_weight && current_value>max_profit_global) {
        max_profit_global=current_value;

        for (int i=0; i<num_items_global; i++) {
            *(best_selection_global+i)=0;
        }

        for (int i=0; i<n; i++) {
            if (*(current_select_item+i)==1) {
                *(best_selection_global+(sorted_items+i)->id)=1;
            }
        }

    }

    double bound=calculate_bound(level,current_weight,current_value,sorted_items,n,W);

    if (bound<=max_profit_global || level==n) {
        return;
    }

    // 选择商品

    if (current_weight+sorted_items[level].weight<=W) {
        *(current_select_item+level)=1;
        knapsack_branch_and_bound(
            level+1,
            current_weight+(sorted_items+level)->weight,
            current_value+(sorted_items+level)->value,
            sorted_items,
            n,
            W,
            current_select_item
            );
        *(current_select_item+level)=0;
    }

    // 不选择商品

    knapsack_branch_and_bound(
        level+1,
        current_weight,
        current_value,
        sorted_items,
        n,
        W,
        current_select_item
        );
}

void display_item(Item *item,int n) {
    for (int i=0; i<n; i++) {
        printf("%d\t %d\t %d\t %lf\n",(item+i)->id,(item+i)->weight,(item+i)->value,(item+i)->ratio);
    }
}

void solve_knapsack(Item *original_items, int n, int W) {
    if (n==0||W==0) {
        printf("背包容量为0,或者商品数量为0！");
        exit(1);
    }

    Item *sorted_items = (Item *)malloc(n*sizeof(Item));
    if (sorted_items==NULL) {
        printf("内存分配失败！");
        exit(1);
    }

    for (int i=0; i<n; i++) {
        sorted_items[i]=original_items[i];
        (sorted_items+i)->id=i;
        if ((sorted_items+i)->weight>0) {
            (sorted_items+i)->ratio=(double)(sorted_items+i)->value/(sorted_items+i)->weight;
        }
        else {
            (sorted_items+i)->ratio=(sorted_items+i)->value>0?1e6:0;
        }
    }

    qsort(sorted_items,n ,sizeof(Item),compareItems);

    max_profit_global=0;
    num_items_global=n;
    best_selection_global=(int *)malloc(num_items_global*sizeof(int));

    if (best_selection_global==NULL) {
        printf("内存申请错误！");
        free(sorted_items);
        exit(1);
    }

    int *current_select_item=(int *)malloc(n*sizeof(int *));
    int total_select_weight=0;

    if (current_select_item==NULL) {
        printf("内存分配失败！");
        free(sorted_items);
        exit(1);
    }

    knapsack_branch_and_bound(0,0,0,sorted_items,n,W,current_select_item);

    printf("最大的价值是：%d\n",max_profit_global);
    printf("选取的商品分别是：\n");
    for (int i=0; i<n; i++) {
        if (*(best_selection_global+i)==1) {
            printf("商品id：%d(第%d个商品)，商品weight：%d，商品value：%d\n",i,i+1,(original_items+i)->weight,(original_items+i)->value);
            total_select_weight+=(sorted_items+i)->weight;
        }
    }

    printf("商品的总重量为%d",total_select_weight);

    free(sorted_items);
    free(current_select_item);
    free(best_selection_global);

}

int main() {
    int n,W,id,weight,value,i=0;
    double ratio;
    printf("输入商品的数量和背包的容量是（n,m）：");
    scanf("%d,%d",&n,&W);

    Item *items = (Item *)malloc(sizeof(Item) * n);
    if (items==NULL) {
        printf("内存分配失败！");
        exit(1);
    }

    printf("请输入(id,weight,value,ratio)\n");

    while (i<n){
        scanf("%d,%d,%d,%lf",&id,&weight,&value,&ratio);
        (items+i)->id=id;
        (items+i)->weight=weight;
        (items+i)->value=value;
        (items+i)->ratio=ratio;
        i+=1;
    }

    solve_knapsack(items,n,W);

    return 0;
}