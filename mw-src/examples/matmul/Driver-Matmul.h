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
#ifndef DRIVER_MATMUL_H
#define DRIVER_MATMUL_H

#include "MWDriver.h"
#include "Task-Matmul.h"

/** The driver class derived from the MWDriver class for this application.

    In particular, this application is a very simple one that calculates 
    the matrix multiplication of two matrices. It does this by sending the entire
    matrices A and B to the other side and then sending a set of row numbers at a time.
    The workers will work on that set of rows and return back the results.

    This simple app will not need any special math packages to run, and
    is designed with the non-math-specialist in mind (like me!).

    Also this example is void of any comments since it is a very simple example.

    @author Sanjeev Kulkarni
*/

class Driver_Matmul : public MWDriver {

 public:

    Driver_Matmul();

    ~Driver_Matmul();

    MWReturn get_userinfo( int argc, char *argv[] );

    MWReturn setup_initial_tasks( int *, MWTask *** );

    MWReturn act_on_completed_task( MWTask * );

    MWReturn pack_worker_init_data( void );

    void printresults();

    void write_master_state( FILE *fp );

    void read_master_state( FILE *fp );

    MWTask* gimme_a_task();

MWDriver* gimme_the_master();


 private:

    double **A;
    double **B;
    double **C;
    int num_rowsA;
    int num_rowsB;
    int num_colsB;
    int num_tasks;
    static const int partition_factor; 

};

#endif
