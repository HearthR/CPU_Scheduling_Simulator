#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "scheduler.h"

void selectScheduler(vprocess_ptr vp, int size);

int main()
{
	int psize = 0;
	vprocess_ptr vp;
	printf("Number of process : ");
	scanf_s("%d", &psize);
	getchar();
	vp = createVProcess(psize);
	selectScheduler(vp, psize);

	system("pause");
	return 0;
}

void selectScheduler(vprocess_ptr vp, int size)
{
	int scheduler_select = 0;
	int t_quantum = 0;

	while (1)
	{
		switch (scheduler_select)
		{
			case 1:
				scheduleFCFS(vp, size);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 2:
				scheduleSJF(vp, size);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 3:
				schedulePriority(vp, size);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 4:
				schedulePESJF(vp, size);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 5:
				schedulePEPriority(vp, size);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 6:
				printf("Please enter time quantum: ");
				scanf_s("%d", &t_quantum);
				getchar();
				scheduleRR(vp, size, t_quantum);
				resetVProcess(vp, size);
				scheduler_select = 0;
				break;
			case 7:
				printf("Number of process : ");
				scanf_s("%d", &size);
				getchar();
				vp = createVProcess(size);
				scheduler_select = 0;
				break;
			default:
				printf("\n\n");
				printf("#### 1: FCFS\n");
				printf("#### 2: Non-preemptive SJF\n");
				printf("#### 3: Non-preemptive Priority\n");
				printf("#### 4: Preemptive SJF\n");
				printf("#### 5: Preemptive Priority\n");
				printf("#### 6: RR\n");
				printf("#### 7: Create new process list\n\n");
				printf("Please select scheduler number: ");
				scanf_s("%d", &scheduler_select);
				getchar();
		}
	}
}