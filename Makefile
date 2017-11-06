#-------------------------------------------------------------------------
# This file was automatically generated by Automake, and manually modified
# to make it simpler and cleaner. There are three sections in the file:
# 1) Macros
# 2) Recursive Rules and Suffixes (implicit rules)
# 3) Explicit Rules
#-------------------------------------------------------------------------

#-------------------------------------------------------------------------
#   Section 1) Macros
#-------------------------------------------------------------------------
top_srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include

# Compiler and tools
CXX = /usr/bin/g++
AR = @AR@
RANLIB = @RANLIB@
INSTALL = @INSTALL@
PACKAGE =
VERSION =
USE_CHIRP =
USE_MWFILE = no

# Condor and MW installation path
CONDOR_DIR = no
MW_DIR = ../mw-source
MW_LIBDIR = $(MW_DIR)/lib
MW_LIBDIR_DEBUG = $(MW_DIR)/debug_lib
MW_BASICLIB = -lMW -lMWRMComm -lMWutil -lNWS  $(SOCKET_LIB)
SOCKET_LIB =

# PVM
PVM_ROOT = no
PVM_ARCH = no
PVM_LIB = -L$(PVM_ROOT)/lib/$(PVM_ARCH) -lpvm3

# Flags
DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\"  -I.
LIBS =
CPPFLAGS =
LDFLAGS =
CXXFLAGS = -g -O2 -Wall

# Subdirectories
SUBDIRS =

# MW-Independent will be used to debug: "setenv ENABLE_MWINDEPENDENT yes" to turn it on;
# Type "master_test_indp < in_master.indp" to see if the example works without submit to condor.
ENABLE_MWINDEPENDENT = yes

ifeq ($(ENABLE_MWINDEPENDENT), yes)
  INDEPENDENT_PROGS = master_test_indp
  INDEPENDENT_INCLS = $(MW_DIR)/src/RMComm/MW-Independent
  master_test_indp_LDADD   = -L$(MW_LIBDIR) -lMW_indp -lMWRMComm_indp -lMWRC_indp -lMWutil_indp -lNWS_indp  $(SOCKET_LIB)
  master_test_indp_OBJECTS =  Driver_test-Ind.o Worker_test-Ind.o Task_test-Ind.o MasterMain_test-Ind.o
endif

# Check programs to be built, and dependent source files.
ifdef DEBUG_BUILD
  # To work with Insure, need to "setenv DEBUG_BUILD='insure'" and write/copy a good .psrc file
  DEBUG_CHECKER = $(DEBUG_BUILD)
  MW_LIBDIR = $(MW_LIBDIR_DEBUG)
  # MW-File doesn't work with Insure, so will not compile *_file if DEBUG_BUILD
  PROGRAMS = master_test_socket worker_test_socket $(INDEPENDENT_PROGS)
  INCLUDES = -I$(MWDIR)/src -I$(MW_DIR)/src/MWControlTasks -I$(MW_DIR)/src/RMComm -I$(MW_DIR)/src/RMComm/MW-CondorPVM \
		-I$(INDEPENDENT_INCLS) $(MEASURE_DEFN)
else
  PROGRAMS = \
		master_test_socket worker_test_socket $(INDEPENDENT_PROGS)
  INCLUDES = -I$(MW_DIR)/src -I$(MW_DIR)/src/MWControlTasks -I$(MW_DIR)/src/RMComm -I$(MW_DIR)/src/RMComm/MW-File \
		-I$(MW_DIR)/src/RMComm/MW-CondorPVM -I$(INDEPENDENT_INCLS) $(MEASURE_DEFN)
endif

ifeq ($(CONDOR_DIR), no)
  PROGRAMS = $(INDEPENDENT_PROGS)
else

ifneq ($(PVM_ROOT), no)
  PROGRAMS += master_test_condorpvm worker_test_condorpvm
endif

# if USE_MWFILE != no, also build mwfile
ifneq ($(USE_MWFILE), no)
PROGRAMS += master_test_file worker_test_file
endif

endif

ifdef USE_CHIRP
CONDOR_COMPILE =
else
CONDOR_COMPILE = $(CONDOR_DIR)/bin/condor_compile
endif

# Compile commands
CXXCOMPILE = $(DEBUG_CHECKER) $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(DEBUG_CHECKER) $(CXX)
CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@

# MW-File Dependencies
master_test_file_SOURCES = Driver_test.C Task_test.C MasterMain_test.C
master_test_file_LDADD   = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWfilemaster $(CONDOR_DIR)/lib/libcondorapi.a -ldl
master_test_file_OBJECTS =  Driver_test.o Task_test.o MasterMain_test.o
master_test_file_DEPENDENCIES =  $(CONDOR_DIR)/lib/libcondorapi.a
master_test_file_LDFLAGS =

worker_test_file_SOURCES = Worker_test.C Task_test.C WorkerMain_test.C
worker_test_file_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWfileworker
worker_test_file_OBJECTS =  Worker_test.o Task_test.o WorkerMain_test.o
worker_test_file_DEPENDENCIES =
worker_test_file_LDFLAGS =

