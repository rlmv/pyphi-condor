// $Id: KnapWorker.C,v 1.20 2005/12/12 20:14:46 wasu Exp $

#include <cassert>

#include <MWSystem.h>

#include "KnapNode.h"
#include "KnapTask.h"
#include "KnapParam.h"
#include "KnapWorker.h"
#include "NodeHeap.h"

using namespace std;

#define CLEANUP

MWWorker *
gimme_a_worker()
{
   return new KnapWorker;
}

KnapWorker::KnapWorker()
{
  workingTask = new KnapTask();
  
  cleanup_ = false;
  moreCleanup_ = false;
  begTime_ = 0.0;
  curTime_ = 0.0;
  numNodes_ = 0;
  bestLowerBound_ = -DBL_MAX;
  maxNodes_ = 10000;
  maxTime_ = 10;
  curTask_ = NULL;
  targetCleanupDepth_ = INT_MAX;
}

KnapWorker::~KnapWorker()
{
}

void
KnapWorker::execute_task(MWTask *t)
{
  KnapTask *kt = dynamic_cast<KnapTask *> (t);
  assert(kt);

  begTime_ = MWSystem::gettimeofday();
  curTime_ = begTime_;
  numNodes_ = 0;
  curTask_ = kt;
  cleanup_ = false;
  moreCleanup_ = false;

  NodeHeap *heap = new NodeHeap(currentNodeOrder_);
  heap->push(new KnapNode(kt->getInputNode()));
    
  while (!finished(heap)) {

    KnapNode *node = heap->top();
    heap->pop();

    // Cram as many "Free" items as possible into the knapsack
    // The items have already been sorted
    // ix is the last item you were able to fit

    // Solve "LP relaxation"
    double remainingSize = instance_.getCapacity() - node->getUsedCapacity();
    double usedValue = node->getUsedValue();

    int ix = 0;
    double ixSize = 0.0;
    double ixProfit = 0.0;
    for (KnapInstance::itemIterator it = instance_.itemsBegin(); 
         it != instance_.itemsEnd(); ++it) {
      if (node->varStatus(ix) == Free) {
        ixSize = (*it).getSize();
        ixProfit = (*it).getProfit();      
        remainingSize -= ixSize;
        usedValue += ixProfit;
      }
      if (remainingSize < KnapZeroTolerance) break;
      ix++;
    }

    // Check to see if it is feasible, compute bound, 
    // else take it out if not feasible
    bool branch = false;
    double nodeUb = DBL_MAX;
    double nodeLb = bestLowerBound_;
    if (remainingSize >= 0) {
      // Solution is feasible.
      nodeLb = usedValue;
      nodeUb = usedValue;
    }
    else {
      usedValue -= ixProfit;
      remainingSize += ixSize;
      nodeUb = usedValue + ixProfit/ixSize * remainingSize;
      nodeLb = usedValue;
      node->setUpperBound(nodeUb);
      if (nodeUb > bestLowerBound_ + KnapBetterBy) {
        branch = true;
      }
    }
    
    // Fathom and update solution if better bound is found
    if (nodeLb > bestLowerBound_) {
      kt->setBetterSolution(nodeLb);
      NodeHeap *newHeap = new NodeHeap(currentNodeOrder_);

      while(!heap->empty()) {
        KnapNode *n = heap->top();
        heap->pop();
        double newLb = nodeLb + (instance_.isIntegerObj() ? 1.0 : 0.0);
        
        if (n->getUpperBound() <= newLb) {
          delete n;
        }
        else {
          newHeap->push(n);
        }
      }
      delete heap;
      heap = newHeap;
    }

    // Branch on ix.
    if (branch) {
      heap->push(new KnapNode(*node, ix, FixedToZero, ixSize, ixProfit));
      if ( node->getUsedCapacity() + ixSize < instance_.getCapacity()) {
        heap->push(new KnapNode(*node, ix, FixedToOne, ixSize, ixProfit));
      }
    }

    delete node;
    numNodes_++;
  }
  
  // Done. Return the nodes left on the heap.
  kt->setNumNodesEvaluated(numNodes_);
  kt->addNodesInHeap(*heap);

  return;
}

