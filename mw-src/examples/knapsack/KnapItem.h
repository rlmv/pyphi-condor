// $Id: KnapItem.h,v 1.5 2005/12/12 20:17:57 wasu Exp $ 

#ifndef KNAP_ITEM_H
#define KNAP_ITEM_H

#include <iostream>

class KnapItem
{

public:

  friend std::ostream &operator<<(std::ostream &stream, const KnapItem &item);

  KnapItem() : id_(-1), size_(0.0), profit_(0.0) {}
  KnapItem(int id, double size, double profit) : id_(id), size_(size), profit_(profit) {}
  virtual ~KnapItem() {}

  int getId() const { return id_; }
  double getSize() const { return size_; }
  double getProfit() const { return profit_; }

  /** Checkpointing Methods*/
  void write(FILE *);
  void read(FILE *);

private:

  int id_;
  double size_;
  double profit_;

};

std::ostream &operator<<(std::ostream &stream, const KnapItem &item);

#endif

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

