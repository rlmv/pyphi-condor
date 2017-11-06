// $Id: NodeHeap.C,v 1.6 2005/12/12 20:17:57 wasu Exp $

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <iostream>

#include "KnapNode.h"
#include "NodeHeap.h"

using namespace std;

bool valueLessThan(const KnapNode *n1, const KnapNode *n2) 
{
  return (n1->getUpperBound() < n2->getUpperBound());
}

bool depthLessThan(const KnapNode *n1, const KnapNode *n2)
{
  return (n1->getDepth() < n2->getDepth());
}

double 
NodeHeap::getAverageNodeLevel() const
{
  int sum = 0;
  for(vector<KnapNode *>::const_iterator it = nodes_.begin();
      it != nodes_.end(); ++it) {
    const KnapNode *n = *it;
    sum += n->getDepth();
  }
  return ( ((double) sum)/nodes_.size() );
}

double
NodeHeap::getBestBound() const
{
  double retval = DBL_MAX;
  if (nodes_.size() > 0) {
    if (type_ == VALUE) {
      retval = nodes_.front()->getUpperBound();
    }
    else {
      vector<KnapNode *>::const_iterator it = max_element(nodes_.begin(), nodes_.end(), 
                                                          valueLessThan);
      retval = (*it)->getUpperBound();
    }
  }
  return retval;
}

int
NodeHeap::getDeepestLevel() const
{
  int retval = 0;
  if (nodes_.size() > 0) {
    if (type_ == DEPTH) {
      retval = nodes_.front()->getDepth();
    }
    else {
      vector<KnapNode *>::const_iterator it = max_element(nodes_.begin(), nodes_.end(), 
                                                          depthLessThan);
      retval = (*it)->getDepth();
    }
  }
  return retval;
}

void 
NodeHeap::pop()
{
  switch(type_) {
  case (VALUE):
    pop_heap(nodes_.begin(), nodes_.end(), valueLessThan);
    break;
  case (DEPTH):
    pop_heap(nodes_.begin(), nodes_.end(), depthLessThan);
    break;
  default:
    assert(0);
  }
  nodes_.pop_back();
}

void 
NodeHeap::print(ostream &o) const
{
  for(vector<KnapNode *>::const_iterator it = nodes_.begin();
      it != nodes_.end(); ++it) {
    const KnapNode *n = *it;
    o << (*n) << endl;
  }
}

void 
NodeHeap::push(KnapNode *n)
{
  nodes_.push_back(n);
  switch(type_) {
  case (VALUE):
    push_heap(nodes_.begin(), nodes_.end(), valueLessThan);
    break;
  case (DEPTH):
    push_heap(nodes_.begin(), nodes_.end(), depthLessThan);
    break;
  default:
    assert(0);
  }
}

void
NodeHeap::setType(Type type)
{
  if (type == type_) return;
   
  switch(type_) {
  case (VALUE):
    make_heap(nodes_.begin(), nodes_.end(), valueLessThan);
    break;
  case (DEPTH):
    make_heap(nodes_.begin(), nodes_.end(), depthLessThan);
    break;
  default:
    assert(0);
  }
}

std::ostream &operator<<(std::ostream &stream, NodeHeap &heap)
{
  stream << "Heap has: " << heap.nodes_.size() << " nodes." << endl;  
  return stream;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
