#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "vprocess.h"


void scheduleFCFS(vprocess_ptr vp, int size);
void scheduleSJF(vprocess_ptr vp, int size);
void schedulePESJF(vprocess_ptr vp, int size);
void schedulePriority(vprocess_ptr vp, int size);
void schedulePEPriority(vprocess_ptr vp, int size);
void scheduleRR(vprocess_ptr vp, int size, int t_quantum);
void schedulerEval(vprocess_ptr vp, int size);