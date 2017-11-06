// $Id: KnapParam.h,v 1.8 2005/12/12 20:17:57 wasu Exp $

#ifndef KNAP_PARAM_H
#define KNAP_PARAM_H

#include "NodeHeap.h"

/** Bound must be at least "this much" better than the current lower bound
    to be considered for evaluation
*/
static const double KnapBetterBy = 1.0e-8; 

/** Number less than this are considered to be <= 0
*/
static const double KnapZeroTolerance = 1.0e-6;

class KnapParam
{

public:

  friend std::istream &operator>>(std::istream &stream, KnapParam &p);

  KnapParam();
  virtual ~KnapParam() {}

  double getBegEndCpuTimeWorker() const { return begEndCpuTimeWorker_; }
  int getBegEndNodeLevel() const { return begEndNodeLevel_; }
  int getCheckpointFrequency() const { return checkpointFrequency_; } 
  double getCleanupCpuTimeWorker() const { return cleanupCpuTimeWorker_; }
  double getCleanupFactorWorker() const { return cleanupFactorWorker_; }
  NodeHeap::Type getMasterNodeOrder() const { return masterNodeOrder_; }
  double getMaxCpuTimeWorker() const { return maxCpuTimeWorker_; }
  int getMaxNodeWorker() const { return maxNodeWorker_; }
  int getNodeHighWater() const { return nodeHighWater_; }
  int getNodeLowWater() const { return nodeLowWater_; }
  int getTargetNumWorkers() const { return targetNumWorkers_; }
  NodeHeap::Type getWorkerNodeOrder() const { return workerNodeOrder_; }

  /** Checkpointing Methods*/
  void write(FILE *);
  void read(FILE *);
  
private:

  NodeHeap::Type masterNodeOrder_;
  double maxCpuTimeWorker_;
  int maxNodeWorker_;
  int targetNumWorkers_;
  NodeHeap::Type workerNodeOrder_;

  /** If the number of nodes in the master queue is "too small", i.e. 
   *  < begEndNodeLevel_,  then we set the CPU time in the worker to be 
   *  at most this amount of time, begEndCpuTimeWorker_.
   */
  double begEndCpuTimeWorker_;
  int begEndNodeLevel_;

  /**
   * if number of nodes is < nodeLowWater, then we want the master to send a
   *  node with large upper bound.
   *
   * if number of nodes is > nodeHighWater_, then we want the master to send a 
   *  "deep" node
   */
  int nodeLowWater_;
  int nodeHighWater_;

  // The amount of "extra" time to try and cleanup "bad" nodes in the worker.
  double cleanupCpuTimeWorker_;

  // The factor that determines target cleanup depth.
  double cleanupFactorWorker_;

  // The frequency (in second) in which the current states are written to file.
  int checkpointFrequency_;

};

std::istream &operator>>(std::istream &stream, KnapParam &p);

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

