#include <stdio.h>
int num, sum, start;
int m = 0;
int n = 0;
int s[100];
int s1[100];
int c1[50];
int c2[50];

void create() {
    printf("请输入从哪个磁道开始：\t\n");
    scanf("%d", &start);
    printf("请输入磁道的个数：\n");
    scanf("%d", &num);
    for (int j = 0; j < num; j++) {
        printf("请输入第%d个磁道\n", j + 1);
        scanf("%d", &s[j]);
        for (int i = 0; i < j; i++)
            if (s[j] == s[i])
                j--;
    }

    printf("被访问的下一个磁道\n");
    for (int i = 0; i < num; i++) {
        printf("\t%d\t\n", s[i]);
    }

    int current = start;
    int t;
    for (int i = 0; i < num; i++)
        if (current > s[i])
            c1[m++] = s[i];
        else
            c2[n++] = s[i];
    for (int i = 0; i < m; i++)
        for (int j = i; j < m; j++)
            if (c1[i] < c1[j]) {
                t = c1[i];
                c1[i] = c1[j];
                c1[j] = t;
            }
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            if (c2[i] > c2[j]) {
                t = c2[i];
                c2[i] = c2[j];
                c2[j] = t;
            }
}

void FCFS() {
    printf("先来先服务 FCFS\n");
    printf("被访问的下一个磁道\t\t\t磁道号移动距离\n");
    int current = start;
    sum = 0;
    for (int i = 0; i < num; i++) {
        if (current < s[i])
            s1[i] = s[i] - current;
        else
            s1[i] = current - s[i];
        current = s[i];
        sum += s1[i];
    }

    for (int i = 0; i < num; i++) {
        printf("\t%d\t\t\t\t\t%d\t\t\n", s[i], s1[i]);
    }
    printf("寻道长度：%d\n", sum);
}

void SCAN() {
    printf("扫描算法 SCAN:\n");
    printf("被访问的下一个磁道：\t\t\t磁道号移动距离：\n");
    int current = start;
    int s2[100];
    sum = 0;
    for (int i = 0; i < n; i++) s2[i] = c2[i];
    for (int i = 0; i < m; i++) s2[i + n] = c1[i];
    for (int i = 0; i < num; i++) {
        if (current < s2[i])
            s1[i] = s2[i] - current;
        else
            s1[i] = current - s2[i];
        current = s2[i];
        sum += s1[i];
    }
    for (int i = 0; i < num; i++) {
        printf("\t%d\t\t\t\t\t%d\t\t\n", s2[i], s1[i]);
    }
    printf("寻道长度：%d\n", sum);
}

void main() {
    printf("1.创建磁道\t 2.先来先服务 FCFS\t 3.扫描算法 SCAN\t 4.退出 EXIT\n");
    int choice;

    scanf("%d", &choice);
    if (choice != 1 && choice != 4) {
        printf("请先创建磁道\n");
    }
    if (choice == 4) {
        printf("谢谢使用！");
    } else {
        create();
        printf("磁盘调度\n");
    P:
        printf("1.创建磁道\t 2.先来先服务 FCFS\t 3.扫描算法 SCAN\t 4.退出 EXIT\n");
        scanf("%d", &choice);
        if (choice > 4 || choice < 1) {
            goto P;
            printf("输入错误\n");
        }
        switch (choice) {
        case 2:
            FCFS();
            goto P;
        case 3:
            SCAN();
            goto P;
        case 4:
            printf("谢谢使用！");
            break;
        }
    }
}
