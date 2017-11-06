// $Id: KnapInstance.C,v 1.8 2005/12/12 20:17:57 wasu Exp $

#include <string>

#include <MWRMComm.h>

#include "KnapInstance.h"
#include "KnapItem.h"

using namespace std;

KnapInstance::KnapInstance()
{
  integerObj_ = true;
}

KnapInstance::~KnapInstance()
{
}

bool ratioGreaterThan(const KnapItem &i1, const KnapItem &i2) 
{
  return (i1.getProfit()/i1.getSize() > i2.getProfit()/i2.getSize());
}

void 
KnapInstance::orderItems()
{
  sort(items_.begin(), items_.end(), ratioGreaterThan);
}

int
KnapInstance::MWPack(MWRMComm *r) const
{
  int stat = 0;
  r->pack(const_cast<double *> (&capacity_), 1, 1);

  int n = items_.size();
  int *ix = new int[n];
  double *a = new double[n];
  double *c = new double[n];
  int i = 0;
  for (itemIterator it = items_.begin(); it != items_.end(); ++it) {
    ix[i] = (*it).getId();
    a[i] = (*it).getSize();
    c[i++] = (*it).getProfit();
  }

  stat += r->pack(&n, 1, 1);
  stat += r->pack(ix, n, 1);
  stat += r->pack(a, n, 1);
  stat += r->pack(c, n, 1);

  int iobj = integerObj_ ? 1 : 0;
  stat += r->pack(&iobj, 1, 1);

  delete [] ix;
  delete [] a;
  delete [] c;

  return stat;
}

int
KnapInstance::MWUnpack(MWRMComm *r)
{
  int stat = 0;

  r->unpack(&capacity_, 1, 1);
  
  int n = 0;
  stat += r->unpack(&n, 1, 1);
  int *ix = new int[n];
  double *a = new double[n];
  double *c = new double[n];
  stat += r->unpack(ix, n, 1);
  stat += r->unpack(a, n, 1);
  stat += r->unpack(c, n, 1);
  for (int i = 0; i < n; i++) {
    items_.push_back(KnapItem(ix[i], a[i], c[i]));
  }

  int iobj = 0;
  stat += r->unpack(&iobj, 1, 1);
  integerObj_ = (iobj == 1);
  
  delete [] ix;
  delete [] a;
  delete [] c;

  return stat;
}

void
KnapInstance::write(FILE *fp)
{
  fprintf(fp, "%d\n", items_.size());
  for (unsigned int i = 0; i < items_.size(); i++) {
    items_[i].write(fp);
  }
  int iio = (int) integerObj_;
  fprintf(fp, "%f %d\n", capacity_, iio);
  return;
}

void
KnapInstance::read(FILE *fp)
{
  int sz;
  fscanf(fp, "%d", &sz);
  items_.resize(sz);
  for (int i = 0; i < sz; i++) {
    items_[i].read(fp);
  }
  int iio;
  fscanf(fp, "%lf %d", &capacity_, &iio);
  integerObj_ = (iio == 0) ? false : true;
  return;
}

istream &
operator>>(istream &stream, KnapInstance &instance)
{
  string key;
  int id;
  double a, b, c;

  while (stream >> key) {
    if (key == "CAPACITY") {
      stream >> b;
      instance.capacity_ = b;
    } 
    else if  (key == "ITEM") {
      stream >> id;
      stream >> a;
      stream >> c;
      //if (c is not integer) integerObj_ = false;
      instance.items_.push_back(KnapItem(id, a, c));
    }
  }
  return stream;
}

ostream &
operator<<(ostream &stream, KnapInstance &instance)
{
  stream << "Capacity: " << instance.capacity_ << endl;

  for (KnapInstance::itemIterator it = instance.itemsBegin();
       it != instance.itemsEnd(); ++it) {
    stream << (*it) << endl;
  }
  return stream;
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

