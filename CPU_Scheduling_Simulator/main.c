#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

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
//bool isPidDuplicate(vprocess* vp, int current);
void scheduleFCFS(vprocess_ptr vp, int size);
void scheduleSJF(vprocess_ptr vp, int size);
void schedulePESJF(vprocess_ptr vp, int size);
void schedulePriority(vprocess_ptr vp, int size);
void vpQSort(vprocess_ptr* vp_arr, int left, int right, int size, int mode);
void vpSwap(vprocess_ptr* vp_arr, int i, int j);
void vpQueuePush(vpqueue* vp_queue, vprocess_ptr vp, int size);
vprocess_ptr vpQueuePop(vpqueue* vp_queue, int size);

int main()
{
	int psize = 0;
	vprocess *vp;
	printf("Number of process : ");
	scanf_s("%d", &psize);
	vp = createProcess(psize);
//	scheduleFCFS(vp, psize);
//	scheduleSJF(vp, psize);
//	schedulePriority(vp, psize);
	schedulePESJF(vp, psize);

	system("pause");
	return 0;
}

vprocess_ptr createProcess(int num)
{
	vprocess_ptr vp = (vprocess_ptr)calloc(sizeof(vprocess), num);

	srand(time(NULL));

	for (int i = 0; i < num; i++)
	{
		vp[i].vprocess_id = i + 1;
		vp[i].arrival_t = rand() % 40;
		vp[i].cpu_burst = rand() % 30 + 1;
		vp[i].cpu_remaining = vp[i].cpu_burst;
		vp[i].io_burst = rand() % 30 + 1;
		vp[i].io_remaining = vp[i].io_burst;
		vp[i].p_priority = rand() % 20 + 1;
	}

	printf("\n%d processes successfully created\n\n", num);
	printf("Process list:\n");
	for (int i = 0; i < num; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    Priority:%3d\n", i, vp[i].vprocess_id, vp[i].arrival_t, vp[i].cpu_burst, vp[i].p_priority);
	}
	printf("\n\n");

	return vp;
}

void scheduleFCFS(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };

	while (job_queue.back < size)
	{
		job_queue.vp_arr[job_queue.back] = &vp[job_queue.back];
		job_queue.back++;
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst);
	}
	printf("\n\n");
	printf("Gantt Chart:\n");

	while (job_queue.front != job_queue.back)
	{
		if (job_queue.vp_arr[job_queue.front]->arrival_t > cur_time)
		{
			printf("<Idle> Process running [%d ~ %d]\n", cur_time, job_queue.vp_arr[job_queue.front]->arrival_t);
			cur_time = job_queue.vp_arr[job_queue.front]->arrival_t;
		}

		printf("<%d> Process running [%d ~ %d]\n", job_queue.vp_arr[job_queue.front]->vprocess_id, cur_time, cur_time + job_queue.vp_arr[job_queue.front]->cpu_burst);
		cur_time += job_queue.vp_arr[job_queue.front]->cpu_burst;
		job_queue.front++;
	}
}

void scheduleSJF(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst);
	}
	printf("\n\n");

	printf("Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front]->arrival_t == cur_time)
		{
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (running == NULL)
		{
			if (ready_queue.front == ready_queue.back)	// if ready_queue is empty
			{
				if (idleflag == false)
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
				else
				{
					cur_time++;
					continue;
				}
			}
			else										// if ready_queue is nonempty
			{
				vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);

				if (idleflag)
				{
					idleflag = false;
					printf(" %d]\n", cur_time);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
				else
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		cur_time++;
	}
}

void schedulePESJF(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst);
	}
	printf("\n\n");

	printf("Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front % size]->arrival_t == cur_time)
		{
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (running == NULL)
		{
			if (ready_queue.front == ready_queue.back)	// if ready_queue is empty
			{
				if (idleflag == false)
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
				else
				{
					cur_time++;
					continue;
				}
			}
			else										// if ready_queue is nonempty
			{
				vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);

				if (idleflag)
				{
					idleflag = false;
					printf(" %d]\n", cur_time);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
				else
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
			}

			else
			{
				vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
				if (ready_queue.front != ready_queue.back && running->cpu_burst > ready_queue.vp_arr[ready_queue.front % size]->cpu_burst)
				{
//					printf("\nPreemptive occured! Current time:%d    running->cpu_burst:%d    ready_queue First:%d\n", cur_time, running->cpu_burst, ready_queue.vp_arr[ready_queue.front % size]->cpu_burst);
					printf(" %d]\n", cur_time);
					vpQueuePush(&ready_queue, running, size);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		cur_time++;
	}
}

void schedulePriority(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    Priority:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->p_priority);
	}
	printf("\n\n");

	printf("Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front]->arrival_t == cur_time)
		{
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (running == NULL)
		{
			if (ready_queue.front == ready_queue.back)	// if ready_queue is empty
			{
				if (idleflag == false)
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
				else
				{
					cur_time++;
					continue;
				}
			}
			else										// if ready_queue is nonempty
			{
				vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);

				if (idleflag)
				{
					idleflag = false;
					printf(" %d]\n", cur_time);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
					running = vpQueuePop(&ready_queue, size);
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				}
				else
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		cur_time++;
	}
}

void vpQSort(vprocess_ptr* vp_arr, int left, int right, int size, int mode)
{
	int border = left;

	switch (mode)
	{
		case VP_ARRIVAL:
			for (int i = left; i < right; i++)
			{
				if (vp_arr[i % size]->arrival_t <= vp_arr[right % size]->arrival_t)
				{
					if (border == i)
					{
						border++;
						continue;
					}
					else
					{
						vpSwap(vp_arr, border % size, i % size);
						border++;
					}
				}
			}
			break;

		case VP_CPU_BURST:
			for (int i = left; i < right; i++)
			{
				if (vp_arr[i % size]->cpu_burst <= vp_arr[right % size]->cpu_burst)
				{
					if (border == i)
					{
						border++;
						continue;
					}
					else
					{
						vpSwap(vp_arr, border % size, i % size);
						border++;
					}
				}
			}
			break;

		case VP_PRIORITY:
			for (int i = left; i < right; i++)
			{
				if (vp_arr[i % size]->p_priority <= vp_arr[right % size]->p_priority)
				{
					if (border == i)
					{
						border++;
						continue;
					}
					else
					{
						vpSwap(vp_arr, border % size, i % size);
						border++;
					}
				}
			}
			break;
	}

	vpSwap(vp_arr, border % size, right % size);

	if (left < border - 1)
	{
		vpQSort(vp_arr, left, border - 1, size, mode);
	}
	if (right > border + 1)
	{
		vpQSort(vp_arr, border + 1, right, size, mode);
	}
}

void vpSwap(vprocess_ptr* vp_arr, int i, int j)
{
	vprocess_ptr temp;
	temp = vp_arr[i];
	vp_arr[i] = vp_arr[j];
	vp_arr[j] = temp;
}

void vpQueuePush(vpqueue* vp_queue, vprocess_ptr vp, int size)
{
	vp_queue->vp_arr[vp_queue->back % size] = vp;
	vp_queue->back++;
}

vprocess_ptr vpQueuePop(vpqueue* vp_queue, int size)
{
	vp_queue->front++;
	return vp_queue->vp_arr[(vp_queue->front - 1) % size];
}