// $Id: KnapMaster.C,v 1.21 2006/07/10 17:23:24 linderot Exp $

#include <cassert>
#include <cfloat>
#include <iostream>
#include <fstream>

#include "KnapMaster.h"
#include "KnapTask.h"

using namespace std;

MWKey upper_bound(MWTask *t)
{
  KnapTask *kt = dynamic_cast<KnapTask *> (t);
  assert(kt);
  return ((MWKey) (kt->getInputNode().getUpperBound()));
}

MWKey neg_upper_bound(MWTask *t)
{
  return (-upper_bound(t));
}

MWKey depth(MWTask *t)
{
  KnapTask *kt = dynamic_cast<KnapTask *> (t);
  assert(kt);
  return ((MWKey) (kt->getInputNode().getDepth()));
}

MWKey neg_depth(MWTask *t)
{
  return (-depth(t));
}

KnapMaster::KnapMaster()
{
  bestLowerBound_ = -DBL_MAX;
  numNodesEvaluated_ = 0.0;
  masterNodeOrder_ = NodeHeap::VALUE;
  maxNodes_ = 10000;
  maxTime_ = 10;
}

KnapMaster::~KnapMaster()
{
}

MWReturn 
KnapMaster::act_on_completed_task(MWTask *t)
{
  
  MWReturn ustat = OK;

  MWprintf(10, "It took %f CPU %f Wall\n", t->cpu_time, t->working_time);

  KnapTask *kt = dynamic_cast<KnapTask *> (t);
  assert(kt);

  // Record number of nodes that worker did
  numNodesEvaluated_ += (double) kt->getNumNodesEvaluated();

  // Fathom nodes of the branch and bound tree
  if (kt->foundImprovedSolution()) {
    double blb = kt->getSolutionValue();
    if (blb > bestLowerBound_) {
      MWprintf(10, "Improved Solution Found.  Fathoming all nodes with UB < %f\n", blb);
      print_task_keys();
      bestLowerBound_ = blb;
      NodeHeap::Type oldMasterNodeOrder = masterNodeOrder_;
      setMasterNodeOrder(NodeHeap::VALUE);
      delete_tasks_worse_than(-bestLowerBound_);
      setMasterNodeOrder(oldMasterNodeOrder);      
    }
  }
  
  // Add new nodes/tasks
  for (vector<KnapNode *>::const_iterator it = kt->constNewNodeBegin(); 
       it != kt->constNewNodeEnd(); ++it) {
    if ((*it)->getUpperBound() > bestLowerBound_) {
      addTask(new KnapTask(**it));
    }
    delete *it;
  }

  // Set maxNodes_ and maxTime_ for pack_driver_task_data()
  int ntasks_left = get_number_tasks();
  maxTime_ = ntasks_left <= param_.getBegEndNodeLevel() ? 
    param_.getBegEndCpuTimeWorker() : param_.getMaxCpuTimeWorker();
  maxNodes_ = param_.getMaxNodeWorker();
    
  // Update the users with important information
  MWprintf(10, "Tasks in Master Queue: %d\n", ntasks_left);
  double bub = getBestUpperBound();
  MWprintf(10, "Best UB: %f Best LB: %f\n", bub, bestLowerBound_);

  // Now switch between worst and best
  if (ntasks_left > param_.getNodeHighWater() && masterNodeOrder_ != NodeHeap::WORST) {
    MWprintf(10, "Number of tasks (%d) exceeds high water mark (%d).  Switching to worst bound\n");
    setMasterNodeOrder(NodeHeap::WORST, true);
  }
  else if (ntasks_left < param_.getNodeLowWater() && masterNodeOrder_ != NodeHeap::VALUE) {
    MWprintf(10, "Number of tasks (%d) smaller than low water mark (%d).  Switching to best bound\n");
    setMasterNodeOrder(NodeHeap::VALUE, true);
  }

  return(ustat);
}

MWReturn 
KnapMaster::get_userinfo(int argc, char *argv[])
{
  MWReturn ustat = OK;

  // If running with mpirun, args will be bigger, so don't do this...
  if (argc < 2) { // || argc > 3) {
    cerr << "Usage: knap <datafile> (optional) <paramfile>" << endl;
    return(ABORT);
  }  
  
  // Read in the knapsack data file
  ifstream data_file(argv[1]);
  if (!data_file) {
    cerr << "Error opening input data file: " << argv[1] 
         << "Aborting" << endl;
    return ABORT;
  }

  try {
    data_file >> instance_;
  }
  catch (...) {
    cerr << "Error in data file: " << argv[1] << "Aborting" << endl;
    return ABORT;
  }
  instance_.orderItems();
  cout << instance_;

  if (argc == 3) {
    ifstream param_file(argv[2]);
    try { 
      param_file >> param_;
    }
    catch (...) {
      cerr << "Error in parameter file: " << argv[2] << "Aborting" << endl;
      return ABORT;
    }
    
  }

  RMC->add_executable("knap-worker-x86_64", "Arch == \"x86_64\" && Opsys == \"LINUX\"");
  RMC->add_executable("knap-worker-x86_32", "Arch == \"INTEL\" && Opsys == \"LINUX\"");

  //RMC->add_executable("knap-worker", "Arch == \"INTEL\" && Opsys == \"LINUX\"");

  RMC->set_target_num_workers(param_.getTargetNumWorkers());

  // Set up handling of the task list
  setMasterNodeOrder(param_.getMasterNodeOrder());
  set_task_add_mode(ADD_BY_KEY);
  set_task_retrieve_mode(GET_FROM_BEGIN);

  // Register benchmark task. Do 10000 nodes of the instance.
  register_benchmark_task(new KnapTask(KnapNode(instance_.getNumItems()), 10000));

  // Tell MW about the checkpoint frequency
  set_checkpoint_frequency(0);
  set_checkpoint_time(param_.getCheckpointFrequency());

  return(ustat);
}

