/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/
#include "Task_MYAPP.h"
#include "MW.h"

/* init */
Task_MYAPP::Task_MYAPP() 
{
	size = 0;
	numbers = NULL;
}

/* init too */
Task_MYAPP::Task_MYAPP(int size, int *numbers) 
{
	if (size > 0) {
		this->size = size;
		this->numbers = new int[size];
		for (int i=0; i<size; i++)
			this->numbers[i] = numbers[i];
		printself(30); 
	} else MWprintf(30, "Task construction: array size <= 0! \n");
}

/* destruction */
Task_MYAPP::~Task_MYAPP() {
	if (numbers != NULL)
		delete [] numbers;
}

/* print the task to stdout */
void
Task_MYAPP::printself( int level ) 
{
	MWprintf ( level, "size=%d, numbers=\n\t", size);
	for (int i=0; i<size; i++)
		MWprintf(level, "%d ", numbers[i]);
	MWprintf (level, "\n");
}

/* The driver packs the input data via RMC, the data which will be sent to a worker. */
void Task_MYAPP::pack_work( void ) 
{
	RMC->pack(&size, 1, 1);
	RMC->pack(numbers, size, 1);
}

/* The worker unpacks input data via RMC, need to allocate space for data */
void Task_MYAPP::unpack_work( void ) 
{
	RMC->unpack(&size, 1, 1);
	if (numbers != NULL)
		delete [] numbers;
	numbers = new int[size];
	RMC->unpack(numbers, size, 1);
}

/* The worker packs result data via RMC, the result will be sent back to driver */
void Task_MYAPP::pack_results( void ) 
{
	RMC->pack(&largest, 1, 1);
}

/* The driver unpacks result data via RMC */
void Task_MYAPP::unpack_results( void ) 
{
	RMC->unpack(&largest, 1, 1);
}

/* write checkpoint info per task, for each task haven't been finished */
void Task_MYAPP::write_ckpt_info( FILE *fp ) 
{
	/* Nothing in this app, will lose data if it crashes. */
}

/* Read checkpoint info, in the order written into the file */
void Task_MYAPP::read_ckpt_info( FILE *fp ) 
{
	/* Nothing to be read since nothing is written */
}
