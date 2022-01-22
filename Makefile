all:
	g++ -std=c++17 -Wall -g -c src/savings.cpp
	g++ -std=c++17 -Wall -g -c src/coiote.cpp
	g++ -std=c++17 -Wall -g src/main.cpp savings.o coiote.o -o prog
clean:
	rm src/savings.o src/coiote.o prog
