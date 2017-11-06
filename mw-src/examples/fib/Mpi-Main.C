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


// C++ Implementation: Master-Driver-fib
//
// Description: 
//
// Author: Sonya Marcarelli <sonya.marcarelli@unisannio.it>, (C) 2005
//         (Minor changes by Jeff Linderoth <jtl3@lehigh,edu>)
//

/* Master-Worker-fib.C
*/

#include "MW.h"
#include "Driver-fib.h"
#include "Worker-fib.h"
#include "MWStaticMPIRC.h"
#include "MWRMComm.h"

int main( int argc, char *argv[] ) {
	
  // Shouldn't need do call this more than once...
  //MWRMComm * GlobalRMComm = new MWStaticMPIRC ();
  MWGlobalMPIRMComm->init_env(argc,argv); 
        
  if (MWGlobalMPIRMComm->is_master_proc()) {
    Driver_Fib *advisor = new Driver_Fib ( );
    /* might as well set debugging level here... */
    set_MWprintf_level( 99 );
    MWprintf ( 10, "The master is starting.\n" );
    advisor->go( argc, argv );
  }
  else {
    Worker_Fib graduate_student;
    // Set up debug level here...
    set_MWprintf_level ( 75 );
    MWprintf ( 10, "A worker is starting.\n" );
    for ( int i = 1; i < argc; i++ )
      MWprintf(10, " Args %d was %s\n", i, argv[i] );
    graduate_student.go( argc, argv );
  }
  return 0;
}
