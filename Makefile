# Welcome to Mamat, 044101
#
#
# To compile, open the terminal in the folder that contains
# this file and run "make".
#
# To clean, open the terminal in the folder that contains
# this file and run "make clean".
#
# All eclipse projects in this machine are under /home/mamat/Documents/

#Defenitions

CC     = gcc
CFLAGS = -Wall -g -std=c99
CCLINK = $(CC)
OBJS   = grades.o libgrades.so
EXEC = libgrades.so
RM = rm -rf *.o *.exe libgrades.so

libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -llinked-list -L.	

#Dependencies - 

grades.o: grades.c grades.h linked-list.h
	$(CC) $(CFLAGS) -c -fpic grades.c

#Clean

clean:
	rm -rf *.o libgrades.so grades.o
