//
// Created by lisisi on 2025/6/15.
//

#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int id; // �����ԭʼ��������ʼ��id��0
    int weight;
    int value;
    double ratio; // ��Ʒ���Լ۱� (value/weight)
}Item;

// ��¼��ǰ״̬�µ����ֵ
int max_profit_global = 0;
// ��̬���ֱ��ÿ����Ʒ�Ƿ�ѡ��
int* best_selection_global = NULL;
// ��Ʒ��������Ŀ���ǹ���best_selection_global�Ĵ�С
int num_items_global = 0;

// compareItem �������ڸ��ݼ�ֵ�ܶȶԣ���Ʒ��������
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

    // ѡ����Ʒ

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

    // ��ѡ����Ʒ

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
        printf("��������Ϊ0,������Ʒ����Ϊ0��");
        exit(1);
    }

    Item *sorted_items = (Item *)malloc(n*sizeof(Item));
    if (sorted_items==NULL) {
        printf("�ڴ����ʧ�ܣ�");
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
        printf("�ڴ��������");
        free(sorted_items);
        exit(1);
    }

    int *current_select_item=(int *)malloc(n*sizeof(int *));
    int total_select_weight=0;

    if (current_select_item==NULL) {
        printf("�ڴ����ʧ�ܣ�");
        free(sorted_items);
        exit(1);
    }

    knapsack_branch_and_bound(0,0,0,sorted_items,n,W,current_select_item);

    printf("���ļ�ֵ�ǣ�%d\n",max_profit_global);
    printf("ѡȡ����Ʒ�ֱ��ǣ�\n");
    for (int i=0; i<n; i++) {
        if (*(best_selection_global+i)==1) {
            printf("��Ʒid��%d(��%d����Ʒ)����Ʒweight��%d����Ʒvalue��%d\n",i,i+1,(original_items+i)->weight,(original_items+i)->value);
            total_select_weight+=(sorted_items+i)->weight;
        }
    }

    printf("��Ʒ��������Ϊ%d",total_select_weight);

    free(sorted_items);
    free(current_select_item);
    free(best_selection_global);

}

int main() {
    int n,W,id,weight,value,i=0;
    double ratio;
    printf("������Ʒ�������ͱ����������ǣ�n,m����");
    scanf("%d,%d",&n,&W);

    Item *items = (Item *)malloc(sizeof(Item) * n);
    if (items==NULL) {
        printf("�ڴ����ʧ�ܣ�");
        exit(1);
    }

    printf("������(id,weight,value,ratio)\n");

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