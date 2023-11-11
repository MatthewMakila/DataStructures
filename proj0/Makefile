CXX = g++
CXXFLAGS = -Wall

proj0: section.o mytest.cpp
	$(CXX) $(CXXFLAGS) section.o mytest.cpp -o proj0

section.o: section.cpp section.h
	$(CXX) $(CXXFLAGS) -c section.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj0

val1:
	valgrind ./proj0
