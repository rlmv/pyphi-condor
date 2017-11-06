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
/* Driver-Matmul.C

These methods specific to the matrix multiplication application

*/

#include "MW.h"
#include "Driver-Matmul.h"
#include "Worker-Matmul.h"
#include "Task-Matmul.h"

const int Driver_Matmul::partition_factor = 1;

Driver_Matmul::Driver_Matmul() 
{
	A = B = C = NULL;
	num_rowsA = num_rowsB = num_colsB = 0;
	num_tasks = 0;
}

Driver_Matmul::~Driver_Matmul() 
{
	if ( A ) {
		for (int i=0; i < num_rowsA; i++)
			delete [] A[i];
		delete []A;
	}
	if ( B ) {
		for (int i=0; i < num_rowsB; i++)
			delete [] B[i];
		delete []B;
	}
	if ( C ) {
		for (int i=0; i < num_rowsA; i++)
			delete [] C[i];
		delete []C;
	}
}

MWReturn Driver_Matmul::get_userinfo( int argc, char *argv[] ) 
{
	int i, j, numarches;

	/* File format of the input file (which is stdin)
             # architectures 
             worker_executable_name condor_requirements_string
             worker_executable_name condor_requirements_string
             ...
             num_rowsA num_rowsB num_colsB
	*/

	scanf ( "%d", &numarches );
	if (numarches<1 || numarches>5) {
	  MWprintf ( 80, " ERROR: Number of Architectures should be 1,2,3,4,or 5\n");
	  return ABORT;
	}

	for ( i=0 ; i<numarches ; i++ ) 
	{
		char executable[80];
		char requirements[80];
		scanf("%s %s", executable, requirements);

		MWprintf(30, "Adding executable %s for %s\n", executable, requirements);
	    RMC->add_executable( executable, requirements);
	} 

	// read the dimensions

	scanf ( "%d %d %d", &num_rowsA, &num_rowsB, &num_colsB );

	// allocate space for the matrices
	
	A = new double*[num_rowsA];
	for ( i = 0; i < num_rowsA; i++ )
	  A[i] = new double[num_rowsB];
	
	B = new double*[num_rowsB];
	for ( i = 0; i < num_rowsB; i++ )
	  B[i] = new double[num_colsB];
	
	C = new double*[num_rowsA];
	for ( i = 0; i < num_rowsA; i++ )
	  C[i] = new double[num_colsB];
	
	// fill A and B with stuff. (Later perhaps use a random-number
	// generator here; for the moment just make all the elements
	// 1.0.)

	for ( i = 0; i < num_rowsA; i++ )
	  for ( j = 0; j < num_rowsB; j++ )
	    A[i][j] = 1.e0;

	for ( i = 0; i < num_rowsB; i++ )
	  for ( j = 0; j < num_colsB; j++ )
	    B[i][j] = 1.e0;

	for ( i = 0; i < num_rowsA; i++ )
	  for ( j = 0; j < num_colsB; j++ )
	    C[i][j] = 0.e0;
	
	set_checkpoint_frequency ( 50 );

	int target_num_workers=40;
	RMC->set_target_num_workers( target_num_workers );

	// max of 100 tasks
	num_tasks = (num_rowsA <= 100) ? num_rowsA : 100; 
	
	return OK;
}

MWReturn Driver_Matmul::setup_initial_tasks(int *n_init , MWTask ***init_tasks) 
{
  
  int i;
  int startRow, endRow;
  
  // Partition into num_tasks tasks by taking slices of the result
  // matrix C (and hence of A).
  
  *n_init = num_tasks;
  *init_tasks = new MWTask *[num_tasks];
  
  startRow=0;
  for ( i = 0 ; i < num_tasks ; i++ ) 
    {
      // size of this slice
      endRow = startRow + (num_rowsA - startRow) / (num_tasks-i)-1;
      if(endRow < startRow) endRow = startRow;
      (*init_tasks)[i] = new Task_Matmul( startRow, endRow, num_colsB );
      startRow = endRow + 1;
    }
  
  return OK;
}



MWReturn Driver_Matmul::act_on_completed_task( MWTask *t ) 
{
  int i, j;
  
  Task_Matmul *tf = dynamic_cast<Task_Matmul *> ( t );
  
  for ( i = tf->startRow; i <= tf->endRow; i++ )
    for ( j = 0; j < num_colsB; j++ )
      C[i][j] = tf->results[i - tf->startRow][j];
  
  return OK;
}


MWReturn Driver_Matmul::pack_worker_init_data( void ) 
{
  // Pass the entire A and B matrices to the workers.

  int i;
  
  RMC->pack( &num_rowsA, 1 );
  RMC->pack( &num_rowsB, 1 );
  RMC->pack( &num_colsB, 1 );
  
  for ( i = 0; i < num_rowsA; i++ )
    RMC->pack ( A[i], num_rowsB );
  
  for ( i = 0; i < num_rowsB; i++ )
    RMC->pack ( B[i], num_colsB );
  
  return OK;
}

void Driver_Matmul::printresults() 
{
  MWprintf ( 10, "Product matrix has been computed!\n");
  MWprintf ( 10, "If you dont see the result below, and wish to,\n call set_MWprintf_level() to set the print level to at least 95\n");
   
  for ( int i = 0; i < num_rowsA; i++ )
    {
      for ( int j = 0; j < num_colsB; j++ )
	MWprintf ( 95, "%12.4e ", C[i][j] );
      
      MWprintf ( 95, "\n" );
    }
  
}

void
Driver_Matmul::write_master_state( FILE *fp ) 
{
  int i, j;

  MWprintf ( 10, "CHECKPOINTING!!\n");

  fprintf( fp, "%d %d %d\n", num_rowsA, num_rowsB, num_colsB);

  // write out the input matrices A and B
  for ( i = 0; i < num_rowsA; i++ )
    for ( j = 0; j < num_rowsB; j++ )
      fprintf ( fp, "%.12e\n", A[i][j]);

  for ( i = 0; i < num_rowsB; i++ )
    for ( j = 0; j < num_colsB; j++ )
      fprintf ( fp, "%.12e\n", B[i][j]);

  // write out the result matrix C (the whole thing, both rows that
  // have and have not been computed)

  for ( i = 0; i < num_rowsA; i++ )
    for ( j = 0; j < num_colsB; j++ )
      fprintf ( fp, "%.12e\n", C[i][j]);

}

void 
Driver_Matmul::read_master_state( FILE *fp ) 
{
  int i, j;
  double XX;

  MWprintf ( 10, "Restarting from checkpoint file\n");

  fscanf( fp, "%d %d %d", &num_rowsA, &num_rowsB, &num_colsB);

  // read in the input matrices A and B
  for ( i = 0; i < num_rowsA; i++ )
    for ( j = 0; j < num_rowsB; j++ ) {
      fscanf ( fp, "%lf", &XX);
      A[i][j] = XX;
    }

  for ( i = 0; i < num_rowsB; i++ )
    for ( j = 0; j < num_colsB; j++ ) {
      fscanf ( fp, "%lf", &XX);
      B[i][j] = XX;
    }

  // read in the result matrix C (the whole thing, both rows that have
  // and have not been computed)

  for ( i = 0; i < num_rowsA; i++ )
    for ( j = 0; j < num_colsB; j++ ) {
      fscanf ( fp, "%lf", &XX);
      C[i][j] = XX;
    }

}

MWTask*
Driver_Matmul::gimme_a_task() 
{
  // The MWDriver needs this. Just return a Task_Matmul instance.

  return new Task_Matmul;
}

MWDriver*
gimme_the_master () 
{
  return new Driver_Matmul;
}
