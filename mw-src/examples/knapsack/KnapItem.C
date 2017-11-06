// $Id: KnapItem.C,v 1.4 2005/09/08 17:59:07 wasu Exp $

#include "KnapItem.h"

void
KnapItem::write(FILE *fp)
{
  fprintf(fp, "%d %f %f\n", id_, size_, profit_);
  return;
}

void
KnapItem::read(FILE *fp)
{
  fscanf(fp, "%d %lf %lf", &id_, &size_, &profit_);
  return;
}

std::ostream &
operator<<(std::ostream &o, const KnapItem &item)
{
  o << "Id: " << item.id_ << " Size: " << item.size_ 
    << " Profit: " << item.profit_;
  return o;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

