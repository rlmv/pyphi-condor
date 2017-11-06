// $Id: KnapInstance.h,v 1.6 2005/12/12 20:17:57 wasu Exp $

#ifndef KNAP_INSTANCE_H
#define KNAP_INSTANCE_H

#include <iostream>
#include <vector>

#include "KnapItem.h"

class MWRMComm;

class KnapInstance
{

public:

  typedef std::vector<KnapItem>::const_iterator itemIterator;  

  friend std::istream &operator>>(std::istream &stream, KnapInstance &instance);
  friend std::ostream &operator<<(std::ostream &stream, KnapInstance &instance);

  KnapInstance();
  virtual ~KnapInstance();

  double getCapacity() const { return capacity_; }
  const KnapItem &getItem(int ix) const { return items_[ix]; }
  int getNumItems() const { return items_.size(); }

  bool isIntegerObj() const { return integerObj_; }

  itemIterator itemsBegin() const { return items_.begin(); }
  itemIterator itemsEnd() const { return items_.end(); }

  void orderItems();

  int MWPack(MWRMComm *r) const;
  int MWUnpack(MWRMComm *r);

  /** Checkpointing Methods*/                                                                        
  void write(FILE *);
  void read(FILE *);

private:

  std::vector<KnapItem> items_;
  double capacity_;
  bool integerObj_;

};

std::istream &operator>>(std::istream &stream, KnapInstance &instance);
std::ostream &operator<<(std::ostream &stream, KnapInstance &instance);

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

