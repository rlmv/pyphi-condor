#######################################################################
# $Id: Makefile,v 1.10 2006/07/10 17:16:18 linderot Exp $
#######################################################################

#
# Which RMComm Type --- FILES, INDEP, MPI, PVM, SOCKET
RMCommType := MPI

#
# Optimization Level (OPT or DEBUG)
#  (Will automagically set the compiler flags)
OptLevel := DEBUG

#
# What user defines
#
UserDefines := 

# Where is MW Release Installation
MWDIR ?= ../..


#######################################################################
#
# This is the OS specific portion
#
#######################################################################

CXX := g++
ifeq ($(RMCommType),MPI)
	CXX := mpicxx
endif
CXXFLAGS := -Wall
DEPCXX := g++ -MM

ifeq ($(OptLevel),OPT)
    OPTFLAG := -O6
else
    OPTFLAG := -g
endif

CXXFLAGS += $(OPTFLAG)

# Link things statically (since running on remote machines)
SYSLD += -static


#######################################################################
#
# Shouldn't have to change anything under here
#
#######################################################################

MASTERONLYSRCS := KnapMaster.C\
	KnapParam.C

MASTERMAINSRCS := KnapMasterMain.C

WORKERONLYSRCS := KnapWorker.C\
	NodeHeap.C

WORKERMAINSRCS := KnapWorkerMain.C

BOTHSRCS := KnapInstance.C\
	KnapItem.C\
	KnapNode.C\
	KnapTask.C

MPIMAINSRCS := KnapMpiMain.C


MASTERSRCS := $(MASTERONLYSRCS) $(MASTERMAINSRCS) $(BOTHSRCS)
WORKERSRCS := $(WORKERONLYSRCS) $(WORKERMAINSRCS) $(BOTHSRCS) 
INDEPSRCS := $(MASTERSRCS) $(MASTERMAINSRCS) $(WORKERONLYSRCS)
MPISRCS := $(MASTERONLYSRCS) $(WORKERONLYSRCS) $(BOTHSRCS) $(MPIMAINSRCS)
ALLSRCS :=  $(MASTERONLYSRCS) $(WORKERONLYSRCS) $(BOTHSRCS)  $(WORKERMAINSRCS)

MASTEROBJS := $(MASTERSRCS:.C=.o)
WORKEROBJS := $(WORKERSRCS:.C=.o)
INDEPOBJS := $(INDEPSRCS:.C=.o)
MPIOBJS := $(MPISRCS:.C=.o)
ALLOBJS := $(ALLSRCS:.C=.o)


#######################################################################
CXXFLAGS += $(addprefix -D,${UserDefines})
CXXMASTERFLAGS :=
CXXWORKERFLAGS :=
MASTERLIBS :=
WORKERLIBS :=


### 
# MW
###
CXXMASTERFLAGS += -I$(MWDIR)/include
CXXWORKERFLAGS += -I$(MWDIR)/include

ifeq ($(RMCommType),FILES)
	CXXMASTERFLAGS += -DFILE_COMM
	CXXWORKERFLAGS += -DFILE_COMM
	MASTERLIBS += -L$(MWDIR)/lib -lMWfilemaster -lMW
	WORKERLIBS += -L$(MWDIR)/lib -lMWfileworker -lMW
endif


ifeq ($(RMCommType),PVM)
	MASTERLIBS += -L$(MWDIR)/lib -lMW -lMWRMComm -lMWcondorpvmmaster\
		-lMWutil -lNWS -L$(PVM_ROOT)/lib/$(PVM_ARCH) -lpvm3
	WORKERLIBS += -L$(MWDIR)/lib -lMW -lMWRMComm -lMWcondorpvmworker\
		-lMWutil -lNWS -L$(PVM_ROOT)/lib/$(PVM_ARCH) -lpvm3
endif

ifeq ($(RMCommType),INDEP)
	CXXMASTERFLAGS += -DINDEPENDENT 
	CXXWORKERFLAGS += -DINDEPENDENT
	MASTERLIBS += -L$(MWDIR)/lib -lMW_indp -lMWRMComm_indp -lMWRC_indp -lMWutil_indp -lNWS
	WORKERLIBS += -L$(MWDIR)/lib -lMW_indp -lMWRMComm_indp -lMWRC_indp -lMWutil_indp -lNWS
endif

ifeq ($(RMCommType),SOCKET)
	MASTERLIBS += -L$(MWDIR)/lib -lMW -lMWRMComm -lMWsocketmaster -lMWutil -lnsl -lNWS
	WORKERLIBS += -L$(MWDIR)/lib -lMW -lMWRMComm -lMWsocketworker -lNWS -lMWutil -lMW -lMWRMComm -lnsl
endif

ifeq ($(RMCommType), MPI)
	MASTERLIBS := -L$(MWDIR)/lib -lMW -lMWRMComm -lMWRC_mpi -lMWutil
endif


#
# Rules
#

TARGET := knap-master knap-worker
ifeq ($(RMCommType), INDEP)
	TARGET := knap
endif

ifeq ($(RMCommType), MPI)
	TARGET := knap-mpi
endif



default: $(TARGET)

.PHONY: clean default all knap-master knap-worker knap depend

all: knap-master knap-worker

knap-master: $(MASTEROBJS)
	@echo Building master on Linux
	$(CXX) $(SYSLD) -o $@ $(MASTEROBJS)\
		$(LDFLAGS) $(MASTERLIBS)

knap-worker: $(WORKEROBJS)
	@echo Building worker on Linux
	$(CXX) $(SYSLD) -o $@ $(WORKEROBJS)\
		$(LDFLAGS) $(WORKERLIBS)

knap: $(INDEPOBJS)
	$(CXX) -o $@ $(INDEPOBJS) $(LDFLAGS) $(MASTERLIBS)

knap-mpi: $(MPIOBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(CXXMASTERFLAGS) $(MPISRCS) $(LDFLAGS) $(MASTERLIBS)

# A default
.C.o:
	$(CXX) $(CXXFLAGS) $(CXXMASTERFLAGS) $(CXXWORKERFLAGS) -c $< -o $@	

clean:
	\rm -f $(ALLOBJS) knap-master knap-worker knap

reallyclean: clean
	\rm -f *~

depend:
	$(DEPCXX) $(CXXFLAGS) $(CXXMASTERFLAGS) $(CXXWORKERFLAGS) $(MASTERSRCS)\
		$(WORKERSRCS) > depend.Linux

depend.Linux:
	touch depend.Linux

include depend.Linux
