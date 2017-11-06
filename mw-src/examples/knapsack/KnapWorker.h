// $Id: KnapWorker.h,v 1.11 2005/12/12 20:14:46 wasu Exp $

#ifndef KNAP_WORKER_H
#define KNAP_WORKER_H

#include <MWWorker.h>

#include "KnapInstance.h"
#include "NodeHeap.h"

class MWTask;

class KnapWorker : public MWWorker
{

public:

  KnapWorker();
  ~KnapWorker();

  /// Compute the result from knapsack algorithm
  void execute_task(MWTask *);	 
  
  /// Check if the grain size limit is reached
  bool finished(NodeHeap * &heap);

  bool inCleanup() const { return cleanup_; }
  bool inPhaseOne() const { return !cleanup_; }

  /// Unpack initial data sent from master upon startup
  MWReturn unpack_init_data();
  
  /// Unpack the data that is part of the task 
  void unpack_driver_task_data();

private:

  // Are you in "cleanup" mode.
  bool cleanup_;

  // Are you in "multi-level cleanup" mode.
  bool moreCleanup_;

  // "Ticks" for the execution of the current task
  double begTime_;
  double curTime_;
  int numNodes_;
  double bestLowerBound_;
  int maxNodes_;
  double maxTime_;
  KnapTask *curTask_;

  KnapInstance instance_;

  // Parameters for clean-up phase
  NodeHeap::Type currentNodeOrder_;
  double cleanupTime_;
  double cleanupFactor_;
  int targetCleanupDepth_;

};

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
