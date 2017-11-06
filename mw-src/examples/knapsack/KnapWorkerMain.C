/*
 */
// $Id: KnapWorkerMain.C,v 1.1 2005/05/26 20:35:29 wasu Exp $

#include "KnapWorker.h"

int 
main(int argc, char *argv[]) 
{

  KnapWorker *kw = new KnapWorker();
  kw->go( argc, argv );
  delete kw;
  return 0;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
