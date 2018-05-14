#include "vprocess.h"

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