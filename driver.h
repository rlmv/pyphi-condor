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
#ifndef _DRIVER_H
#define _DRIVER_H

#include <Python.h>
#include "MWDriver.h"
#include "task.h"
#include <string>

/** Application Driver subclass derived from MWDriver */

class Driver : public MWDriver
{
 public:
    Driver(char * pickle, int pickle_size, PyListObject* cuts);
    ~Driver();

    /* Get the info from the user.  Don't forget to get the worker_executable! */
    MWReturn get_userinfo( int argc, char *argv[] );

    /* Set up an array of tasks here */
    MWReturn setup_initial_tasks( int *, MWTask *** );

    /* What to do when a task finishes */
    MWReturn act_on_completed_task( MWTask * );

    /* Put things in the send buffer here that go to a worker */
    MWReturn pack_worker_init_data( void );

    /* OK, this one doesn't *have* to be...but you want to be able to
     * tell the world the results, don't you? :-) */
    void printresults();

    /* Write out the state of the master to a file. */
    void write_master_state( FILE *fp );

    /* Read in the state from a file. */
    void read_master_state( FILE *fp );

    /* Just return a newly constructed application task */
    MWTask* gimme_a_task();

 private:  /* You application specific information goes here! */


    char * pickle;
    int pickle_size;

    PyListObject * cuts;
    int num_tasks;

    PyObject* results;
};

#endif
