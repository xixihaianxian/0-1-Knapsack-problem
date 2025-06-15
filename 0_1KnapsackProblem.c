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

    printf("(id,weight,value,ratio)\n");

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