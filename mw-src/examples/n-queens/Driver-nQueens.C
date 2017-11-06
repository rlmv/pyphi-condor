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
/* Driver-nQueens.C

These methods specific to the matrix multiplication application

*/

#include <math.h>

#include "MW.h"
#include "Driver-nQueens.h"
#include "Worker-nQueens.h"
#include "Task-nQueens.h"

Driver_nQueens::Driver_nQueens() 
{
	N = -1;
	solution = NULL;
	num_tasks = 0;
}

Driver_nQueens::~Driver_nQueens() 
{
	if ( solution ) delete []solution;
}

MWReturn Driver_nQueens::get_userinfo( int argc, char *argv[] ) 
{
	int i, j, numarches;
	char exec[_POSIX_PATH_MAX];
	int num_exe;

	/* File format of the input file (which is stdin)
	     # arches worker_executables ...

         num_rowsA num_rowsB num_colsB
	 A B C
	*/

	RMC->set_num_exec_classes ( 1 );
	scanf ( "%d", &numarches );
	RMC->set_num_arch_classes( numarches );
	for ( i=0 ; i<numarches ; i++ ) 
	{
		if ( i == 0 )
			RMC->set_arch_class_attributes ( i, "((Arch==\"INTEL\") && (Opsys==\"LINUX\") )" );
		else
			RMC->set_arch_class_attributes ( i, "((Arch==\"INTEL\") && (Opsys==\"SOLARIS26\") )" );
	} 
	scanf ( "%d", &num_exe );
	RMC->set_num_executables( num_exe );

	for ( i = 0; i < num_exe; i++ )
	{
		scanf ( "%s %d", exec, &j );
		MWprintf ( 30, " %s\n", exec );
		RMC->add_executable( 0, j, exec, "");
	} 

	scanf ( "%d ", &N );

	solution = new int[N];

	set_checkpoint_frequency ( 100 );

	RMC->set_target_num_workers( 10 );

	num_tasks = partition_factor >= N ? 1 : (int)pow((double)(N - partition_factor), (double)(N - partition_factor));

	return OK;
}

MWReturn Driver_nQueens::setup_initial_tasks(int *n_init , MWTask ***init_tasks) 
{

	int i;

/* Basically we have to tell MW Layer of the number of tasks and what they are */

	*n_init = num_tasks;
	*init_tasks = new MWTask *[num_tasks];

/* And now we make the Task_nQueens instances.  They will basically contain the 
   rows to operate upon.
*/

	for ( i = 0 ; i < num_tasks ; i++ ) 
	{
		(*init_tasks)[i] = new Task_nQueens( N, i );
		MWprintf ( 10, "Added the task %p\n", (*init_tasks)[i] );
	}

	return OK;
}



MWReturn Driver_nQueens::act_on_completed_task( MWTask *t ) 
{

	int i;

#ifdef NO_DYN_CAST
	Task_nQueens *tf = (Task_nQueens *) t;
#else
	Task_nQueens *tf = dynamic_cast<Task_nQueens *> ( t );
#endif

	if ( tf->results )
	{
		// Result has been computed.
		for ( i = 0; i < N; i++ )
			solution[i] = tf->results[i];

		printresults ( );
		//RMC->exit(0);
		//::exit(0);
	}
	
	return OK;
}

MWReturn Driver_nQueens::pack_worker_init_data( void ) 
{
	int part = partition_factor;
	// We pass the N.
	RMC->pack( &N, 1 );
	RMC->pack( &part, 1 );

	return OK;
}

void Driver_nQueens::printresults() 
{
	MWprintf ( 10, "The resulting Matrix is as follows\n");

	for ( int i = 0; i < N; i++ )
	{
		MWprintf ( 10, "%d ", solution[i] );
	}
	MWprintf ( 10,"\n" );
}

void
Driver_nQueens::write_master_state( FILE *fp ) 
{
	// This is not checkpoint enabled.
}

void 
Driver_nQueens::read_master_state( FILE *fp ) 
{
	// This is not checkpoint enabled.
}

MWTask*
Driver_nQueens::gimme_a_task() 
{
	/* The MWDriver needs this.  Just return a Task_nQueens instance. */
	return new Task_nQueens;
}

MWDriver*
gimme_the_master () 
{
    return new Driver_nQueens;
}