MWReturn
KnapMaster::pack_worker_init_data()
{
   MWReturn ustat = OK;
   instance_.MWPack(RMC);

   int stat = 0;
   int maxn = param_.getMaxNodeWorker();
   stat += RMC->pack(&maxn, 1);
   double maxt = param_.getMaxCpuTimeWorker();
   stat += RMC->pack(&maxt, 1);
   int wno = (int) param_.getWorkerNodeOrder();
   stat += RMC->pack(&wno, 1);
   double ct = param_.getCleanupCpuTimeWorker();
   stat += RMC->pack(&ct, 1);
   double cf = param_.getCleanupFactorWorker();
   stat += RMC->pack(&cf, 1);
   
   if (stat > 0) 
     ustat = ABORT;

   return(ustat);
}

void
KnapMaster::setMasterNodeOrder(NodeHeap::Type type, bool sort_it)
{

  masterNodeOrder_ = type;
  switch(type) {
  case NodeHeap::VALUE:
    set_task_key_function(neg_upper_bound);
    break;
  case NodeHeap::WORST:
    set_task_key_function(upper_bound);
    break;
  case NodeHeap::DEPTH:
    set_task_key_function(neg_depth);
    break;  
  default:
    assert(0);
  }

  if(sort_it) sort_task_list();
}

MWTask *
KnapMaster::gimme_a_task()
{
   return (new KnapTask());
}

void 
KnapMaster::printresults()
{
  MWprintf(1, "--------------------------------------------------\n");
  MWprintf(1, "BEST SOLUTION VALUE: %f\n", bestLowerBound_);
  MWprintf(1, "TOTAL NUMBER OF NODES: %f\n", numNodesEvaluated_);
  MWprintf(1, "(Master) bytes packed: %lf\n", RMC->get_bytes_packed());
  MWprintf(1, "(Master) bytes unpacked: %lf\n", RMC->get_bytes_unpacked());
  MWprintf(1, "--------------------------------------------------\n\n");
}

void 
KnapMaster::write_master_state(FILE *fp)
{
  instance_.write(fp);
  param_.write(fp);
  fprintf(fp, "%f\n", numNodesEvaluated_);
  int itype = (int) masterNodeOrder_;
  fprintf(fp, "%d\n", itype);
  fprintf(fp, "%f\n", bestLowerBound_);
  fprintf(fp, "%d\n", maxNodes_);
  fprintf(fp, "%f\n", maxTime_);
  return;
}

void 
KnapMaster::read_master_state(FILE *fp)
{
  instance_.read(fp);
  param_.read(fp);
  fscanf(fp, "%lf", &numNodesEvaluated_);
  int itype;
  fscanf(fp, "%d", &itype);
  masterNodeOrder_ = (enum NodeHeap::Type) itype;  
  setMasterNodeOrder(masterNodeOrder_);
  fscanf(fp, "%lf", &bestLowerBound_);
  fscanf(fp, "%d", &maxNodes_);
  fscanf(fp, "%lf", &maxTime_);
  return;
}

MWReturn 
KnapMaster::setup_initial_tasks(int *n_init, MWTask ***init_tasks)
{
  MWReturn ustat = OK;

  *n_init = 1;   
  *init_tasks = new MWTask * [*n_init];

#if defined(INDEPENDENT)
  (*init_tasks)[0] = new KnapTask(KnapNode(instance_.getNumItems()));
#else
  (*init_tasks)[0] = new KnapTask(KnapNode(instance_.getNumItems()));
#endif
   return(ustat);
}

double
KnapMaster::getBestUpperBound()
{
  double bub = DBL_MAX;
  NodeHeap::Type oldMasterNodeOrder = masterNodeOrder_;
  setMasterNodeOrder(NodeHeap::VALUE);
  bub = std::min(return_best_todo_keyval(), return_best_running_keyval());
  setMasterNodeOrder(oldMasterNodeOrder);
  return -bub;
}

void
KnapMaster::pack_driver_task_data()
{ 
  RMC->pack(&bestLowerBound_, 1);
  RMC->pack(&maxNodes_, 1);
  RMC->pack(&maxTime_, 1);
}

// Local Variables:
// mode: c++
// eval: (c-set-style "gnu")
// eval: (setq indent-tabs-mode nil)
// End:

