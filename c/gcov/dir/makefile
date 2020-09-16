link : compile
	cc -o ./bin/run ./src/source.o ./src/main.o -lgcov

compile : ./src/source.c ./src/main.c
	cc -c -ftest-coverage -fprofile-arcs ./src/source.c -o ./src/source.o
	cc -c -ftest-coverage -fprofile-arcs ./src/main.c -o ./src/main.o

clean : 
	rm ./src/*.o
	rm ./bin/run
