//$Id: KnapParam.C,v 1.10 2005/09/08 17:59:07 wasu Exp $

#include <cfloat>
#include <iostream>
#include <string>
#include "KnapParam.h"

using namespace std;

KnapParam::KnapParam()
{
  masterNodeOrder_ = NodeHeap::VALUE;
  maxCpuTimeWorker_ = 100.0;
  maxNodeWorker_ = INT_MAX;
  targetNumWorkers_ = 64;
  workerNodeOrder_ = NodeHeap::DEPTH;

  begEndCpuTimeWorker_ = 5.0;
  begEndNodeLevel_ = 250;

  nodeLowWater_ = 2000;
  nodeHighWater_ = 8000;

  cleanupCpuTimeWorker_ = 0.0;
  cleanupFactorWorker_ = DBL_MAX;

  checkpointFrequency_ = 0;
}

static NodeHeap::Type
StringToType(string &is)
{

  NodeHeap::Type t = NodeHeap::VALUE;

  if (is == string("VALUE")) {
    t = NodeHeap::VALUE;
  }
  else if (is == string("DEPTH")) {
    t = NodeHeap::DEPTH;
  }
  else if (is == string("WORST")) {
    t = NodeHeap::WORST;
  }
  return t;
}

void
KnapParam::write(FILE *fp)
{
  int itype1 = (int) masterNodeOrder_;
  fprintf(fp, "%d\n", itype1);
  fprintf(fp, "%f %d %d\n",
          maxCpuTimeWorker_, maxNodeWorker_, targetNumWorkers_);
  int itype2 = (int) workerNodeOrder_;
  fprintf(fp, "%d \n", itype2);
  fprintf(fp, "%f %d %d %d %f %f %d\n",
          begEndCpuTimeWorker_, begEndNodeLevel_, nodeLowWater_, 
          nodeHighWater_, cleanupCpuTimeWorker_, cleanupFactorWorker_, 
          checkpointFrequency_);
  return;
}

void
KnapParam::read(FILE *fp)
{
  int itype1;
  fscanf(fp, "%d", &itype1);
  masterNodeOrder_ = (enum NodeHeap::Type) itype1;
  fscanf(fp, "%lf %d %d",
          &maxCpuTimeWorker_, &maxNodeWorker_, &targetNumWorkers_);
  int itype2;
  fscanf(fp, "%d", &itype2);
  workerNodeOrder_ = (enum NodeHeap::Type) itype2;
  fscanf(fp, "%lf %d %d %d %lf %lf %d",
         &begEndCpuTimeWorker_, &begEndNodeLevel_, &nodeLowWater_, 
         &nodeHighWater_, &cleanupCpuTimeWorker_, &cleanupFactorWorker_, 
         &checkpointFrequency_);
  return;
}

istream &
operator>>(istream &stream, KnapParam &p)
{
  string key;
  while(stream >> key) {
    cout << key << " ";

    if (key == string("masterNodeOrder")) {
      string val;
      stream >> val;
      cout << val << endl;
      p.masterNodeOrder_ = StringToType(val);	
    }
    else if (key == string("maxCpuTimeWorker")) {
      double d;
      stream >> d;
      if (d < 0.0) {
        d = DBL_MAX;
      }
      cout << d << endl;
      p.maxCpuTimeWorker_ = d;
    }
    else if (key == string("maxNodeWorker")) {
      int n;
      stream >> n;
      if (n < 0) {
        n = INT_MAX;
      }
      cout << n << endl;
      p.maxNodeWorker_ = n; 
    }
    else if (key == string("targetNumWorkers")) {
      int n;
      stream >> n;
      cout << n << endl;
      p.targetNumWorkers_ = n;
    }
    else if (key == string("workerNodeOrder")) {
      string val;
      stream >> val;
      cout << val << endl;
      p.workerNodeOrder_ = StringToType(val);	
    }
    else if (key == string("begEndCpuTimeWorker")) {
      double d;
      stream >> d;
      if (d < 0.0) {
        d = DBL_MAX;
      }
      cout << d << endl;
      p.begEndCpuTimeWorker_ = d;
    }
    else if (key == string("begEndNodeLevel")) {
      int n;
      stream >> n;
      cout << n << endl;
      p.begEndNodeLevel_ = n;
    }
    else if (key == string("nodeLowWater")) {
      int n;
      stream >> n;
      cout << n << endl;
      p.nodeLowWater_ = n;
    }
    else if (key == string("nodeHighWater")) {
      int n;
      stream >> n;
      if (n < 0) {
        n = INT_MAX;
      }
      cout << n << endl;
      p.nodeHighWater_ = n;
    }
    else if (key == string("cleanupCpuTimeWorker")) {
      double d;
      stream >> d;
      cout << d << endl;
      p.cleanupCpuTimeWorker_ = d;
    }
    else if (key == string("cleanupFactorWorker")) {
      double d;
      stream >> d;
      cout << d << endl;
      p.cleanupFactorWorker_ = d;
    }
    else if (key == string("checkpointFrequency")) {
      int n;
      stream >> n;
      cout << n << endl;
      p.checkpointFrequency_ = n;
    }
    else {
      cerr << "Unknown key: " << key << " in parameter file" << endl;
      string s;
      stream >> s;
    }
  }
  return stream;
}


// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:
