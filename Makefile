#
# Created by gmakemake (Ubuntu Jul 25 2014) on Mon Oct 19 00:13:42 2015
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


CFLAGS = -Wall -Wextra -pedantic -std=c99 -ggdb 
LFLAGS = -ggdb
CLIBFLAGS = -L/home/course/csci243/pub/projects/amigonet2 -lamigomem

default: all checksymbols

checksymbols:
	@./checksyms.sh


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	amigosim.c amigonet.c hash.c table.c test.c test_table.c
PS_FILES =	
S_FILES =	
H_FILES =	amigomem.h amigonet.h hash.h table.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	amigonet.o hash.o table.o 

#
# Main targets
#

all:	amigosim test test_table 

amigosim:	amigosim.o $(OBJFILES)
	$(CC) $(CFLAGS) -o amigosim amigosim.o $(OBJFILES) $(CLIBFLAGS)

test:	test.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test test.o $(OBJFILES) $(CLIBFLAGS)

test_table:	test_table.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_table test_table.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

amigonet.o:     amigomem.h amigonet.h hash.h table.h
amigosim.o:	amigomem.h amigonet.h table.h
hash.o:	hash.h
table.o:	amigomem.h hash.h table.h
test.o:	hash.h table.h
test_table.o:	amigomem.h hash.h table.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) amigosim.o test.o test_table.o core

realclean:        clean
	-/bin/rm -f amigosim test test_table 
