#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

struct timeval tv0;
FILE *pfile;
void timer1000() {
    struct timeval tv;
    int ds, dus;
    double dds;

    gettimeofday(&tv, NULL);
    ds = tv.tv_sec - tv0.tv_sec;
    dus = tv.tv_usec - tv0.tv_usec;
    dds = (ds + dus * 0.000001 - 1) * 1000;
    tv0 = tv;
    /*printf("sec: %-5d  us: %-8d  误差: %-10f毫秒\n", ds, dus, dds);*/
    fprintf(pfile, "误差: %15f毫秒\n", dds);
    fflush(pfile);
    printf("误差: %15f毫秒\n", dds);

    if (signal(SIGALRM, (void(*)(int))timer1000) == SIG_ERR) {
        fprintf(stderr, "未能捕获SIGALRM。\n");
        exit(1);
    }
}

// 
// 初始化定时器
//
void init_timer(double seconds) {
    struct itimerval it_val;  /* for setting itimer */
    int isec, ius;

    isec = (int)seconds;
    ius = (int)(1000000 * (seconds - isec));

    if (signal(SIGALRM, (void(*)(int))timer1000) == SIG_ERR) {
        fprintf(stderr, "未能捕获SIGALRM。\n");
        exit(1);
    }
    it_val.it_value.tv_sec = isec;
    it_val.it_value.tv_usec = ius;
    it_val.it_interval.tv_sec = isec;
    it_val.it_interval.tv_usec = ius;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        fprintf(stderr, "调用setitimer失败。\n");
        exit(1);
    }
    else {
        printf("启动定时器成功，定时间隔：%f秒。\n", seconds);
    }
}

int main(int argc, char *argv[])
{
    gettimeofday(&tv0, NULL);
    pfile = fopen("timeerr.dat", "w");
    init_timer(1.0);
    while (1)
        pause();

    return 0;
}