bool
KnapWorker::finished(NodeHeap *&heap)
{
  if (heap->empty()) return true;
  double avgNodeDepth = heap->getAverageNodeLevel();
 
  if (inPhaseOne()) {
    curTime_ = MWSystem::gettimeofday();
    bool htl = curTime_ - begTime_ >= maxTime_;
    bool hnl = numNodes_ >= min(maxNodes_, curTask_->getBenchMaxNodes());
  
    if (htl || hnl) {
      if (cleanupTime_ <= 0.0 || hnl) return true;
      cleanup_ = true;

      // Calculate the average node depth in the queue
      MWprintf(30, "Cleanup: heap has average node depth of %f.  Deepest: %d\n", 
               avgNodeDepth, heap->getDeepestLevel());

      // Try to cleanup all nodes up to target cleanup depth
      targetCleanupDepth_ = (int) (cleanupFactor_ * avgNodeDepth);
      
      // Switch to DFS
      currentNodeOrder_ = NodeHeap::DEPTH;
      NodeHeap *newHeap = new NodeHeap(currentNodeOrder_);
      while(!heap->empty()) {
        KnapNode *n = heap->top();
        heap->pop();
        newHeap->push(n);
      }
      delete heap;
      heap = newHeap;
    }    
  }
  else {
    curTime_ = MWSystem::gettimeofday();

#if defined(CLEANUP)
    if (heap->getDeepestLevel() <= targetCleanupDepth_) {
      MWprintf(30, "Cleanup: Stopping task, as popped up to max depth: %d\n", targetCleanupDepth_);
      return(true);
    }
    if (curTime_ - begTime_ >= maxTime_ + cleanupTime_) {
      MWprintf(30, "Cleanup: Hit max cleanup time.  Avg Node Depth: %f\n", avgNodeDepth);
      return (true);
    }
#endif
#if defined(CLEANUP2)
    if (heap->getDeepestLevel() <= targetCleanupDepth_ && !moreCleanup_) {
      MWprintf(30, "Cleanup: Stopping task, as popped up to max depth: %d\n", targetCleanupDepth_); 
      return(true);
    }
    if (!moreCleanup_) {
      if (curTime_ - begTime_ < maxTime_ + cleanupTime_) return false;
      moreCleanup_ = true;
      targetCleanupDepth_ = targetCleanupDepth_ + 5;
      cleanupTime_ = 1.5 * cleanupTime_;
    }
    else {
      if (heap->getDeepestLevel() <= targetCleanupDepth_) {
        MWprintf(30, "MultiLevelCleanup: Stopping task, as popped up to max depth: %d\n", targetCleanupDepth_);
        return(true);
      }
      if (curTime_ - begTime_ >= maxTime_ + cleanupTime_) {
        MWprintf(30, "Cleanup: Hit max cleanup time.  Avg Node Depth: %f.  Deepest: %d\n",
                 avgNodeDepth, heap->getDeepestLevel());
        return (true);
      }
    }
#endif
  }
  return(false);  
}

MWReturn
KnapWorker::unpack_init_data()
{
  MWReturn ustat = OK;
  instance_.MWUnpack(RMC);

  RMC->unpack(&maxNodes_, 1);
  RMC->unpack(&maxTime_, 1);
  int wno;
  RMC->unpack(&wno, 1);
  currentNodeOrder_ = (NodeHeap::Type) wno;
  RMC->unpack(&cleanupTime_, 1);
  RMC->unpack(&cleanupFactor_, 1);

  return(ustat);
}

void
KnapWorker::unpack_driver_task_data()
{
  RMC->unpack(&bestLowerBound_, 1);
  RMC->unpack(&maxNodes_, 1);
  RMC->unpack(&maxTime_, 1);
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

