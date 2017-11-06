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
/* Worker-Matmul.C */

#include "Worker-Matmul.h"
#include "Task-Matmul.h"
//#include "TaskContainer-Matmul.h"
Worker_Matmul::Worker_Matmul() 
{
	workingTask = new Task_Matmul;
	A = B = NULL;
	num_rowsA = num_rowsB = num_colsB = 0;
}

Worker_Matmul::~Worker_Matmul() 
{
	delete workingTask;
	if ( A ) {
		for (int i = 0; i < num_rowsA; i++ )
			delete [] A[i];
		delete []A;
	}
	if ( B ) {
		for (int i = 0; i < num_rowsB; i++ )
			delete [] B[i];
		delete []B;
	}
}

MWReturn Worker_Matmul::unpack_init_data( void ) 
{
	int i;

	RMC->unpack ( &num_rowsA, 1 );
	RMC->unpack ( &num_rowsB, 1 );
	RMC->unpack ( &num_colsB, 1 );

	A = new double*[num_rowsA];
	for ( i = 0; i < num_rowsA; i++ )
		A[i] = new double[num_rowsB];

	B = new double*[num_rowsB];
	for ( i = 0; i < num_rowsB; i++ )
		B[i] = new double[num_colsB];

	for ( i = 0; i < num_rowsA; i++ )
		RMC->unpack ( A[i], num_rowsB );

	for ( i = 0; i < num_rowsB; i++ )
		RMC->unpack ( B[i], num_colsB );


	return OK;
}

void Worker_Matmul::execute_task( MWTask *t ) 
{
  int i, j, k;
  
  Task_Matmul *tf = (Task_Matmul *) t;
  
  tf->results = new int*[tf->endRow - tf->startRow + 1];
  for ( i = tf->startRow; i <= tf->endRow; i++ )
    {
      tf->results[i - tf->startRow] = new int[tf->nCols];
      for ( k = 0; k < num_colsB; k++ )
	tf->results[i - tf->startRow][k] = 0;
      
      for ( k = 0; k < num_rowsB; k++ )
	for ( j = 0; j < num_colsB; j++ )
	  tf->results[i - tf->startRow][j] += A[i][k] * B[k][j];
    }
}

MWTask*
Worker_Matmul::gimme_a_task()
{
    /* The MWWorker needs this.  Just return a Task_Matmul instance. */
    return new Task_Matmul;
}

MWWorker*
gimme_a_worker ()
{
    return new Worker_Matmul;
}
