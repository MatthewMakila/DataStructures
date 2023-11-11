CXX = g++
CXXFLAGS = -Wall

proj1: elevator.o mytest.cpp
	$(CXX) $(CXXFLAGS) elevator.o mytest.cpp -o proj1

elevator.o: elevator.cpp elevator.h
	$(CXX) $(CXXFLAGS) -c elevator.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj1

val1:
	valgrind ./proj1
