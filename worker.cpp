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
#include "Worker.h"
#include "Task.h"
#include <Python.h>
#include "caller.h"
#include <stdexcept>


/* init */
Worker::Worker()
{
    workingTask = new Task;
    pickle = NULL;
    pickle_size = 0;
    python_worker = NULL;
}

/* destruct */
Worker::~Worker()
{
    delete workingTask;
    delete pickle;
}

/* Do benchmark and return result (usually the time to task t), t is supposed
 * to be a benchmark task.  In this app, it just send back a PI. */
double
Worker::benchmark( MWTask *t )
{
    Task *tl = dynamic_cast<Task *> ( t );
    tl->printself(30);
    return 3.14159;
}

/* unpack the init data from the driver */
MWReturn Worker::unpack_init_data( void )
{
    RMC->unpack(&pickle_size, 1, 1);

    pickle = new char[pickle_size];

    RMC->unpack(pickle, pickle_size, 1);

    python_worker = unpack_pickle(pickle, pickle_size);
    // TODO handle this.
    // It seems that MWorker.C doesn't actually check the return value of
    // unpack_init_data?
    if (python_worker == NULL) {
        return ABORT;
        //throw std::invalid_argument("Failed to unpickle");
    }

    return OK;
}

/* Execute each task */
void Worker::execute_task( MWTask *t )
{

    MWprintf(30, "Enter Worker::execute_task\n");
#ifdef NO_DYN_CAST
    Task *tl = (Task *) t;
#else
    Task *tl = dynamic_cast<Task *> ( t );
#endif

    tl->result = use_pickle(python_worker, tl->input);
    // CHeck exception handling
    // if (tl->result == NULL) {   //
    //     throw std::invalid_argument("Failed to run worker");
    // }

    // MWprintf(30, "The task I am working on is: \n\t");
    // for (i=0; i<tl->size; i++)
    //     MWprintf(30, "%d ", tl->numbers[i]);
    // MWprintf(30, "\n");

    // /* the real work :-) */
    // tl->largest = tl->numbers[0];
    // for (i=1; i<tl->size; i++)
    //     if (tl->largest < tl->numbers[i])
    //         tl->largest = tl->numbers[i];

    // MWprintf(30, "Leave Worker::execute_task, largest = %d\n", tl->largest);
}

MWTask* Worker::gimme_a_task()
{
    return new Task;
}

/* Just return a newly created application worker object */
MWWorker* gimme_a_worker ()
{
    return new Worker;
}
