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
/* Task-Matmul.C */

#include "Task-Matmul.h"
#include "MW.h"

#include "MWRMComm.h"
#include <string.h>

Task_Matmul::Task_Matmul() 
{
  startRow = endRow = -1;
  nCols = 0;
  results = NULL;
}

Task_Matmul::Task_Matmul( int strtRow, int edRow, int ncols )
{
  startRow = strtRow;
  endRow = edRow;
  nCols = ncols;
  
  results = NULL;
}

Task_Matmul::~Task_Matmul() 
{
  if ( results ) {
    for ( int i = startRow; i <= endRow; i++ ) 
      delete [] results[i-startRow];   // make Insure happy
    delete [] results;
  }
}

void
Task_Matmul::printself( int level ) 
{
  MWprintf( level, "This task computes rows %d through %d of C\n",
	    startRow, endRow);
}


void Task_Matmul::pack_work( void ) 
{
  RMC->pack( &startRow, 1, 1 );
  RMC->pack( &endRow, 1, 1 );
  RMC->pack( &nCols, 1, 1 );
}


void Task_Matmul::unpack_work( void ) 
{
  RMC->unpack( &startRow, 1, 1 );
  RMC->unpack( &endRow, 1, 1 );
  RMC->unpack( &nCols, 1, 1 );
}


void Task_Matmul::pack_results( void ) 
{
  for ( int i = startRow; i <= endRow; i++ ) {
    RMC->pack( results[i-startRow], nCols, 1 );
    delete [] results[i-startRow];	// make Insure happy
  }
  
  delete [] results;
  results = NULL;
}


void Task_Matmul::unpack_results( void ) 
{
  results = new int*[endRow - startRow + 1];
  for ( int i = startRow; i <= endRow; i++ )
    {
      results[i - startRow] = new int[nCols];
      RMC->unpack( results[i - startRow], nCols, 1 );
    }
}

void Task_Matmul::write_ckpt_info( FILE *fp ) 
{
  MWprintf ( 50, "checkpointing task\n");
  fprintf ( fp, "%d %d %d\n", startRow, endRow, nCols);

}

void Task_Matmul::read_ckpt_info( FILE *fp ) 
{
  MWprintf ( 50, "recovering task from checkpoint\n");
  fscanf ( fp, "%d %d %d", &startRow, &endRow, &nCols);

}
