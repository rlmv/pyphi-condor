/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/
/* These methods will be implemented to reflect the application behavior */

#include "MW.h"
#include "Driver_MYAPP.h"
#include "Worker_MYAPP.h"
#include "Task_MYAPP.h"
#include <unistd.h>

/* initialization */
Driver_MYAPP::Driver_MYAPP() 
{
	/* For statically generated tasks, you can decide how many tasks
	 * you want to use based on the input information. However, you can 
	 * also dynamically generate tasks (either when init, or when 
	 * acr_on_completed_task, and add them to TODO queue by pushTask. */
	num_tasks = 0;
	
	/* The list of tasks you will generate */
	job = NULL;
}

/* destruction */
Driver_MYAPP::~Driver_MYAPP() 
{
	/* release the memory allocated for tasks */
	if (job) 
		delete [] job;
}

/* Here the application can (1) get per-run information from stdin (the input 
 * file redirected by Condor), or (2) get configuration info from user-defined
 * config file; or (3) just hard-code your config here. The app should tell MW
 * (or RMC the Resource Manager and Communicator, which is a class member) 
 * some basic info by calling the functions below: 
 * (a) RMC->set_num_exec_classes()      num of different processes you have
 * (b) RMC->set_num_arch_classes() 	num of different platforms you have 
 * (c) RMC->set_arch_class_attributes()  	for each platform 
 * (d) RMC->add_executable() 	for combination of exec_class, arch_class;
 * (e) set_checkpoint_freqency(), ... and other information.  */

MWReturn Driver_MYAPP::get_userinfo( int argc, char *argv[] ) 
{
	int i, j;
	int num_exec = 0;
	int num_arch = 0;
	char exec[_POSIX_PATH_MAX];
	
	MWprintf(30, "Enter Driver_MYAPP::get_userinfo\n");
	for ( i=0; i<argc; i++ ) {
		MWprintf( 70, "arg %d: %s\n", i, argv[i] );
	}

	/* exec classes */
	RMC->set_num_exec_classes(1);
	
	/* arch classes */
	scanf ("%d", &num_arch);
	RMC->set_num_arch_classes(num_arch);
	MWprintf( 10, "Set the arch class to %d.\n", num_arch);
	
	/* Should have a better way to read attributes and set_arch_class_attributes */
	for ( i=0; i<num_arch; i++) {
		if (i==0)
			RMC->set_arch_class_attributes (0, "((Arch==\"INTEL\") && (Opsys==\"LINUX\"))");
		else RMC->set_arch_class_attributes (1, "((Arch==\"INTEL\") && (Opsys==\"SOLARIS26\"))");
	}

	/* executables */	
	scanf ("%d", &num_exec);
	RMC->set_num_executables(num_exec);
	for ( i=0; i<num_exec; i++) {
		scanf("%s %d", exec, &j);
		MWprintf( 30, " %s\n", exec);
		RMC->add_executable(0, j, exec, ""); 
	}

	/* checkpoint requirement */
	set_checkpoint_frequency (10);
	
	/* Now there are application specific configurations. 
	 * Please replace them with the application logic !! */
	scanf( "%d", &job_size);
	scanf( "%d", &task_size);
	if (job_size == 0) {
		MWprintf(10, "The job size is 0, so I quit\n");
		return QUIT;
	}
	job = new int[job_size];
	for ( i=0; i<job_size; i++) 
		scanf( "%d ", &job[i]);
	largest = job[0];

	remain = job_size % task_size;
	num_tasks = remain ? (job_size/task_size + 1) : job_size/task_size ; 
	RMC->set_target_num_workers(num_tasks);
	MWprintf(30, "Patitioned into %d tasks\n", num_tasks);
	
	MWprintf(30, "Leave Driver_MYAPP::get_userinfo\n");
	return OK;
}

/* setup (generate and push) the first batch of tasks in the beginning */
MWReturn Driver_MYAPP::setup_initial_tasks(int *n_init , MWTask ***init_tasks) 
{
	int i;
	int head_pos;
	
	*n_init = num_tasks;
    	*init_tasks = new MWTask *[num_tasks];

	head_pos = 0;

	for ( i=0; i<num_tasks-1; i++) {
    		(*init_tasks)[i] = new Task_MYAPP(task_size, &(job[head_pos]));
		head_pos += task_size;
	}

	if (remain)
		(*init_tasks)[i] = new Task_MYAPP(remain, &(job[head_pos]));
	else (*init_tasks)[i] = new Task_MYAPP(task_size, &(job[head_pos]));

	return OK;
}

/* Implement application behavior to process a just completed task */
MWReturn Driver_MYAPP::act_on_completed_task( MWTask *t ) 
{
#ifdef NO_DYN_CAST
	Task_MYAPP *tf = (Task_MYAPP*) t;
#else
	Task_MYAPP *tf = dynamic_cast<Task_MYAPP *> (t);
#endif
	
	if ( tf->largest > this->largest)
		this->largest = tf->largest;
	
	MWprintf(30, "Driver_MYAPP::act_on_completed_task: current largest = %d\n", this->largest);
	return OK;
}

/* The first batch of data for a newly spawned worker, e.g. init data */
MWReturn Driver_MYAPP::pack_worker_init_data( void ) 
{
	/* Nothing for this application */
	return OK;
}

/* Print out the result when MW is done. MW assume that the application 
 * is keeping track of the results :-) */
void Driver_MYAPP::printresults() 
{
	MWprintf ( 10, "The largest number is %d.\n", this->largest);
}

/* Write app-specific master checkpoint info */
void
Driver_MYAPP::write_master_state( FILE *fp ) 
{
	/* Nothing to be written */
}

/* Read app-specific master checkpoint info */
void 
Driver_MYAPP::read_master_state( FILE *fp ) 
{
	/* Nothing to be read */
}

/* Return a new application task object */
MWTask*
Driver_MYAPP::gimme_a_task() 
{
    	return new Task_MYAPP;
}

/* Return a new driver object */
MWDriver* gimme_the_master() 
{
	return new Driver_MYAPP;
}
