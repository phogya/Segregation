#
# Created by gmakemake (Ubuntu Jul 25 2014) on Tue Sep 29 23:44:23 2015
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS =	-g -std=c99 -ggdb -Wall -Wextra -pedantic
CLIBFLAGS = 	-lm

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	check_input.c display.c segregation.c
PS_FILES =	
S_FILES =	
H_FILES =	check_input.h display.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	check_input.o display.o 

#
# Main targets
#

all:	segregation 

segregation:	segregation.o $(OBJFILES)
	$(CC) $(CFLAGS) -o segregation segregation.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

check_input.o:	check_input.h
display.o:	display.h
segregation.o:	check_input.h display.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) segregation.o core

realclean:        clean
	-/bin/rm -f segregation 
