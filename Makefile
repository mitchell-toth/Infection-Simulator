#Alec Houseman, Mitchell Toth
#May 2019

CXXFLAGS = -g -Wall -Og -std=c++11
CXX = g++


INFECTION_SIMULATOR_OBJECTS = Board.o conio.o Doctor.o Human.o main.o Scavenger.o


simulate: $(INFECTION_SIMULATOR_OBJECTS) 
	g++ -o simulate $(INFECTION_SIMULATOR_OBJECTS)
	@echo "Infection simulator program is in 'simulate'. Run as './simulate'"

clean:
	rm -f *.o simulate
	rm -r html latex

tar:
	tar -cvf Toth_Houseman_InfectionSimulator.tar Board.cpp Board.h conio.cpp conio.h Doctor.cpp Doctor.h Human.cpp Human.h Scavenger.cpp Scavenger.h main.cpp Makefile Doxyfile

Board.o: Board.h Human.h conio.h

conio.o: conio.h

Human.o: Human.h conio.h

Doctor.o: Human.h Doctor.h conio.h

Scavenger.o: Scavenger.h Human.h conio.h

main.o: Board.h Human.h
