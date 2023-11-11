CXX = g++
CXXFLAGS = -Wall

proj3: squeue.o
	$(CXX) $(CXXFLAGS) squeue.o mytest.cpp -o proj3

fleet.o: squeue.cpp squeue.h
	$(CXX) $(CXXFLAGS) -c squeue.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj3

val1:
	valgrind ./proj3