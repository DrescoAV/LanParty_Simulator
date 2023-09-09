CC=gcc
CFLAGS=-g -Wall -Wextra -std=c99

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)
TARGETS=$(OBJS:%.o=%)


build: $(OBJS)
	$(CC) $(CFLAGS) -o lanParty $(OBJS)
	
run_vma:build
	./lanParty c.in d.in r.out

pack:
	zip -FSr 314AA_Andreescu_Andrei_Valerian_Tema1 Makefile *.c *.h README.MD

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: pack clean	
