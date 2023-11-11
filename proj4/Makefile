CXX = g++
CXXFLAGS = -Wall

proj4: dnadb.o
	$(CXX) $(CXXFLAGS) dnadb.o mytest.cpp -o proj4

dnadb.o: dnadb.cpp dnadb.h
	$(CXX) $(CXXFLAGS) -c dnadb.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj4

val1:
	valgrind ./proj4