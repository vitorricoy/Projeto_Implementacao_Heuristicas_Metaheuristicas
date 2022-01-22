all:
	g++ -std=c++17 -Wall -c src/savings.cpp
	g++ -std=c++17 -Wall -c src/coiote.cpp
	g++ -std=c++17 -Wall src/main.cpp savings.o coiote.o -o prog
clean:
	rm src/savings.o src/coiote.o prog
