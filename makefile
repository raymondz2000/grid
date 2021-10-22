all: display.o gridMaker.o gridTester.o  
	gcc -o gridTester display.o gridMaker.o gridTester.o -lm -lX11 	
display.o: display.c obstacles.h display.h
	gcc -c display.c  -lm
gridMaker.o: gridMaker.c obstacles.h display.h
	gcc -c gridMaker.c -lm
gridTester.o: gridTester.c obstacles.h display.h
	gcc -c gridTester.c -lm
clean:
	 

	rm -f display.o gridMaker.o gridTester.o gridTester