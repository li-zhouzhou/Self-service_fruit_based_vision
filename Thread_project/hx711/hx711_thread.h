#ifndef    _HX711_APP_H
#define    _HX711_APP_H

#include <stdio.h>
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include<pthread.h>

#include "../main/main_a.h"

void* hx711(void* thread_arg);

#endif