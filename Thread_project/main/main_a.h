#ifndef    _MAIN_A_H
#define    _MAIN_A_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

static pthread_mutex_t m_mutex; //定义互斥锁
static pthread_cond_t cond; //定义条件变量

struct thread_data
{
    char type[20];
    volatile int weight;
    char last_type[20];
    volatile int last_weight;
};

struct thread_data data;

#endif