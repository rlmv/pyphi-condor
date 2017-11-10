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
/* These methods will be implemented to reflect the application behavior */

#include <Python.h>
#include "MW.h"
#include "driver.h"
#include "worker.h"
#include "task.h"
#include <unistd.h>
#include <string>
#include "caller.h"

/* initialization */
Driver::Driver(char * p, int p_size, PyListObject* cuts)
{
    /* For statically generated tasks, you can decide how many tasks
     * you want to use based on the input information. However, you can
     * also dynamically generate tasks (either when init, or when
     * acr_on_completed_task, and add them to TODO queue by pushTask. */

    /* The list of tasks you will generate */
    pickle = p;
    pickle_size = p_size;

    this->cuts = cuts;
    this->num_tasks = PyList_Size((PyObject *) cuts);
    this->results = PyList_New(0);
}

/* destruction */
Driver::~Driver()
{
    /* release the memory allocated for tasks */

    // TODO: release cuts
}

/* Here the application can (1) get per-run information from stdin (the input
 * file redirected by Condor), or (2) get configuration info from user-defined
 * config file; or (3) just hard-code your config here. The app should tell MW
 * (or RMC the Resource Manager and Communicator, which is a class member)
 * some basic info by calling the functions below:
 * (a) RMC->set_num_exec_classes()      num of different processes you have
 * (b) RMC->set_num_arch_classes() 	num of different platforms you have
 * (c) RMC->set_arch_class_attributes()  	for each platform
 * (d) RMC->add_executable() 	for combination of exec_class, arch_class;
 * (e) set_checkpoint_freqency(), ... and other information.  */

MWReturn Driver::get_userinfo( int argc, char *argv[] )
{
    // placeholder. Replace with compiled worker executable.
    // Could even be a Python executable?
    char exec[] = "run.py";

    MWprintf(30, "Enter Driver::get_userinfo\n");

    RMC->add_executable(exec, "((Arch==\"INTEL\") && (Opsys==\"LINUX\"))");

    /* checkpoint requirement */
    set_checkpoint_frequency (10);

    RMC->set_target_num_workers(2);

    MWprintf(30, "Leave Driver::get_userinfo\n");
    return OK;
}

/* setup (generate and push) the first batch of tasks in the beginning */
MWReturn Driver::setup_initial_tasks(int *n_init , MWTask ***init_tasks)
{
    int i;
    PyObject* cut;

    MWprintf(30, "Num tasks: %d\n", num_tasks);

    *n_init = num_tasks;
    *init_tasks = new MWTask *[num_tasks];

    for ( i=0; i < num_tasks; i++) {
        cut = PyList_GetItem((PyObject *) cuts, i);
        (*init_tasks)[i] = new Task(cut);
    }

    return OK;
}

/* Implement application behavior to process a just completed task */
MWReturn Driver::act_on_completed_task( MWTask *t )
{
#ifdef NO_DYN_CAST
    Task *tf = (Task*) t;
#else
    Task *tf = dynamic_cast<Task *> (t);
#endif

    print_result(tf->result);
    if (PyList_Append(results, tf->result) != 0) {
        return ABORT;
    }

    return OK;
}

/* The first batch of data for a newly spawned worker, e.g. init data */
MWReturn Driver::pack_worker_init_data( void )
{
    /* Nothing for this application */
    MWprintf(10, "Packing init data\n");

    RMC->pack(&pickle_size, 1, 1);
    RMC->pack(pickle, pickle_size, 1);

    return OK;
}

/* Print out the result when MW is done. MW assume that the application
 * is keeping track of the results :-) */
void Driver::printresults()
{
    //    MWprintf ( 10, "The largest number is %d.\n", this->largest);
    print_result(results);
}

/* Write app-specific master checkpoint info */
void
Driver::write_master_state( FILE *fp )
{
    /* Nothing to be written */
}

/* Read app-specific master checkpoint info */
void
Driver::read_master_state( FILE *fp )
{
    /* Nothing to be read */
}

/* Return a new application task object */
MWTask*
Driver::gimme_a_task()
{
    return new Task;
}

// /* Return a new driver object */
// MWDriver* gimme_the_master()
// {
//     return new Driver;
// }
