// $Id: KnapNode.C,v 1.10 2005/12/12 20:17:57 wasu Exp $

#include <cfloat>

#include <MWRMComm.h>

#include "KnapNode.h"

using namespace std;

KnapNode::KnapNode()
{
  depth_ = 0;
  ub_ = DBL_MAX;
  usedCapacity_ = 0.0;
  usedValue_ = 0.0;
}

KnapNode::KnapNode(int n)
{
  for(int i = 0; i < n; i++) {
    itemStatus_.push_back(Free);
  }
  depth_ = 0;
  ub_ = DBL_MAX;
  usedCapacity_ = 0;
  usedValue_ = 0;
}

KnapNode::KnapNode(const KnapNode &rhs) : 
  depth_(rhs.depth_), ub_(rhs.ub_), usedCapacity_(rhs.usedCapacity_), 
  usedValue_(rhs.usedValue_), itemStatus_(rhs.itemStatus_)
{  
}

KnapNode::KnapNode(const KnapNode &rhs, int ix, KnapVarStatus fv, 
                   double ixSize, double ixProfit) : 
  depth_(rhs.depth_+1), ub_(rhs.ub_), usedCapacity_(rhs.usedCapacity_), 
  usedValue_(rhs.usedValue_), itemStatus_(rhs.itemStatus_)
{  
  itemStatus_[ix] = fv;
  if (fv == FixedToOne) {
    usedCapacity_ += ixSize;
    usedValue_ += ixProfit;
  }
}

int
KnapNode::MWPack(MWRMComm *r)
{
  int stat = 0;
  int n = itemStatus_.size();
  stat += r->pack(&n, 1, 1);
  int *is = reinterpret_cast<int *> (&itemStatus_[0]);
  stat += r->pack(is, n, 1);
  stat += r->pack(&depth_, 1, 1);
  stat += r->pack(&ub_, 1, 1);
  stat += r->pack(&usedCapacity_, 1, 1);
  stat += r->pack(&usedValue_, 1, 1);
  
  return stat;
}

int
KnapNode::MWUnpack(MWRMComm *r)
{
  int stat = 0;
  int n = 0;
  stat += r->unpack(&n, 1, 1);
  itemStatus_.resize(n);
  int *is = reinterpret_cast<int *> (&itemStatus_[0]);
  stat += r->unpack(is, n, 1);
  stat += r->unpack(&depth_, 1, 1);
  stat += r->unpack(&ub_, 1, 1);
  stat += r->unpack(&usedCapacity_, 1, 1);
  stat += r->unpack(&usedValue_, 1, 1);

  return stat;
}

void
KnapNode::write(FILE *fp)
{
  fprintf(fp, "%d %f %f %f %d\n",
          depth_, ub_, usedCapacity_, usedValue_, itemStatus_.size());
  for (unsigned int i = 0; i < itemStatus_.size(); i++) {
    int itype = (int) itemStatus_[i];
    fprintf(fp, "%d\n", itype);
  }
  return;
}

void
KnapNode::read(FILE *fp)
{
  int sz;
  fscanf(fp, "%d %lf %lf %lf %d",
         &depth_, &ub_, &usedCapacity_, &usedValue_, &sz);
  itemStatus_.resize(sz);
  for (int i = 0; i < sz; i++) {
    int itype;
    fscanf(fp, "%d", &itype);
    itemStatus_[i] = (enum KnapVarStatus) itype;
  }
  return;
}

ostream &
operator<<(ostream &stream, const KnapNode &node)
{
  cout << "Depth: " << node.depth_ << endl;
  cout << "UB: " << node.ub_ << endl;
  cout << "Used Capacity: " << node.usedCapacity_ << endl;
  cout << "Used Value: " << node.usedValue_;
  for (unsigned int i = 0; i < node.itemStatus_.size(); i++) {
    if (i%25 == 0) cout << endl;
    cout << (node.itemStatus_[i] == Free ? "F " : 
             (node.itemStatus_[i] == FixedToOne ? "1 " : "0 "));
  }
  return stream;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

