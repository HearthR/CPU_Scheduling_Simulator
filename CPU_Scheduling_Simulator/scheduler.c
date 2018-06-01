#include "scheduler.h"

void scheduleFCFS(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	srand(time(NULL));

	while (job_queue.back < size)
	{
		job_queue.vp_arr[job_queue.back] = &vp[job_queue.back];
		job_queue.back++;
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### FCFS Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining <= 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
					cur_time++;
					continue;
				}
			}
			else										// if ready_queue is nonempty
			{
				if (idleflag)
				{
					idleflag = false;
					printf(" %d]\n", cur_time);
				}

				running = vpQueuePop(&ready_queue, size);
				running->waiting_t += cur_time - running->waiting_start;
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
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
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt remaining:%d\n", cur_time, running->io_remaining);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					}
					else
					{
						idleflag = true;
						printf("<Idle> Process running [%d ~", cur_time);
					}
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void scheduleSJF(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### SJF Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining == 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
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
				}

				running = vpQueuePop(&ready_queue, size);
				running->waiting_t += cur_time - running->waiting_start;
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
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
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt\n", cur_time);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					}
					else
					{
						idleflag = true;
						printf("<Idle> Process running [%d ~", cur_time);
					}
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void schedulePESJF(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### Preemptive SJF Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front % size]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining == 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
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
				}

				running = vpQueuePop(&ready_queue, size);
				running->waiting_t += cur_time - running->waiting_start;
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
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
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt\n", cur_time);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_CPU_BURST);
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
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
						running->waiting_start = cur_time;
						vpQueuePush(&ready_queue, running, size);

						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					}
				}				
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void schedulePriority(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    Priority:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->p_priority, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### Priority Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining == 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
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
				}

				running = vpQueuePop(&ready_queue, size);
				running->waiting_t += cur_time - running->waiting_start;
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
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
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt\n", cur_time);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					}
					else
					{
						idleflag = true;
						printf("<Idle> Process running [%d ~", cur_time);
					}
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void schedulePEPriority(vprocess_ptr vp, int size)
{
	int cur_time = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    Priority:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->p_priority, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### Preemptive Priority Gantt Chart:\n");
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front % size]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining == 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
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
				}

				running = vpQueuePop(&ready_queue, size);
				running->waiting_t += cur_time - running->waiting_start;
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
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
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt\n", cur_time);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						vpQSort(ready_queue.vp_arr, ready_queue.front, ready_queue.back - 1, size, VP_PRIORITY);
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
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
						running->waiting_start = cur_time;
						printf(" %d]\n", cur_time);
						vpQueuePush(&ready_queue, running, size);

						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					}
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void scheduleRR(vprocess_ptr vp, int size, int t_quantum)
{
	int cur_time = 0;
	int used_quantum = 0;
	vpqueue job_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue ready_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vpqueue waiting_queue = { (vprocess_ptr*)calloc(sizeof(vprocess_ptr), size), 0, 0 };
	vprocess_ptr running = NULL;
	bool idleflag = false;

	for (int i = 0; i < size; i++)
	{
		vpQueuePush(&job_queue, &vp[i], size);
	}
	vpQSort(job_queue.vp_arr, 0, size - 1, size, VP_ARRIVAL);

	printf("\n\n#### Process list\n");
	for (int i = 0; i < size; i++)
	{
		printf("Num:%3d    PID:%3d    Arrival time:%3d    CPU burst:%3d    I/O burst:%3d\n", i, job_queue.vp_arr[i]->vprocess_id, job_queue.vp_arr[i]->arrival_t, job_queue.vp_arr[i]->cpu_burst, job_queue.vp_arr[i]->io_burst);
	}
	printf("\n\n");

	printf("#### RR Gantt Chart (time quantum %d):\n", t_quantum);
	while (job_queue.front != job_queue.back || ready_queue.front != ready_queue.back || waiting_queue.front != waiting_queue.back || running != NULL)
	{
		while (job_queue.front != job_queue.back && job_queue.vp_arr[job_queue.front % size]->arrival_t == cur_time)
		{
			job_queue.vp_arr[job_queue.front]->waiting_start = cur_time;
			vpQueuePush(&ready_queue, vpQueuePop(&job_queue, size), size);
		}

		if (waiting_queue.front != waiting_queue.back)
		{
			vpQSort(waiting_queue.vp_arr, waiting_queue.front, waiting_queue.back - 1, size, VP_IO);
			for (int i = waiting_queue.front; i < waiting_queue.back; i++)
			{
				if (waiting_queue.vp_arr[i % size]->io_remaining == 0)
				{
					vprocess_ptr temp = vpQueuePop(&waiting_queue, size);
					temp->waiting_start = cur_time;
					vpQueuePush(&ready_queue, temp, size);
				}
			}
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
					if (waiting_queue.front != waiting_queue.back)
					{
						vpIOWorking(&waiting_queue, size);
					}
					cur_time++;
					continue;
				}
			}
			else										// if ready_queue is nonempty
			{
				if (idleflag)
				{
					idleflag = false;
					printf(" %d]\n", cur_time);
				}

				running = vpQueuePop(&ready_queue, size);
				printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
				used_quantum = 0;
			}
		}
		else
		{
			if (running->cpu_remaining == 0)
			{
				printf(" %d]\n", cur_time);
				running->completed_t = cur_time;
				running = NULL;

				if (ready_queue.front == ready_queue.back && job_queue.front == job_queue.back && waiting_queue.front == waiting_queue.back)
				{
					break;
				}

				if (ready_queue.front != ready_queue.back)
				{
					running = vpQueuePop(&ready_queue, size);
					running->waiting_t += cur_time - running->waiting_start;
					printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
					used_quantum = 0;
				}
				else
				{
					idleflag = true;
					printf("<Idle> Process running [%d ~", cur_time);
				}
			}

			else
			{
				if (running->io_remaining > 0 && rand() % 4 == 0)
				{
					printf(" %d]\t\t I/O interrupt\n", cur_time);
					vpQueuePush(&waiting_queue, running, size);
					running = NULL;

					if (ready_queue.front != ready_queue.back)
					{
						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
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
					if (used_quantum == t_quantum)
					{
						running->waiting_start = cur_time;
						printf(" %d]\n", cur_time);
						vpQueuePush(&ready_queue, running, size);

						running = vpQueuePop(&ready_queue, size);
						running->waiting_t += cur_time - running->waiting_start;
						printf("<%d> Process running [%d ~", running->vprocess_id, cur_time);
						used_quantum = 0;
					}
				}
			}
		}

		if (running != NULL)
		{
			running->cpu_remaining--;
			used_quantum++;
		}
		if (waiting_queue.front != waiting_queue.back)
		{
			vpIOWorking(&waiting_queue, size);
		}
		cur_time++;
	}

	schedulerEval(vp, size);
}

void schedulerEval(vprocess_ptr vp, int size)
{
	float avg_waiting = 0;
	float avg_turnaround = 0;
	printf("\n\n#### Scheduler Evaluation:\n");
	for (int i = 0; i < size; i++)
	{
		printf("PID:%3d    Waiting time:%3d    Turnaround time:%3d\n", vp[i].vprocess_id, vp[i].waiting_t, vp[i].completed_t - vp[i].arrival_t);
		avg_waiting += vp[i].waiting_t;
		avg_turnaround += vp[i].completed_t - vp[i].arrival_t;
	}
	printf("\n");
	printf("Average waiting time:%.4f    Average turnaround time:%.4f\n\n\n\n", avg_waiting / size, avg_turnaround / size);
}