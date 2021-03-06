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
/* Worker-Fib.C

   The .C file for the Worker_Fib class 

*/

#include "Worker-fib.h"
#include "Task-fib.h"

Worker_Fib::Worker_Fib() {
    workingTask = new Task_Fib;
}

Worker_Fib::~Worker_Fib() {
    delete workingTask;
}

MWReturn Worker_Fib::unpack_init_data( void ) {
    
    // This unpacks the stuff packed in Driver_Fib::pack_worker_init_data().
    // Normally, you would do something here like unpack a big matrix
    // or get *some* type of starting data...
    char foo[128];
    RMC->unpack( foo );
    MWprintf ( 40, "The master says: \"%s\"\n", foo );
    return OK;
}

double
Worker_Fib::benchmark( MWTask *t ) {
	Task_Fib *tf = dynamic_cast<Task_Fib *> ( t );
	MWprintf ( 30, "Benchmark task:\n" );
	tf->printself(30);
	return 3.14159;
}

void Worker_Fib::execute_task( MWTask *t ) {

    // We have to downcast this task passed to us into a Task_Fib:
#ifdef NO_DYN_CAST
	Task_Fib *tf = (Task_Fib *) t;
#else
    Task_Fib *tf = dynamic_cast<Task_Fib *> ( t );
#endif    

    tf->results = new int[tf->sequencelength];
    get_fib( tf->first, tf->second, tf->results, tf->sequencelength );
}


/* Given two seed numbers (firstseed and secondseed), fill the array
   pointed at by sequence to contain the first SEQUENCELENGTH numbers
   in the related fibonacci series.  Pretty simple, really.
*/
void Worker_Fib::get_fib ( int firstseed, int secondseed, 
                           int * sequence, int len ) {

	int i;

	sequence[0] = firstseed;
	sequence[1] = secondseed;
	
	for ( i=2 ; i < len ; i++ ) {
			// Each element in the array is the sum of the previous two.
		sequence[i] = sequence[i-1] + sequence[i-2];
	}

        // pretend like this is a really difficult thing to do:

    for ( int j=0 ; j < 10000 ; j++ ) {
        for ( int k=0 ; k != 10000 ; k++ )
            ;
    }


}

MWTask* Worker_Fib::gimme_a_task ()
{
	return new Task_Fib;
}

MWWorker*
gimme_a_worker ()
{
    return new Worker_Fib;
}
