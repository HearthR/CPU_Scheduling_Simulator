#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "scheduler.h"

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
//	schedulePESJF(vp, psize);
	schedulePEPriority(vp, psize);

	system("pause");
	return 0;
}