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
/* Task-nQueens.C */

#include "Task-nQueens.h"
#include "MW.h"

#include "MWRMComm.h"
#include <string.h>

Task_nQueens::Task_nQueens() 
{
	N = -1;
	num = 0;
	results = NULL;
}

Task_nQueens::Task_nQueens( int n, int Num )
{
	N = n;
	num = Num;
	results = NULL;
}

Task_nQueens::~Task_nQueens() 
{
    if ( results )
        delete [] results;
}

void
Task_nQueens::printself( int level ) 
{
	// Right now nothing
}


void Task_nQueens::pack_work( void ) 
{
	RMC->pack( &N, 1, 1 );
	RMC->pack( &num, 1, 1 );
}


void Task_nQueens::unpack_work( void ) 
{
	RMC->unpack( &N, 1, 1 );
	RMC->unpack( &num, 1, 1 );
}


void Task_nQueens::pack_results( void ) 
{
	int res;
	if ( results ) res = 1;
	else res = 0;
	RMC->pack ( &res, 1, 1 );

	if ( res == 1 )
	{
		RMC->pack( results, N, 1 );
		delete [] results;
		results = NULL;
	}
}


void Task_nQueens::unpack_results( void ) 
{
	int res;
	RMC->unpack ( &res, 1, 1 );

	if ( res == 1 )
	{
		results = new int[N];
    		RMC->unpack( results, N, 1 );
	}
}

void Task_nQueens::write_ckpt_info( FILE *fp ) 
{
	// Not checkpoint enabled.
}

void Task_nQueens::read_ckpt_info( FILE *fp ) 
{
	// Not checkpoint enabled.
}
