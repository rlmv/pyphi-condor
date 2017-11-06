// $Id: KnapTask.C,v 1.13 2005/12/12 20:14:46 wasu Exp $

#include <cassert>
#include <iostream>

#include <MWprintf.h>

#include "KnapTask.h"

using namespace std;

KnapTask::~KnapTask()
{
}

void
KnapTask::pack_results()
{
  int fis = foundImprovedSolution_ ? 1 : 0;
  RMC->pack(&fis, 1);
  if (fis) {
    RMC->pack(&solutionValue_, 1);
  }
  RMC->pack(&numNodesEvaluated_, 1);
  int n = (int) outputNode_.size();
  RMC->pack(&n, 1);
  for (vector<KnapNode *>::const_iterator it = outputNode_.begin();
       it != outputNode_.end(); ++it) {
    (*it)->MWPack(RMC);
  }
}

void
KnapTask::pack_work()
{
  inputNode_.MWPack(RMC);
  RMC->pack(&benchMaxNodes_, 1);
}

void
KnapTask::write_ckpt_info(FILE *fp)
{
  inputNode_.write(fp);
  fprintf(fp, "%d\n", benchMaxNodes_);
  return; 
}

void
KnapTask::read_ckpt_info(FILE *fp)
{
  inputNode_.read(fp);
  fscanf(fp, "%d", &benchMaxNodes_);
  return;
}

void
KnapTask::printself(int level)
{
  int current_level = get_MWprintf_level();
  if (level < current_level) {
    cout << "Received Knapsack Task: " << number << endl;
    cout << inputNode_ << endl;
  }
}

void 
KnapTask::setBetterSolution(double value)
{
  foundImprovedSolution_ = true;
  solutionValue_ = value;
}

void
KnapTask::unpack_results()
{
  int fis;
  RMC->unpack(&fis, 1);
  foundImprovedSolution_ = (fis == 0) ? false : true;
  if (foundImprovedSolution_) {
    RMC->unpack(&solutionValue_, 1);
  }
  RMC->unpack(&numNodesEvaluated_, 1);
  assert(outputNode_.size() == 0);
  int n;
  RMC->unpack(&n, 1);
  for (int i = 0; i < n; i++) {
    KnapNode *node = new KnapNode();
    node->MWUnpack(RMC);
    outputNode_.push_back(node);
  }
}

void
KnapTask::unpack_work()
{
  inputNode_.MWUnpack(RMC);
  RMC->unpack(&benchMaxNodes_, 1);
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

