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
#include "Worker_test.h"
#include "Task_test.h"

/* init */
Worker_test::Worker_test() 
{
    workingTask = new Task_test;
}

/* destruct */
Worker_test::~Worker_test() 
{
    delete workingTask;
}

/* Do benchmark and return result (usually the time to task t), t is supposed
 * to be a benchmark task.  In this app, it just send back a PI. */
double
Worker_test::benchmark( MWTask *t ) 
{
        Task_test *tl = dynamic_cast<Task_test *> ( t );
        tl->printself(30);
        return 3.14159;
}

/* unpack the init data from the driver */
MWReturn Worker_test::unpack_init_data( void ) 
{
	/* As we don'e have init data, we do nothing */
	return OK;
}

/* Execute each task */
void Worker_test::execute_task( MWTask *t ) 
{
	int i;
	
	MWprintf(30, "Enter Worker_test::execute_task\n");
#ifdef NO_DYN_CAST
	Task_test *tl = (Task_test *) t;
#else
    	Task_test *tl = dynamic_cast<Task_test *> ( t );
#endif
	
	MWprintf(30, "The task I am working on is: \n\t");
	for (i=0; i<tl->size; i++)
		MWprintf(30, "%d ", tl->numbers[i]);
	MWprintf(30, "\n");
	
	/* the real work :-) */
	tl->largest = tl->numbers[0];
    	for (i=1; i<tl->size; i++) 
		if (tl->largest < tl->numbers[i]) 
			tl->largest = tl->numbers[i];

	MWprintf(30, "Leave Worker_test::execute_task, largest = %d\n", tl->largest);
}

MWTask* Worker_test::gimme_a_task()
{
	return new Task_test;
}

/* Just return a newly created application worker object */
MWWorker*
gimme_a_worker ()
{
       	return new Worker_test;
}