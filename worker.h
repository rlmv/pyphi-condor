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
#ifndef _WORKER_H
#define _WORKER_H

#include "MWWorker.h"
#include "task.h"
#include <Python.h>

class Worker : public MWWorker
{
 public:
    Worker();
    ~Worker();

    /* Benchmarking */
    double benchmark( MWTask *t );

    /* unpack init data */
    MWReturn unpack_init_data( void );

    /* do the real work for each task */
    void execute_task( MWTask * );

    MWTask* gimme_a_task();

    char * pickle;
    int pickle_size;
    PyObject * python_worker;
};
#endif
