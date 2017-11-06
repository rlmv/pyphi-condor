// $Id: KnapTask.h,v 1.10 2005/12/12 20:14:46 wasu Exp $

#ifndef KNAP_TASK_H
#define KNAP_TASK_H

#include <cfloat>
#include <vector>

#include <MWTask.h>

#include "KnapNode.h"
#include "NodeHeap.h"

class KnapTask : public MWTask 
{

public:

  KnapTask(): benchMaxNodes_(INT_MAX) {}
  KnapTask(const KnapNode node): inputNode_(node), benchMaxNodes_(INT_MAX) {}
  KnapTask(const KnapNode node, int benchMaxNodes):                  
    inputNode_(node), benchMaxNodes_(benchMaxNodes) {}   
  
  virtual ~KnapTask();

  void addNodesInHeap(NodeHeap &heap) { outputNode_ = heap.nodes_; }
  bool foundImprovedSolution() const { return foundImprovedSolution_; }
  const KnapNode & getInputNode() const { return inputNode_; }
  double getNumNodesEvaluated() const { return numNodesEvaluated_; }
  double getSolutionValue() const { return solutionValue_; }
  int getBenchMaxNodes() const { return benchMaxNodes_; }
 
  std::vector<KnapNode *>::const_iterator constNewNodeBegin() const { return outputNode_.begin(); }
  std::vector<KnapNode *>::const_iterator constNewNodeEnd() const { return outputNode_.end(); }
    
  /// Pack the results from this task into the RMComm buffer
  void pack_results( void );

  /// Pack the work for this task into the RMComm buffer
  void pack_work( void );

  /** Checkpointing Methods */
  void write_ckpt_info(FILE *fp);
  void read_ckpt_info(FILE *fp);

  /// Prints himself
  void printself(int level);

  void setBetterSolution(double value);
  void setNumNodesEvaluated(int nn) { numNodesEvaluated_ = nn; }

  /// Unpack the results from this task into the RMComm buffer
  void unpack_results( void );

  /// Unpack the work for this task from the RMComm buffer
  void unpack_work( void );

private:

  // Input parameter
  KnapNode inputNode_;
  
  // Output parameters
  bool foundImprovedSolution_;
  double solutionValue_;
  double numNodesEvaluated_;
  std::vector<KnapNode *> outputNode_;

  // Benchmark parameter
  int benchMaxNodes_;

};

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
