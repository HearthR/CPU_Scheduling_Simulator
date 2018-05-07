#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
	int vprocess_id;
	int cpu_burst;
	int io_burst;
	int arrival_t;
	int p_priority;
	int cpu_remaining;
	int io_remaining;
} vprocess;

vprocess* createProcess(int num);
bool isPidDuplicate(vprocess* vp, int current);

int main()
{


	return 0;
}

vprocess* createProcess(int num)
{
	vprocess *vp = (vprocess*)malloc(sizeof(vprocess) * num);
	memset(vp, 0, sizeof(vp));

	srand(time(NULL));

	for (int i = 0; i < num; i++)
	{
		while (isPidDuplicate(vp, i))
		{
			vp[i].vprocess_id = rand() % 100 + 1;
		}
		vp[i].arrival_t = rand() % 30;
		vp[i].cpu_burst = rand() % 30 + 1;
		vp[i].cpu_remaining = vp[i].cpu_burst;
		vp[i].io_burst = rand() % 30 + 1;
		vp[i].io_remaining = vp[i].io_burst;
		vp[i].p_priority = rand() % 20 + 1;
	}

	return vp;
}

bool isPidDuplicate(vprocess* vp, int current)
{
	if (vp[0].vprocess_id == 0)
	{
		return true;
	}

	for (int i = 0; i < current; i++)
	{
		if (vp[current].vprocess_id == vp[i].vprocess_id)
		{
			return true;
		}
	}
	return false;
}