# MW-CondorPvm Dependencies
master_test_condorpvm_SOURCES = Driver_test.C Task_test.C MasterMain_test.C
master_test_condorpvm_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWcondorpvmmaster $(PVM_LIB)
master_test_condorpvm_OBJECTS =  Driver_test.o Task_test.o MasterMain_test.o
master_test_condorpvm_DEPENDENCIES =
master_test_condorpvm_LDFLAGS =

worker_test_condorpvm_SOURCES = Worker_test.C Task_test.C WorkerMain_test.C
worker_test_condorpvm_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWcondorpvmworker $(PVM_LIB)
worker_test_condorpvm_OBJECTS =  Worker_test.o Task_test.o WorkerMain_test.o
worker_test_condorpvm_DEPENDENCIES =
worker_test_condorpvm_LDFLAGS =

# MW-Socket Dependencies
master_test_socket_SOURCES = Driver_test.C Task_test.C MasterMain_test.C
master_test_socket_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWsocketmaster
master_test_socket_OBJECTS =  Driver_test.o Task_test.o MasterMain_test.o
master_test_socket_DEPENDENCIES =
master_test_socket_LDFLAGS =

worker_test_socket_SOURCES = Worker_test.C Task_test.C WorkerMain_test.C
worker_test_socket_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWsocketworker
worker_test_socket_OBJECTS =  Worker_test.o Task_test.o WorkerMain_test.o
worker_test_socket_DEPENDENCIES =
worker_test_socket_LDFLAGS =

#-------------------------------------------------------------------------
#   Section 2) Explicit and Implicit Rules
#-------------------------------------------------------------------------
all: $(PROGRAMS)
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

# Rules for *-Ind.o and test_indp
Driver_test-Ind.o: Driver_test.C
	$(CXXCOMPILE) -DINDEPENDENT -o Driver_test-Ind.o -c Driver_test.C

Task_test-Ind.o: Task_test.C
	$(CXXCOMPILE) -DINDEPENDENT -o Task_test-Ind.o -c Task_test.C

Worker_test-Ind.o: Worker_test.C
	$(CXXCOMPILE) -DINDEPENDENT -o Worker_test-Ind.o -c Worker_test.C

MasterMain_test-Ind.o: MasterMain_test.C
	$(CXXCOMPILE) -DINDEPENDENT -o MasterMain_test-Ind.o -c MasterMain_test.C

master_test_indp: $(master_test_indp_OBJECTS) $(master_test_indp_DEPENDENCIES)
	@rm -f master_test_indp
	$(CXXLINK) $(master_test_indp_LDFLAGS) $(master_test_indp_OBJECTS) $(master_test_indp_LDADD) $(LIBS)

# Rules for _file
master_test_file: $(master_test_file_OBJECTS) $(master_test_file_DEPENDENCIES)
	@rm -f master_test_file
	$(CXXLINK) $(master_test_file_LDFLAGS) $(master_test_file_OBJECTS) $(master_test_file_LDADD) $(LIBS)

worker_test_file: $(worker_test_file_OBJECTS)
	$(CONDOR_COMPILE) $(CXXLINK) $(worker_test_file_LDFLAGS) $(worker_test_file_OBJECTS) \
	$(worker_test_file_LDADD) $(LIBS) $(INCLUDES)

# Rules for _condorpvm
master_test_condorpvm: $(master_test_condorpvm_OBJECTS) $(master_test_condorpvm_DEPENDENCIES)
	@rm -f master_test_condorpvm
	$(CXXLINK) $(master_test_condorpvm_LDFLAGS) $(master_test_condorpvm_OBJECTS) $(master_test_condorpvm_LDADD) $(LIBS)

worker_test_condorpvm: $(worker_test_condorpvm_OBJECTS) $(worker_test_condorpvm_DEPENDENCIES)
	@rm -f worker_test_condorpvm
	$(CXXLINK) $(worker_test_condorpvm_LDFLAGS) $(worker_test_condorpvm_OBJECTS) $(worker_test_condorpvm_LDADD) $(LIBS)

# Rules for _socket
master_test_socket: $(master_test_socket_OBJECTS) $(master_test_socket_DEPENDENCIES)
	@rm -f master_test_socket
	$(CXXLINK) $(master_test_socket_LDFLAGS) $(master_test_socket_OBJECTS) $(master_test_socket_LDADD) $(LIBS)

worker_test_socket: $(worker_test_socket_OBJECTS) $(worker_test_socket_DEPENDENCIES)
	@rm -f worker_test_socket
	$(CXXLINK) $(worker_test_socket_LDFLAGS) $(worker_test_socket_OBJECTS) $(worker_test_socket_LDADD) $(LIBS)

# Default rules
.SUFFIXES: .C .o

.C.o:
	$(CXXCOMPILE) -c $<

#-------------------------------------------------------------------------
#   Section 3) Recursive Rules: Common
#-------------------------------------------------------------------------
check:

clean:
	-rm -f $(PROGRAMS) *.o *.a core
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done
	rm -f caller.cpp

distclean:
	-rm -f Makefile *.tar *.gz
	-rm -rf .deps
	-rm -f configure config.cache config.status config.log stamp-h stamp-h[0-9]*
	-rm -f $(PROGRAMS) *.o *.a core *~
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

.PHONY: all check clean distclean cython


cython:
	python setup.py build_ext --inplace

run:
	PYTHONPATH=. python run.py < in_master.indp
