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
#ifndef WORKER_FIB_H
#define WORKER_FIB_H

#include "MWWorker.h"
#include "Task-fib.h"

/** The Worker class created for my fibonacci example application.

    ...more to come...

*/

class Worker_Fib : public MWWorker {

 public:
    /// Default Constructor
    Worker_Fib();

    /// Default Destructor
    ~Worker_Fib();

    /**@name Implemented methods.
       
       These are the ones that must be Implemented in order
       to create an application
    */
    //@{
    /// Unpacks the "base" application data from the PVM buffer
    MWReturn unpack_init_data( void );
    
    /** Executes the given task.
        Remember to cast the task as type {\ttDummyTask}
    */
    void execute_task( MWTask * );
    //@}

	// Need this so that generic worker class can get a derived task instance
	MWTask* gimme_a_task();

		/** Benchmarking. */
	double benchmark( MWTask *t );

 private:

    /**  Given two seed numbers (firstseed and secondseed), fill the array
         pointed at by sequence to contain the first SEQUENCELENGTH numbers
         in the related fibonacci series.  Pretty simple, really.
         @param firstseed The first of the pair to make the sequence
         @param secondseed The second of the pair to begin the sequence
         @param sequence A pointer to an int array that will hold the 
                         completed fibonacci sequence.
         @param len The length of sequence.
    */
    void get_fib ( int firstseed, int secondseed, int *sequence, int len );

};

#endif
