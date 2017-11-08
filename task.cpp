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
#include "caller.h"

/* init */
Task::Task()
{
    input = NULL;
    result = NULL;
}

/* init too */
Task::Task(PyObject *input)
{
    this->input = input;
    this->result = NULL;
}

// TODO
/* destruction */
Task::~Task() {
    // if (result != NULL)
    //     delete [] result;
}

/* print the task to stdout */
void
Task::printself( int level )
{
    // MWprintf ( level, "size=%d, job=%d\n\t", size, job);
    // for (int i=0; i<size; i++)
    //     MWprintf(level, "%d ", numbers[i]);
    // MWprintf (level, "\n");
}

void Task::pack_PyObject(PyObject* obj)
{
    int size;
    char *data;
    pack_pickle(obj, &data, &size);
    RMC->pack(&size, 1, 1);
    RMC->pack(data, size, 1);
}


// TODO: what sort of error catching needs to happen?
/* Stream and unpickle a Python object from RMC */
PyObject* Task::unpack_PyObject()
{
    int size;
    char *data;
    RMC->unpack(&size, 1, 1);

    data = new char[size];
    RMC->unpack(data, size, 1);

    return unpack_pickle(data, size);
}

/* The driver packs the input data via RMC, the data which will be sent to a worker. */
void Task::pack_work( void )
{
    pack_PyObject(input);
}

/* The worker unpacks input data via RMC, need to allocate space for data */
void Task::unpack_work( void )
{
    // if (input != NULL)
    //     delete [] input;

    input = unpack_PyObject();
    // TODO catch error??
}

/* The worker packs result data via RMC, the result will be sent back to driver */
void Task::pack_results( void )
{
    pack_PyObject(result);
    // RMC->pack(&largest, 1, 1);
    // RMC->pack(&result, 1, 1);
}

/* The driver unpacks result data via RMC */
void Task::unpack_results( void )
{
    result = unpack_PyObject();
    // RMC->unpack(&largest, 1, 1);
    // RMC->unpack(&result, 1, 1);
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
