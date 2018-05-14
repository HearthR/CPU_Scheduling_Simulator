#pragma once

#include <stdlib.h>
#include <time.h>


#define VP_ARRIVAL 1500
#define VP_CPU_BURST 1501
#define VP_PRIORITY 1502

typedef struct _vprocess
{
	int vprocess_id;
	int cpu_burst;
	int io_burst;
	int arrival_t;
	int p_priority;
	int cpu_remaining;
	int io_remaining;
} vprocess;

typedef vprocess* vprocess_ptr;

typedef struct _vpqueue
{
	vprocess_ptr* vp_arr;
	int front;
	int back;
} vpqueue;



vprocess_ptr createProcess(int num);
void vpQueuePush(vpqueue* vp_queue, vprocess_ptr vp, int size);
vprocess_ptr vpQueuePop(vpqueue* vp_queue, int size);
void vpQSort(vprocess_ptr* vp_arr, int left, int right, int size, int mode);
void vpSwap(vprocess_ptr* vp_arr, int i, int j);
