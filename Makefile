DEFS :=  $(wildcard *.h)
SRCS_BASIC := hsm_event.c utils_hsm.c time_counter_hsm.c
SRCS_EXTRA :=  hsm_header.c dial_hsm.c display_hsm.c top_hsm.c
OBJDIR = obj
OBJSPATHS := $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS_BASIC))
OBJSPATHS_EXTRA :=  $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS_EXTRA))
CC = gcc

main: main.c $(OBJSPATHS) $(OBJSPATHS_EXTRA) device_interface.h
	@echo "Linking .... "
	$(CC) -o main main.c obj/*.o -lncurses

run: main
	./main

all: $(OBJSPATHS) $(OBJSPATHS_EXTRA)
	@echo "Compiling ...."
	$(CC) $^ -o $@

$(OBJDIR)/%.o : %.c %.h | $(OBJDIR) 
	$(CC) -c $< -o $@

$(OBJDIR):
	@mkdir $(OBJDIR)

$(OBJDIR)/dial_hsm.o : dial_hsm.c dial_hsm.h \
                       $(OBJDIR)/utils_hsm.o $(OBJDIR)/hsm_event.o $(OBJDIR)/hsm_header.o\
					   $(OBJDIR)/time_counter_hsm.o \
                       device_interface.h
	$(CC) -c dial_hsm.c -o $(OBJDIR)/dial_hsm.o

$(OBJDIR)/top_hsm.o : top_hsm.c top_hsm.h \
                      $(OBJDIR)/hsm_event.o $(OBJDIR)/dial_hsm.o $(OBJDIR)/hsm_header.o \
					  $(OBJDIR)/time_counter_hsm.o $(OBJDIR)/display_hsm.o
	$(CC) -c top_hsm.c -o $(OBJDIR)/top_hsm.o

$(OBJDIR)/hsm_header.o : hsm_header.c hsm_header.h \
                         $(OBJDIR)/hsm_event.o
	$(CC) -c hsm_header.c -o $(OBJDIR)/hsm_header.o

$(OBJDIR)/display_hsm.o : display_hsm.c display_hsm.h \
						  $(OBJDIR)/hsm_header.o $(OBJDIR)/time_counter_hsm.o
	$(CC) -c display_hsm.c -o $(OBJDIR)/display_hsm.o

clear :
	rm obj/*