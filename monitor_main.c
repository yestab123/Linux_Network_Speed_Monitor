#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

uint64_t *my_ipconfig(char *dev, char *ath0)
{


    int nDevLen = strlen(ath0);
    if (nDevLen < 1 || nDevLen > 100) {
        printf("dev length too long\n");
        return NULL;
    }

    int fd = open(dev, O_RDONLY | O_EXCL);
    if (-1 == fd) {
        printf("%s not exists!\n", dev);
        return NULL;
    }

    char buf[1024*2];
    lseek(fd, 0, SEEK_SET);
    int nBytes = read(fd, buf, sizeof(buf)-1);
    if (-1 == nBytes) {
        perror("read error");
        close(fd);
        return NULL;
    }
    close(fd);
    buf[nBytes] = '\0';

    //返回第一次指向ath0位置的指针
    char* pDev = strstr(buf, ath0);
    if (NULL == pDev) {
        printf("don't find dev %s\n", ath0);
        printf("Usage:./net_flow [NIC_name] (default use eth0, check by cmd ifconfig)\n");
        exit(0);
    }

    char *p;
    char *ifconfig_value;
    int i = 0, x = 0, mark = 0;
    char save[10];
    static uint64_t rx2_tx10[2];
    /* Find eth0:63335 and eth0: 6335  */
    for (x = 0; x < strlen(pDev); x++) {
        if (pDev[x] == ':') {
            mark = 1;
        }
        if ((mark == 1) && ((pDev[x] >= '0') && (pDev[x] <= '9'))) {
            mark = 2;
            break;
        }
    }

    if (mark != 2) {
        printf("%s format not right(%s)", dev, pDev);
        exit(0);
    }

    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev + x, " "); p; p = strtok(NULL, " ")) {
        i++;
        /* printf("%s\n", p); */
        ifconfig_value = (char*)malloc(20);
        strcpy(ifconfig_value, p);
        /*得到的字符串中的第1个字段是接收流量*/
        /* printf("%d:%s\n", i, ifconfig_value); */
        if(i == 1) {
            rx2_tx10[0] = atoll(ifconfig_value);
        }
        /*得到的字符串中的第9个字段是发送流量*/
        if(i == 9) {
            rx2_tx10[1] = atoll(ifconfig_value);
            break;
        }
        free(ifconfig_value);
    }
    /* printf("%s\n", pDev); */
    /* sscanf(pDev, "%s:%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u \n%s", */
    /*        p, &rx2_tx10[0], &i, &i,&i,&i,&i,&i,&i,&rx2_tx10[1], */
    /*        &i,&i,&i,&i,&i,&i,&i, p); */
    return rx2_tx10;
}

uint32_t
get_now_sec(void) {

    time_t sec;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    sec = spec.tv_sec;

    return sec;
}

#define SMOOTH_NUM 20
double
input_smooth(double speed) {
    static int i = 0;
    int j;
    static double t[SMOOTH_NUM];
    double rate = 0;

    t[i] = speed;
    i++;
    if (i >= SMOOTH_NUM) {
        i = 0;
    }

    for (j = 0; j < SMOOTH_NUM; j++) {
        rate += t[j];
    }

    return (rate/SMOOTH_NUM);
}

double
output_smooth(double speed) {
    static int i = 0;
    int j;
    static double t[SMOOTH_NUM];
    double rate = 0;

    t[i] = speed;
    i++;
    if (i >= SMOOTH_NUM) {
        i = 0;
    }

    for (j = 0; j < SMOOTH_NUM; j++) {
        rate += t[j];
    }

    return (rate/SMOOTH_NUM);
}

int
main(int argc, char **argv)
{

    uint64_t *ifconfig_result;
    double re_mb;
    char *dev_name;
    uint32_t last = 0;
    uint32_t pl;
    double speed;
    double in_past = 0;
    double out_past = 0;
    double in_now;
    double out_now;

    if (argc > 1) {
        dev_name = argv[1];
    } else {
        dev_name = "eth0";
    }

    while(1) {
        pl = get_now_sec() - last;
        if (pl > 1) {
            /*网卡名称*/
            ifconfig_result = my_ipconfig("/proc/net/dev", dev_name);

            /*保存在文件中的数值的单位是B，经过计算换算成MB*/
            in_now = (double)ifconfig_result[0]/1024;
            re_mb = (double)ifconfig_result[0]/(1024*1024);
            speed = (in_now - in_past)/pl;
            printf("Input: %0.2fMB  %0.2fKB/s Smooth_Speed:%0.2fKB/s | ",
                   re_mb, speed, input_smooth(speed));
            in_past = in_now;

            /*保存在文件中的数值的单位是B，经过计算换算成MB*/
            out_now = (double)ifconfig_result[1]/1024;
            re_mb = (double)ifconfig_result[1]/(1024*1024);
            speed = (out_now - out_past)/pl;
            printf("Output: %0.2fMB  %0.2fKB/s Smooth_Speed:%0.2fKB/s                   \r",
                   re_mb, speed, output_smooth(speed));
            out_past = out_now;
            fflush(stdout);
            last = get_now_sec();
        }
	sleep(1);
    }
}
