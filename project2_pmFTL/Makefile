PREFIX		=
CC			= $(PREFIX)gcc
CXX			= $(PREFIX)g++
CFLAGS		= -g -O2 -Wall 
LIBS		= 
RM			= rm
TAR			= tar

TARGET		= ftl
CSRCS		= ftl_sim.c nand.c ftl.c
CXXSRCS		= 
HEADERS		= nand.h
OBJS		= $(CSRCS:.c=.o) $(CXXSRCS:.cpp=.o)

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS) 

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o: $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

tar:
	$(RM) -f $(TARGET).tgz
	$(TAR) cvzf $(TARGET).tgz $(CSRCS) $(HEADERS) Makefile
	ls -l $(TARGET).tgz

clean:
	$(RM) -f $(TARGET) $(TARGET).tgz $(OBJS) *~
