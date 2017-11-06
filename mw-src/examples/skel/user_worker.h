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
#ifndef USER_WORKER_H
#define USER_WORKER_H

#include "MWWorker.h"
#include "user_task.h"

/** 

	This is your user-defined worker, that knows how do your
	user-defined tasks...

*/

class user_worker : public MWWorker {

 public:
    /// Default Constructor
    user_worker();

    ~user_worker();

    /**@name Implemented methods.
       
       These are the ones that must be Implemented in order
       to create an application
    */
    //@{
    /// Unpacks the "base" application data from the PVM buffer
    void unpack_init_data( void );
    
    /** Executes the given task.
	 */
    void execute_task( MWTask * );
    //@}

	// we need this so that the generic worker class can get
	// an instance of the derived task
	MWTask* gimme_a_task();
    
 private:


		/* Private stuff for your app... */

};

#endif
