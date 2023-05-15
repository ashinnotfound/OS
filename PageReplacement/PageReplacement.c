#include <stdio.h>
#include <stdlib.h>

enum METHOD{
    FIFO = 1,
    LRU,
    OPT
}method;
// 缺页次数(置换次数)
int replaceCount;
// 内存块
struct block {
    // 当前内存块页号
    int page;

    struct block* next;
};
typedef struct block* BLOCK;
BLOCK head;

void init() {
    replaceCount = 0;
    head = NULL;
    printf("请输入要使用的页面置换算法：1.FIFO 2.LRU 3.OPT\n");
    scanf("%d", &method);
}

// 判断内存块中是否有页号
int check(int pageNum) {
    BLOCK p = head;
    int count = 0;
    while (p != NULL) {
        if (pageNum == p->page) {
            return count;
        }
        count++;
        p = p->next;
    }
    return -1;
}

void showBlock() {
    printf("当前4个内存块中的页面引号为:");
    BLOCK p = head;
    while (p != NULL) {
        printf("%d ", p->page);
        p = p->next;
    }
    printf("\t");
}

// 先进先出算法
void fifo(int pageNum) {
    // 如果所访问的指令不在内存
    if (check(pageNum) == -1) {
        // 记录缺页的次数
        replaceCount++;
    
        // 将当前页调入内存
        BLOCK current = (BLOCK)malloc(sizeof(struct block));
        current->page = pageNum;
        current->next = NULL;

        BLOCK p = head;
        int blockNum = 0;

        if (p == NULL) {
            head = current;
        } else {
            // 找到尾内存块
            while (p->next != NULL) {
                p = p->next;
                blockNum++;
            }
            p->next = current;
            // 如果4个内存块均已装入该作业
            if (blockNum == 3) {
                BLOCK toFree = head;
                head = head->next;
                free(toFree);
                printf("😢发生第%d次页面置换😢\n", replaceCount);
            }
        }
    }
}
// 最近最久未使用算法
void lru(int pageNum){
    int index = check(pageNum);

    // 如果访问的页在内存
    if (index != -1){
        if (index !=0 && head->next != NULL){            
            BLOCK current = head->next;
            BLOCK behind = head;
            for (int i = 1; i < index; i++){
                current = current->next;
                behind = behind->next;
            }
            behind->next = current->next;
            current->next = head;
            head = current;
        }
    }else{
        // 如果访问的页不在内存
        replaceCount++;
        printf("😢发生第%d次页面置换😢\n", replaceCount);

        BLOCK current = (BLOCK)malloc(sizeof(struct block));
        current->page = pageNum;
        current->next = head;
        head = current;

        //判断是否需要释放链表尾部
        int count = 0;
        BLOCK p = head;
        while (p->next != NULL){
            if (count == 3){
                free(p->next);
                p->next = NULL;
                return;
            }
            p = p->next;
            count++;
        }
    }
}
// 最佳置换算法
void opt(int pageNum){
    // 所选择的被换出的页面将是最长时间内不再被访问，通常可以保证获得最低的缺页率。
    // 是一种理论上的算法，因为无法知道一个页面多长时间不再被访问。
    // 但在此处等价于fifo，因为顺序访问页面，故最长时间内不再被访问的页面就是最先进入内存的页面
    fifo(pageNum);
}

void work() {
    for (int i = 0; i < 320; i++) {
        // 页号
        int pageNum = i / 10;
        // 页内索引
        int index = i % 10;
        switch (method){
            case FIFO :
                fifo(pageNum);
                break;
            case LRU :
                lru(pageNum);
                break;
            case OPT :
                opt(pageNum);
                break;
            default :
                printf("错误的算法选择😶‍🌫️,程序终止\n");
                return;
        }
        showBlock();
        // 假设一条指令占8个地址长度
        printf("成功访问第%d条指令,其物理地址为:%d\n", i + 1, i * 8);
    }
    printf("执行完毕，共完成%d次页面置换，缺页率为：%f\n", replaceCount, replaceCount / 320.0);
}

void main() {
    init();
    work();
}