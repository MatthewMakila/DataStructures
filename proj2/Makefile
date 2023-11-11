CXX = g++
CXXFLAGS = -Wall

proj2: fleet.o
	$(CXX) $(CXXFLAGS) fleet.o mytest.cpp -o proj2

fleet.o: fleet.cpp fleet.h
	$(CXX) $(CXXFLAGS) -c fleet.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj2

val1:
	valgrind ./proj2
