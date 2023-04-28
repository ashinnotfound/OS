//首次适应
#include <stdio.h>
#include <stdlib.h>

//空闲分区链结构
struct block{
    //分区起始地址
    int head;
    //分区大小
    int size;
    //指向上一个
    struct block* last;
    //指向下一个
    struct block* next;
};
typedef struct block BLOCK;

//作业
struct work{
    //作业被分配空间的起始地址
    int head;
    //作业请求空间
    int requestSize;
};
typedef struct work WORK;

//指向第一个分区
BLOCK* firstBlock = NULL;
//指向最后一个分区
BLOCK* lastBlock = NULL;
//用于存储作业对应的空间起始地址、请求空间大小
WORK* workIndex[100];

void info(){
    //打印分区
    printf("当前空闲分区链情况：\n");
    printf("分区id\t分区起始地址\t分区大小\n");
    BLOCK* p = firstBlock;
    int count = 1;
    while (p != NULL){
        printf("%d\t\t%d\t\t%d\n", count++, p->head, p->size);
        p = p->next;
    }
}

//初始化，将内存空间分配为若干分区
void init(int wholeSize, int defaultBlockSize){
    printf("正在初始化\n");
    int num = wholeSize / defaultBlockSize;

    if (num == 0){
        //如果默认分区大小太大，则只能分配一个分区
        firstBlock = (BLOCK*)malloc(sizeof(BLOCK));
        firstBlock->head = 0;
        firstBlock->size = wholeSize;
        firstBlock->last = NULL;
        firstBlock->next = NULL;
        lastBlock = firstBlock;
    }else{
        BLOCK* p = NULL;
        for (int i = 1; i <= num; i++){
            //块号从1开始
            BLOCK* current = (BLOCK*)malloc(sizeof(BLOCK));
            current->head = (i-1) * defaultBlockSize;
            current->size = defaultBlockSize;
            if (firstBlock == NULL){
                firstBlock = current;
                firstBlock->last = NULL;
            }
            if (p != NULL){
                current->last = p;
                p->next = current;
            }
            p = current;
        }
        //判断尾部是否有漏掉的内存
        if (p->head + p->size < wholeSize){
            BLOCK* current = (BLOCK*)malloc(sizeof(BLOCK));
            current->head = p->head + p->size;
            current->size = wholeSize - current->head;
            current->last = p;
            p ->next = current;
            p = current;
        }
        p->next = NULL;
        lastBlock = p;
    }
    
    printf("初始化成功!\n");
    info();
}

void partition(int workId, int requestSize){
    printf("正在使用首次适应算法为 作业%d 分配 %d 大小的空间\n", workId, requestSize);
    BLOCK * p = firstBlock;
    while (p != NULL){
        if (p->size >= requestSize){
            int result = p->head;

            if (p->size - requestSize == 0){
                //刚好全部分配完，释放该分区
                BLOCK* last = p->last;
                BLOCK* next = p->next;
                last->next = next;
                if (next != NULL){
                    next->last = last;
                }
                free(p);
            }else{
                p->head += requestSize;
                p->size -= requestSize;
            }

            WORK* work = (WORK*)malloc(sizeof(WORK));
            work->head = result;
            work->requestSize = requestSize;
            workIndex[workId] = work;

            printf("成功为 作业%d 分配 %d大小 的空间,起始地址为 %d\n", workId, requestSize, result);
            info();
            break;
        }
        p = p->next;
    }
    if (p == NULL){
        printf("分配失败：空间不足\n");
    }
}

//回收空间
void recovery(int workId){
    if (workIndex[workId] == NULL){
        printf("释放失败，并不存在作业id为 %d 的作业", workId);
        return;
    }

    printf("正在为 作业%d 释放空间\n", workId);

    //找到在哪个块
    WORK* work = workIndex[workId];
    BLOCK* p = firstBlock;

    //如果作业空间位于空闲分区链前
    if (work->head < p->head){
        //如果作业下一个为空闲区
        if (p->head == work->head + work->requestSize){
            p->head = work->head;
            p->size += work->requestSize;
        }else{
            BLOCK* current = (BLOCK*)malloc(sizeof(BLOCK));
            current->head = work->head;
            current->size = work->requestSize;
            current->last = NULL;
            current->next = firstBlock;

            firstBlock = current;
        }
    }else{
        //如果作业在空闲分区链中或与lastBlock相邻
        while (p != NULL){
            if (p->head + p->size == work->head){
                BLOCK* next = p->next;
                if (next != NULL && work->head + work->requestSize == next->head){
                    //作业与后一空闲分区相邻
                    p->size += (work->requestSize + next->size);
                    p->next = next->next;
                    free(next);
                }else{
                    //不相邻
                    p->size += work->requestSize;
                }
                break;
            }
            p = p->next;
        }
        if (p == NULL){
            //作业在空闲分区链之后且不相邻
            BLOCK* current = (BLOCK*)malloc(sizeof(BLOCK));
            current->head = work->head;
            current->size = work->requestSize;
            current->last = lastBlock;
            current->next = NULL;

            lastBlock = current;
        }
    }
    free(work);

    printf("作业%d 释放成功\n", workId);
    info();
}

void main(){
    int defaultBlockSize = 0;
    printf("请输入初始状态时一个分区分配的大小:\n");
    scanf("%d", &defaultBlockSize);

    init(640, defaultBlockSize);

    partition(1, 130);
    partition(2, 60);
    partition(3, 100);
    recovery(2);
    partition(4, 200);
    recovery(3);
    recovery(1);
    partition(5, 140);
    partition(6, 60);
    partition(7, 50);
    partition(8, 60);
}