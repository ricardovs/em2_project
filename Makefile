DEFS :=  $(wildcard *.h)
SRCS_BASIC := display_hsm.c hsm_header.c  hsm_event.c
SRCS_EXTRA := utils_hsm.c dial_hsm.c top_hsm.c
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

main: main.c $(OBJSPATHS) $(OBJSPATHS_EXTRA)
	@echo "Linking .... "
	$(CC) main.c obj/*.o -o main

$(OBJDIR)/dial_hsm.o : dial_hsm.c dial_hsm.h main.c
	$(CC) -c dial_hsm.c  -o $(OBJDIR)/dial_hsm.o

$(OBJDIR)/top_hsm.o : $(SRCS_BASIC) $(DEFS)  dial_hsm.c top_hsm.c
	$(CC) -c top_hsm.c -o $(OBJDIR)/top_hsm.o

$(OBJDIR)/utils_hsm.o : utils_hsm.c utils_hsm.c
	$(CC) -c utils_hsm.c -o $(OBJDIR)/utils_hsm.o

clear :
	rm obj/*