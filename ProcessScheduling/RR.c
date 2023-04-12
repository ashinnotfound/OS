#include <stdio.h>

//PCB结构体
struct PCB{
    char name[20];//进程名
    int arrivalTime;//到达时间
    int needTime;//需要运行时间
    int beginTime;//开始时间
    int usedTime;//已用CPU时间
    int finishTime;//结束时间
    char status;//进程状态 就绪W(Wait)、运行R(Run)、完成F(Finish)
};
//创建新进程
struct PCB createProcess(){
    printf("正在创建新进程\n");
    struct PCB newPCB;

    printf("请输入进程名:\n");
    getchar();//打消回车
    gets(newPCB.name);
    printf("请输入到达时间:\n");
    scanf("%d", &newPCB.arrivalTime);
    printf("请输入需要运行时间:\n");
    scanf("%d", &newPCB.needTime);

    newPCB.beginTime = -10086;
    newPCB.finishTime = -10086;
    newPCB.usedTime = 0;
    newPCB.status = 'W';

    printf("创建成功\n");
    return newPCB;
}

//将进程数组按FCFS原则排序
void sort(int processNum, struct PCB pcbs[]){
    //无脑冒泡
    for (int i = 0; i < processNum; i++){
        for (int j = 0; j < processNum - i; j++){
            if (pcbs[j].arrivalTime > pcbs[j+1].arrivalTime){
                struct PCB temp = pcbs[j];
                pcbs[j] = pcbs[j+1];
                pcbs[j+1] = temp;    
            }
        }
    }
}

//打印进程信息
void print(struct PCB pcbs[], int num){
    printf("当前进程信息如下:\n");
    printf("进程名\t到达时间\t需要运行时间\t开始时间\t已用CPU时间\t结束时间\t进程状态\n");
    for (int i = 0; i < num; i++){
        printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%c\n", 
            pcbs[i].name, pcbs[i].arrivalTime, pcbs[i].needTime, pcbs[i].beginTime, pcbs[i].usedTime, pcbs[i].finishTime, pcbs[i].status);
    }
    printf("\n\n");
}

//计算带权周转时间
float getWeightedTurnaroundTime(struct PCB pcb){
    return (pcb.finishTime - pcb.arrivalTime)*1.0/pcb.needTime;
} 

//时间片轮转调度函数
void work(int timeSlice, int processNum, struct PCB pcbs[]){
    int time = 0;
    int currentSlice = 0;
    int index = 0;
    int over = 0;

    printf("当前时间:%d\n", time);
    print(pcbs, processNum);

    //空转到第一个进程进入cpu的时间
    while (1){
        if (pcbs[index].arrivalTime == time && pcbs[index].status == 'W'){
            //若就绪且到达，则将当前时间片分配给该进程
            break;
        }else{
            time++;
        }
    }
    
    while (1){
        pcbs[index].status = 'R';
        if (!pcbs[index].usedTime){
            pcbs[index].beginTime = time;
        }
        pcbs[index].usedTime++;
        time++;
        currentSlice++;

        if (pcbs[index].usedTime == pcbs[index].needTime){
            //如果进程完成
            pcbs[index].status = 'F';
            pcbs[index].finishTime = time;
            printf("当前时间:%d\n", time);
            print(pcbs, processNum);
            //重置时间片
            currentSlice = 0;
            int next = (index+1)%processNum;
            while (pcbs[next].status != 'W'){
                next = (next+1)%processNum;
                if (next == index){
                    over = 1;
                    break;
                }
            }
            if (over){
                printf("所有进程运行结束\n");
                print(pcbs, processNum);
                printf("各进程带权周转时间:\n");
                float wholeWeightedTurnaroundTime = 0;
                for (int i = 0; i < processNum; i++){
                    float temp = getWeightedTurnaroundTime(pcbs[i]);
                    wholeWeightedTurnaroundTime += temp;
                    printf("%s: %f\t", pcbs[i].name, temp);
                }
                printf("\n系统带权周转时间:%f\n", wholeWeightedTurnaroundTime/processNum);
                break;
            }
            index = next;
        }else if (currentSlice == timeSlice){
            //用完一个时间片，剥夺进程
            printf("当前时间:%d\n", time);
            print(pcbs, processNum);
            currentSlice = 0;
            pcbs[index].status = 'W';
            index = (index+1)%processNum;
            while (pcbs[index].status != 'W'){
                index = (index+1)%processNum;
            }
        }else {
            //无事发生
            printf("当前时间:%d\n", time);
            print(pcbs, processNum);
        }
    }
}

int main(){
    printf("请输入时间片大小:\n");
    int timeSlice;
    scanf("%d", &timeSlice);
    printf("请输入进程数量:\n");
    int processNum;
    scanf("%d", &processNum);
    struct PCB pcbs[processNum];
    for (int i = 0; i < processNum; i++){
        pcbs[i] = createProcess();
    }
    sort(processNum, pcbs);
    
    work(timeSlice, processNum, pcbs);
    
    return 0;
}
