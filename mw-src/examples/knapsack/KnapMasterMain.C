/*
 */
// $Id: KnapMasterMain.C,v 1.3 2005/07/01 15:11:52 wasu Exp $

#include "KnapMaster.h"

int 
main(int argc, char *argv[]) 
{
  //set_MWprintf_level( 90 );

  KnapMaster *km = new KnapMaster();
  km->go( argc, argv );
  delete km;
  return 0;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
