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
#include "Task.h"
#include "MW.h"
#include <string>

/* init */
Task::Task()
{
    size = 0;
    numbers = NULL;
    s = std::string();
}

/* init too */
Task::Task(int size, int *numbers, std::string s)
{
    if (size > 0) {
        this->s = s;
        this->size = size;
        this->numbers = new int[size];
        for (int i=0; i<size; i++)
            this->numbers[i] = numbers[i];
        printself(30);
    } else MWprintf(30, "Task construction: array size <= 0! \n");
}

/* destruction */
Task::~Task() {
    if (numbers != NULL)
        delete [] numbers;
}

/* print the task to stdout */
void
Task::printself( int level )
{
    MWprintf ( level, "size=%d, string=%s, numbers=\n\t", size, s.c_str());
    for (int i=0; i<size; i++)
        MWprintf(level, "%d ", numbers[i]);
    MWprintf (level, "\n");
}

/* The driver packs the input data via RMC, the data which will be sent to a worker. */
void Task::pack_work( void )
{
    RMC->pack(&size, 1, 1);
    RMC->pack(numbers, size, 1);
}

/* The worker unpacks input data via RMC, need to allocate space for data */
void Task::unpack_work( void )
{
    RMC->unpack(&size, 1, 1);
    if (numbers != NULL)
        delete [] numbers;
    numbers = new int[size];
    RMC->unpack(numbers, size, 1);
}

/* The worker packs result data via RMC, the result will be sent back to driver */
void Task::pack_results( void )
{
    RMC->pack(&largest, 1, 1);
}

/* The driver unpacks result data via RMC */
void Task::unpack_results( void )
{
    RMC->unpack(&largest, 1, 1);
}

/* write checkpoint info per task, for each task haven't been finished */
void Task::write_ckpt_info( FILE *fp )
{
    /* Nothing in this app, will lose data if it crashes. */
}

/* Read checkpoint info, in the order written into the file */
void Task::read_ckpt_info( FILE *fp )
{
    /* Nothing to be read since nothing is written */
}
