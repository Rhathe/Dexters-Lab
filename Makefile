dextersLab.exe: dextersLab.o graph.o heap.o hash.o
	g++ -o dextersLab.exe dextersLab.o graph.o heap.o hash.o
	
dextersLab.o: dextersLab.cpp graph.h
	g++ -c dextersLab.cpp

graph.o: graph.cpp graph.h
	g++ -c graph.cpp

heap.o: heap.cpp heap.h
	g++ -c heap.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o dexterdebug.exe graph.cpp heap.cpp hash.cpp

clean:
	rm *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
