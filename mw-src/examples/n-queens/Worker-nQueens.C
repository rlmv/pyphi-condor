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
/* Worker-nQueens.C */

#include "Worker-nQueens.h"
#include "Task-nQueens.h"

Worker_nQueens::Worker_nQueens() 
{
	workingTask = new Task_nQueens;
	N = -1;
}

Worker_nQueens::~Worker_nQueens() 
{
	delete workingTask;
}

MWReturn Worker_nQueens::unpack_init_data( void ) 
{
	RMC->unpack ( &N, 1 );
	RMC->unpack ( &partition_factor, 1 );

	return OK;
}

void Worker_nQueens::execute_task( MWTask *t ) 
{
	int permutation[N];
	int i;
	int givenPerm = partition_factor >= N ? 0 : N - partition_factor;
	int index;

	Task_nQueens *tf = (Task_nQueens *) t;
	index = tf->num;

	for ( i = 0; i < givenPerm; i++ )
	{
		permutation[i] = index % N;
		index = index / N;
	}

	tf->results = search_subspace ( permutation, N, givenPerm );
}

MWWorker*
gimme_a_worker ()
{
    return new Worker_nQueens;
}

MWTask* Worker_nQueens::gimme_a_task ()
{
	return new Task_nQueens;
}

int *
Worker_nQueens::search_subspace ( int *permutation, int N, int givenPerm )
{
	int *result;

	if ( givenPerm == N )
		return isCorrectPermutation ( permutation, N );

	for ( int i = 0; i < N; i++ )
	{
		permutation[givenPerm] = i;
		result = search_subspace ( permutation, N, givenPerm + 1 );
		if ( result ) return result;
	}

	return NULL;
}

int *
Worker_nQueens::isCorrectPermutation ( int *permutation, int N )
{
	int *res;
	int i, j;

	for ( i = 0; i < N; i++ )
	{
		for ( j = 0; j < N; j++ )
		{
			if ( i == j ) continue;

			if ( permutation[i] == permutation[j] )
				return NULL;

			if ( permutation[i] + ( j - i ) == permutation[j] )
				return NULL;

		}
	}

	res = new int[N];
	for ( i = 0; i < N; i++ )
	{
		res[i] = permutation[i];
	}
	return res;
}
