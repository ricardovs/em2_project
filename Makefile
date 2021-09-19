DEFS :=  $(wildcard *.h)
SRCS_BASIC := display_hsm.c hsm_header.c  hsm_event.c
SRCS_EXTRA := dial_hsm.c top_hsm.c main.c
OBJDIR = obj
OBJSPATHS := $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS_BASIC))
OBJSPATHS_EXTRA :=  $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS_EXTRA))
CC = gcc

run: main
	./main

all: $(OBJSPATHS) $(OBJSPATHS_EXTRA)
	@echo "Compiling ...."
	$(CC) $^ -o $@

$(OBJDIR)/%.o : %.c %.h | $(OBJDIR) 
	$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir $(OBJDIR)

main: | $(OBJSPATHS) $(OBJSPATHS_EXTRA)
	@echo "Linking .... "
	$(CC) -o main -Wall $(OBJSPATHS) $(OBJSPATHS_EXTRA)

$(OBJDIR)/dial_hsm.o : dial_hsm.c dial_hsm.h main.c
	$(CC) -c dial_hsm.c  -o $(OBJDIR)/dial_hsm.o

$(OBJDIR)/main.o :| $(SRCS_BASIC) $(SRCS_EXTRA) $(DEFS)
	$(CC) -c main.c -o $(OBJDIR)/main.o

$(OBJDIR)/top_hsm.o : $(SRCS_BASIC) $(DEFS)  dial_hsm.c top_hsm.c
	$(CC) -c top_hsm.c -o $(OBJDIR)/top_hsm.o

clear :
	rm obj/*