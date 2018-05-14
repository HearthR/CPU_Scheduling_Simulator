#include "scheduler.h"

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

void schedulePEPriority(vprocess_ptr vp, int size)
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

			else
			{
				vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
				if (ready_queue.front != ready_queue.back && running->p_priority > ready_queue.vp_arr[ready_queue.front % size]->p_priority)
				{
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