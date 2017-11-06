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
/* 

   The .C file for the user_worker class 

*/

#include "user_worker.h"
#include "user_task.h"
#include "pvm3.h"

user_worker::user_worker() {
    // MWTaskContainer is the generic "task container" class that holds
    // instances of user_task via the gimme_a_task function. We do this
    // here so that users may optionally use their own derived task container
    // class. Note: Previous versions of MW had a workingTask, which is no
    // longer used here
    workingTask = new MWTaskcontainer;

/* The idea is to keep one task in the worker, and that
   task get reused over and over.  It doesn't have to be
   this way, but it's a wee bit faster... */
}

user_worker::~user_worker() {
    delete workingTask;
}

void user_worker::unpack_init_data( void ) {
    
/* This unpacks the stuff packed in user_driver::pack_worker_init_data().
   Normally, you would do something here like unpack a big matrix
   or get *some* type of starting data... See the MWBaseComm class for
   more information on communication.   You'll want to:

   MWComm->unpack( args... )
*/
}

void user_worker::execute_task( MWTask *t ) {

    // We have to downcast this task passed to us into a user_task:
    user_task *tu = dynamic_cast<user_task *> ( t );
    
/* Now that we've got the task, take it and solve the problem! 
   The solution goes back into the task class. */
}

MWTask* user_worker::gimme_a_task()
{
	return new user_worker;
}	
