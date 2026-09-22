CXX = g++
CXXFLAGS = -std=c++17 -Wall

scanner: main.cpp NetworkScanner.cpp
	$(CXX) $(CXXFLAGS) -o scanner main.cpp NetworkScanner.cpp

clean:
	rm -f scanner