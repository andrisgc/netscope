CXX = g++
CXXFLAGS = -std=c++17 -Wall

scanner: src/main.cpp src/NetworkScanner.cpp
	$(CXX) $(CXXFLAGS) -o scanner src/main.cpp src/NetworkScanner.cpp

clean:
	rm -f scanner