a.out:crackrar.o main.cpp
	g++ crackrar.o main.cpp -o a.out -lssl -lcrypto
crackrar.o:crackrar.cpp crackrar.h
	g++ -c crackrar.cpp crackrar.h -lssl -lcrypto
clean:
	rm crackrar.o crackrar.h.gch a.out
