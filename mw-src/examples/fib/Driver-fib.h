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
#ifndef DRIVER_FIB_H
#define DRIVER_FIB_H

#include "MWDriver.h"
#include "Task-fib.h"

/** The driver class derived from the MWDriver class for this application.

    In particular, this application is a very simple one that calculates 
    the fibonacci sequence for different pairs of starting numbers.

    This simple app will not need any special math packages to run, and
    is designed with the non-math-specialist in mind (like me!).

    @author Mike Yoder
*/

class Driver_Fib : public MWDriver {

 public:
    /// Constructor
    Driver_Fib();
    /// Destructor
    ~Driver_Fib();

    /**@name Implemented Methods

       These methods are the ones that *must* be implemented in order
       to create an application
    */

    //@{
    /// Get the info from the user.  Don't forget to get the 
    /// worker_executable!
    MWReturn get_userinfo( int argc, char *argv[] );

    /// Set up an array of tasks here
    MWReturn setup_initial_tasks( int *, MWTask *** );

    /// What to do when a task finishes:
    MWReturn act_on_completed_task( MWTask * );

    /// Put things in the send buffer here that go to a worker
    MWReturn pack_worker_init_data( void );

    /// OK, this one doesn't *have* to be...but you want to be able to
    /// tell the world the results, don't you? :-)
    void printresults();
    //@}

        /** @name Checkpointing Methods */
    //@{
    
        /** Write out the state of the master to an fp. */
    void write_master_state( FILE *fp );

        /** Read the state from an fp.  This is the reverse of 
            write_master_state(). */
    void read_master_state( FILE *fp );

        /** That simple annoying function */
    MWTask* gimme_a_task();

    //@}

 private:

    int max_seed_num;
    int num_tasks;
    int seqlen;
	int target; // num of workers.

    Task_Fib **completedTasks;
};

#endif
