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
# Type "master_indp < in_master.indp" to see if the example works without submit to condor.
ENABLE_MWINDEPENDENT = yes

ifeq ($(ENABLE_MWINDEPENDENT), yes)
  INDEPENDENT_PROGS = master_indp
  INDEPENDENT_INCLS = $(MW_DIR)/src/RMComm/MW-Independent
  master_indp_LDADD   = -L$(MW_LIBDIR) -lMW_indp -lMWRMComm_indp -lMWRC_indp -lMWutil_indp -lNWS_indp  $(SOCKET_LIB)
  master_indp_OBJECTS =  driver_ind.o worker_ind.o task_ind.o master_main_ind.o
endif

# Check programs to be built, and dependent source files.
ifdef DEBUG_BUILD
  # To work with Insure, need to "setenv DEBUG_BUILD='insure'" and write/copy a good .psrc file
  DEBUG_CHECKER = $(DEBUG_BUILD)
  MW_LIBDIR = $(MW_LIBDIR_DEBUG)
  # MW-File doesn't work with Insure, so will not compile *_file if DEBUG_BUILD
  PROGRAMS = master_socket worker_socket $(INDEPENDENT_PROGS)
  INCLUDES = -I$(MWDIR)/src -I$(MW_DIR)/src/MWControlTasks -I$(MW_DIR)/src/RMComm -I$(MW_DIR)/src/RMComm/MW-CondorPVM \
		-I$(INDEPENDENT_INCLS) $(MEASURE_DEFN)
else
  PROGRAMS = \
		master_socket worker_socket $(INDEPENDENT_PROGS)
  INCLUDES = -I$(MW_DIR)/src -I$(MW_DIR)/src/MWControlTasks -I$(MW_DIR)/src/RMComm -I$(MW_DIR)/src/RMComm/MW-File \
		-I$(MW_DIR)/src/RMComm/MW-CondorPVM -I$(INDEPENDENT_INCLS) $(MEASURE_DEFN)
endif

ifeq ($(CONDOR_DIR), no)
  PROGRAMS = $(INDEPENDENT_PROGS)
else

ifneq ($(PVM_ROOT), no)
  PROGRAMS += master_condorpvm worker_condorpvm
endif

# if USE_MWFILE != no, also build mwfile
ifneq ($(USE_MWFILE), no)
PROGRAMS += master_file worker_file
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
master_file_SOURCES = driver.cpp task.cpp master_main.cpp
master_file_LDADD   = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWfilemaster $(CONDOR_DIR)/lib/libcondorapi.a -ldl
master_file_OBJECTS =  driver.o task.o master_main.o
master_file_DEPENDENCIES =  $(CONDOR_DIR)/lib/libcondorapi.a
master_file_LDFLAGS =

worker_file_SOURCES = worker.cpp task.cpp worker_main.cpp
worker_file_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWfileworker
worker_file_OBJECTS =  worker.o task.o worker_main.o
worker_file_DEPENDENCIES =
worker_file_LDFLAGS =

# MW-CondorPvm Dependencies
master_condorpvm_SOURCES = driver.cpp task.cpp master_main.cpp
master_condorpvm_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWcondorpvmmaster $(PVM_LIB)
master_condorpvm_OBJECTS =  driver.o task.o master_main.o
master_condorpvm_DEPENDENCIES =
master_condorpvm_LDFLAGS =

worker_condorpvm_SOURCES = worker.cpp task.cpp worker_main.cpp
worker_condorpvm_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWcondorpvmworker $(PVM_LIB)
worker_condorpvm_OBJECTS =  worker.o task.o worker_main.o
worker_condorpvm_DEPENDENCIES =
worker_condorpvm_LDFLAGS =

# MW-Socket Dependencies
master_socket_SOURCES = driver.cpp task.cpp master_main.cpp
master_socket_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWsocketmaster
master_socket_OBJECTS =  driver.o task.o master_main.o
master_socket_DEPENDENCIES =
master_socket_LDFLAGS =

