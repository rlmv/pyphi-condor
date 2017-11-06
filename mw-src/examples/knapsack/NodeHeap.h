// $Id: NodeHeap.h,v 1.8 2005/12/12 20:17:57 wasu Exp $

#ifndef NODEHEAP_H
#define NODEHEAP_H

#include <vector>

class KnapNode;
class KnapTask;

class NodeHeap {

public:
  enum Type { VALUE, DEPTH, WORST };

public:

  friend class KnapTask;
  friend std::ostream &operator<<(std::ostream &stream, NodeHeap &heap);

  NodeHeap(Type type) : type_(type) {}
  virtual ~NodeHeap() {}

  double getAverageNodeLevel() const;
  bool empty() const { return nodes_.empty(); }
  double getBestBound() const;
  int getDeepestLevel() const;
  void pop();
  void print(std::ostream &o) const;
  void print() const { print(std::cout); }
  void push(KnapNode *n);
  void setType(Type type);
  KnapNode *top() const { return (nodes_.front()); }
  unsigned int size() const { return (nodes_.size()); }
  
private:
  std::vector<KnapNode *> nodes_;
  Type type_;
  
};

std::ostream &operator<<(std::ostream &stream, NodeHeap &heap);

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
