#ifndef _WT_DEBUG_DEBUG_H_
#define _WT_DEBUG_DEBUG_H_

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#define		LV_ERR				0		//<推荐使用>  运行过程中出现的错误,对系统正常运行有影响
#define		LV_WARN				1		// 警告信息
#define		LV_INFO				2		//<推荐使用>  提示信息
#define		LV_DEBUG			3		//<推荐使用>  调试信息
#define		LV_NONE			    4		//<推荐使用>  不输出

#define   LOG_TO_FILE  1
#define		LV_LOG			    5		//<推荐使用>  输出到log文件

extern FILE *log_file_fp;
extern const char log_file_path[];
extern int Debug_Level;

//static FILE *log_file_fp ;
//static const char log_file_path[] = "log.log";

//static void DebugInit();
//static void DebugDestroy();

static inline unsigned int get_tid(){
#ifdef __APPLE__
    return (unsigned int) pthread_self();
#else
    return (unsigned int) syscall(__NR_gettid);
#endif

}

#if LOG_TO_FILE

    static inline void DebugInit() {
        log_file_fp = fopen(log_file_path, "w+");
        if(log_file_fp == NULL){
            printf("creat log_file_fp: %s error!\n",log_file_path);
            //exit(0);
        }
    }

    static inline void DebugDestroy() {
        fclose(log_file_fp);
    }

#else
    static inline void DebugInit() {}
    static inline void DebugDestroy() {}
#endif

static inline void SetDebugLevel(int level) {
    Debug_Level = level;
}

// #define 	Debug_Level   3 //限制调试信息出处的宏

static const char *Debuginfo[] = {
    "Error", "Warning", "Info", "Debbug", "None", "LOG"
};

#define MYDEBUG

#ifdef MYDEBUG
#define MYPRINT printf


//	程序中请使用 DBG_PRINT 宏
#define DBG_PRINT(level, format, a...)  \
do{                                 \
    time_t t;\
	struct tm* local;\
	t = time(NULL);\
	local = localtime(&t);\
	if (level <= Debug_Level)  {    \
        MYPRINT("%-7s: %-20s %3d: "#format"\n", Debuginfo[level], __FUNCTION__, __LINE__, ##a);\
        fflush(stdout);\
	}\
    else if(LOG_TO_FILE && level == LV_LOG) {  \
        if(log_file_fp == NULL) MYPRINT("[%-15s][%3d]:log_file_fp error!\n",  __FUNCTION__, __LINE__);  \
        else { \
            fprintf(log_file_fp, "[%4d-%02d-%02d %02d:%02d:%02d][%-18s][%4d][%5d]: " #format"\n", \
                local->tm_year+1900, local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec, __FUNCTION__, __LINE__, get_tid(), ##a); \
            fflush(log_file_fp);  \
        }\
    }\
}while(0)	

#else

#define DBG_PRINT(level, format, a...)

#endif

#endif 