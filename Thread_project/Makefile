target = project

GCC = gcc
ARM = arm-linux-gnueabihf-gcc
CC = $(ARM)

CFLAGS = -lpthread

source1 = ./main/
source2 = ./print_thread1/
source3 = ./socker/
source4 = ./hx711/
source5 = ./sound/

#由于 .c文件在不同的文件下，那么匹配模式 要匹配不同文件下的%.c文件
src1 = $(wildcard $(source1)*.c)
src2 = $(wildcard $(source2)*.c)
src3 = $(wildcard $(source3)*.c)
src4 = $(wildcard $(source4)*.c)
src5 = $(wildcard $(source5)*.c)
obj1 = $(patsubst %.c,%.o,$(src1))
obj2 = $(patsubst %.c,%.o,$(src2))
obj3 = $(patsubst %.c,%.o,$(src3))
obj4 = $(patsubst %.c,%.o,$(src4))
obj5 = $(patsubst %.c,%.o,$(src5))


$(target):$(obj1) $(obj2) $(obj3) $(obj4) $(obj5)
	$(CC) $^ -o $@  $(CFLAGS)
#	gcc $(obj) -o $(target)

#编译 src下的.c文件 为.o文件	
%.o:$(source1)%.c
	$(CC) -c $< -o $@

%.o:$(source2)%.c
	$(CC) -c $< -o $@

%.o:$(source3)%.c
	$(CC) -c $< -o $@

%.o:$(source4)%.c
	$(CC) -c $< -o $@

%.o:$(source5)%.c
	$(CC) -c $< -o $@

clean:
	#删除当前工程的所有.o文件及目标project文件
	-rm $(target) $(obj1) $(obj2) $(obj3) $(obj4) $(obj5) -f