worker_socket_SOURCES = worker.cpp task.cpp worker_main.cpp
worker_socket_LDADD = -L$(MW_LIBDIR) $(MW_BASICLIB) -lMWsocketworker
worker_socket_OBJECTS =  worker.o task.o worker_main.o
worker_socket_DEPENDENCIES =
worker_socket_LDFLAGS =

#-------------------------------------------------------------------------
#   Section 2) Explicit and Implicit Rules
#-------------------------------------------------------------------------
all: $(PROGRAMS)
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

# Rules for *-Ind.o and test_indp
driver_ind.o: driver.cpp
	$(CXXCOMPILE) -DINDEPENDENT -o driver_ind.o -c driver.cpp

task_ind.o: task.cpp
	$(CXXCOMPILE) -DINDEPENDENT -o task_ind.o -c task.cpp

worker_ind.o: worker.cpp
	$(CXXCOMPILE) -DINDEPENDENT -o worker_ind.o -c worker.cpp

master_main_ind.o: master_main.cpp
	$(CXXCOMPILE) -DINDEPENDENT -o master_main_ind.o -c master_main.cpp

master_indp: $(master_indp_OBJECTS) $(master_indp_DEPENDENCIES)
	@rm -f master_indp
	$(CXXLINK) $(master_indp_LDFLAGS) $(master_indp_OBJECTS) $(master_indp_LDADD) $(LIBS)

# Rules for _file
master_file: $(master_file_OBJECTS) $(master_file_DEPENDENCIES)
	@rm -f master_file
	$(CXXLINK) $(master_file_LDFLAGS) $(master_file_OBJECTS) $(master_file_LDADD) $(LIBS)

worker_file: $(worker_file_OBJECTS)
	$(CONDOR_COMPILE) $(CXXLINK) $(worker_file_LDFLAGS) $(worker_file_OBJECTS) \
	$(worker_file_LDADD) $(LIBS) $(INCLUDES)

# Rules for _condorpvm
master_condorpvm: $(master_condorpvm_OBJECTS) $(master_condorpvm_DEPENDENCIES)
	@rm -f master_condorpvm
	$(CXXLINK) $(master_condorpvm_LDFLAGS) $(master_condorpvm_OBJECTS) $(master_condorpvm_LDADD) $(LIBS)

worker_condorpvm: $(worker_condorpvm_OBJECTS) $(worker_condorpvm_DEPENDENCIES)
	@rm -f worker_condorpvm
	$(CXXLINK) $(worker_condorpvm_LDFLAGS) $(worker_condorpvm_OBJECTS) $(worker_condorpvm_LDADD) $(LIBS)

# Rules for _socket
master_socket: $(master_socket_OBJECTS) $(master_socket_DEPENDENCIES)
	@rm -f master_socket
	$(CXXLINK) $(master_socket_LDFLAGS) $(master_socket_OBJECTS) $(master_socket_LDADD) $(LIBS)

worker_socket: $(worker_socket_OBJECTS) $(worker_socket_DEPENDENCIES)
	@rm -f worker_socket
	$(CXXLINK) $(worker_socket_LDFLAGS) $(worker_socket_OBJECTS) $(worker_socket_LDADD) $(LIBS)

# Default rules
.SUFFIXES: .cpp .o

.cpp.o:
	$(CXXCOMPILE) -c $<

#-------------------------------------------------------------------------
#   Section 3) Recursive Rules: Common
#-------------------------------------------------------------------------
check:

clean:
	-rm -f $(PROGRAMS) *.o *.a core checkpoint
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done
	rm -f caller.cpp caller.cpython-36m-darwin.so

distclean:
	-rm -f Makefile *.tar *.gz
	-rm -rf .deps
	-rm -f configure config.cache config.status config.log stamp-h stamp-h[0-9]*
	-rm -f $(PROGRAMS) *.o *.a core *~
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

.PHONY: all check clean distclean cython

cython: clean
	python setup.py build_ext --inplace

run:
	PYTHONPATH=. python run.py

test: cython run
