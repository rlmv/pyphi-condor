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
#ifndef DRIVER_NQUEENS_H
#define DRIVER_NQUEENS_H

#include "MWDriver.h"
#include "Task-nQueens.h"

/** The driver class derived from the MWDriver class for this application.

    In particular, this application is a very simple one that solves 
    the nQueens problem. It does this by dividing the search space into
    subspaces and distributing to each worker one subspace at a time to search.
    The workers will work on that subspace and if a solution is found will
    return the solution. Once the solution is found the master will stop 
    all the workers and exit.

    Also this example is void of any comments since it is a very simple example.

    @author Sanjeev Kulkarni
*/

class Driver_nQueens : public MWDriver {

 public:

    Driver_nQueens();

    ~Driver_nQueens();

    MWReturn get_userinfo( int argc, char *argv[] );

    MWReturn setup_initial_tasks( int *, MWTask *** );

    MWReturn act_on_completed_task( MWTask * );

    MWReturn pack_worker_init_data( void );

    void printresults();

    void write_master_state( FILE *fp );

    void read_master_state( FILE *fp );

    MWTask* gimme_a_task();

 private:

    int N;
    int *solution;
    int num_tasks;
    const static int partition_factor = 5;

};

#endif
