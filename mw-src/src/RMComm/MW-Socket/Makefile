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
top_srcdir = ../../..
prefix = /Users/robertmarchman/IIT/chtc/mw
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include

CONDOR_DIR = no
CXX = /usr/bin/g++
MEASURE_DEFN = 
MISC_DEFN = 
MISC_LIB = -lNWS
MW_LIBDIR = /Users/robertmarchman/IIT/chtc/mw-source/lib
MW_LIBDIR_DEBUG = /Users/robertmarchman/IIT/chtc/mw-source/debug_lib
PVM_ROOT = no
RANLIB = ranlib
INSTALL = /usr/local/bin/ginstall -c
SOCKET_LIB = 
PACKAGE = mw
VERSION = 0.3.0

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
AR = ar

DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_LIMITS_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_UNISTD_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_VPRINTF=1 -DHAVE_GETCWD=1 -DHAVE_GETHOSTNAME=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_MKDIR=1 -DHAVE_STRSTR=1 -DHAVE_DYNAMIC_CAST= -DCONDOR_DIR=\"no\"  -I.
LIBS = 
CPPFLAGS = 
LDFLAGS = 
CXXFLAGS = -g -O2 -Wall

# To work with Insure, need to "setenv DEBUG_BUILD='insure'" and write/copy a good .psrc file
ifdef DEBUG_BUILD
DEBUG_CHECKER = $(DEBUG_BUILD)
MW_LIBDIR = $(MW_LIBDIR_DEBUG)
endif

CXXCOMPILE = $(DEBUG_CHECKER) $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(DEBUG_CHECKER) $(CXX)
CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@

# Subdirectories
SUBDIRS =  

# Libraries to be built, and dependent source files
LIBRARIES = libMWsocketworker.a libMWsocketmaster.a
libMWsocketmaster_a_SOURCES = MWSocketRCM.ii
libMWsocketmaster_a_LIBADD = MWSocketRCM.o
libMWsocketmaster_a_DEPENDENCIES =  MWSocketRCM.o
libMWsocketmaster_a_OBJECTS =

libMWsocketworker_a_SOURCES = MWSocketRCW.ii
libMWsocketworker_a_LIBADD = MWSocketRCW.o
libMWsocketworker_a_DEPENDENCIES =  MWSocketRCW.o
libMWsocketworker_a_OBJECTS = 

INCLUDES = -g -I. -I.. -I../.. -I$(CONDOR_DIR)/include $(MEASURE_DEFN)
INCLUDEFILES = MWSocketRC.h 

#-------------------------------------------------------------------------
#   Section 2) Explicit and Implicit Rules
#-------------------------------------------------------------------------

all: $(LIBRARIES)

libMWsocketmaster.a: $(libMWsocketmaster_a_OBJECTS) $(libMWsocketmaster_a_DEPENDENCIES)
	-rm -f libMWsocketmaster.a
	$(AR) cru libMWsocketmaster.a $(libMWsocketmaster_a_OBJECTS) $(libMWsocketmaster_a_LIBADD)
	$(RANLIB) libMWsocketmaster.a
	cp libMWsocketmaster.a $(MW_LIBDIR) 

libMWsocketworker.a: $(libMWsocketworker_a_OBJECTS) $(libMWsocketworker_a_DEPENDENCIES)
	-rm -f libMWsocketworker.a
	$(AR) cru libMWsocketworker.a $(libMWsocketworker_a_OBJECTS) $(libMWsocketworker_a_LIBADD)
	$(RANLIB) libMWsocketworker.a
	cp libMWsocketworker.a $(MW_LIBDIR) 

MWSocketRCM.ii:MWSocketRC.C
	$(CXX) -E $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -DSOCKET_MASTER MWSocketRC.C > MWSocketRCM.ii

MWSocketRCM.o:MWSocketRCM.ii
	$(CXX) -c $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -DSOCKET_MASTER MWSocketRCM.ii

MWSocketRCW.ii:MWSocketRC.C
	$(CXX) -E $(DEFS) $(MISC_DEFN) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) MWSocketRC.C > MWSocketRCW.ii

MWSocketRCW.o:MWSocketRCW.ii
	$(CXX) -c $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) MWSocketRCW.ii

.SUFFIXES: .C .o

.C.o:
	$(CXXCOMPILE) -c $<

#-------------------------------------------------------------------------
#   Section 3) Recursive Rules: Common
#-------------------------------------------------------------------------
install: $(LIBRARIES)
	$(mkinstalldirs) $(libdir)
	@list='$(LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  ${INSTALL} -m 644 $$p $(libdir)/$$p"; \
	    $(INSTALL) -m 644 $$p $(libdir)/$$p; \
	    echo " $(RANLIB) $(libdir)/$$p"; \
	    $(RANLIB) $(libdir)/$$p; \
	  fi; \
	done
	$(mkinstalldirs) $(includedir)
	@list='$(INCLUDEFILES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  ${INSTALL} -m 644 $$p $(includedir)/$$p"; \
	    $(INSTALL) -m 644 $$p $(includedir)/$$p; \
	  fi; \
	done	
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

check:

clean:
	-rm -f *.o *.a *core *.ii
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

distclean:
	-rm -f Makefile *.tar *.gz
	-rm -rf .deps
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*
	-rm -f *.o *.a *core *.ii
	[ "__$(SUBDIRS)" = "__" ] || for subdir in `echo "$(SUBDIRS)"`; do (cd $$subdir && $(MAKE) $@) ; done

.PHONY: all check clean distclean install
