// $Id: KnapMaster.h,v 1.10 2005/12/12 20:14:46 wasu Exp $

#ifndef KNAP_MASTER_H
#define KNAP_MASTER_H

#include <MWDriver.h>

#include "KnapInstance.h"
#include "KnapParam.h"
#include "NodeHeap.h"

class KnapMaster : public MWDriver
{

public:

  KnapMaster();
  virtual ~KnapMaster();

  /// Define what actions to be done after a task finishes 
  MWReturn act_on_completed_task(MWTask *);

  /// Process arguments and do basic setup
  MWReturn get_userinfo(int argc, char *argv[]);
  
  /// Create a new task
  MWTask *gimme_a_task();

  /// Pack initial data to be sent to worker upon startup
  MWReturn pack_worker_init_data();

  /// Pack the data that is part of the task
  void pack_driver_task_data();
  
  void printresults();
  
  /** Checkpointing Methods */
  void write_master_state(FILE *fp);
  void read_master_state(FILE *fp);

  /// Return the address of an array of pointers to the root node
  MWReturn setup_initial_tasks(int *, MWTask ***);
  
  void setMasterNodeOrder(NodeHeap::Type type, bool sort_it = false);

private:

  KnapInstance instance_;
  KnapParam param_;
  double numNodesEvaluated_;   // double since may be > 2^31-1

  NodeHeap::Type masterNodeOrder_;

  // Control parameters
  double bestLowerBound_;
  int maxNodes_;
  double maxTime_;  

  // Private method
  double getBestUpperBound();

};

#endif
// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
