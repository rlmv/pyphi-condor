// $Id: KnapNode.h,v 1.6 2005/12/12 20:17:57 wasu Exp $

#ifndef KNAP_NODE_H
#define KNAP_NODE_H

#include <iostream>
#include <vector>

class MWRMComm;

enum KnapVarStatus {
  Free,
  FixedToOne,
  FixedToZero,
};

class KnapNode
{

public:

  friend std::ostream &operator<<(std::ostream &stream, const KnapNode &node);

  KnapNode();
  KnapNode(int n);
  KnapNode(const KnapNode &rhs);
  KnapNode(const KnapNode &rhs, int ix, KnapVarStatus fv, 
           double ixSize, double ixProfit);

  virtual ~KnapNode() {}

  int getDepth() const { return depth_; }
  double getUpperBound() const { return ub_; }
  double getUsedCapacity() const { return usedCapacity_; }
  double getUsedValue() const { return usedValue_; }

  void setUpperBound(double val) { ub_ = val; }

  KnapVarStatus varStatus(int ix) const { return itemStatus_[ix]; }

  int MWPack(MWRMComm *r);
  int MWUnpack(MWRMComm *r);
  
  /** Checkpointing Methods*/
  void write(FILE *);
  void read(FILE *);

private:
  int depth_;
  double ub_;
  double usedCapacity_;
  double usedValue_;
  std::vector<KnapVarStatus> itemStatus_;

};

std::ostream &operator<<(std::ostream &stream, const KnapNode &node);

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

