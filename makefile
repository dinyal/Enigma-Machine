enigma: main.o enigma.o errorcheck.o plugboard.o rotor.o reflector.o
	g++ main.o enigma.o errorcheck.o plugboard.o rotor.o reflector.o -o enigma

main.o: main.cpp errors.h enigma.h constants.h
	g++ -Wall -g -c main.cpp

enigma.o: enigma.cpp errors.h plugboard.h reflector.h rotor.h constants.h
	g++ -Wall -g -c enigma.cpp

plugboard.o: plugboard.cpp errors.h errorcheck.h plugboard.h constants.h
	g++ -Wall -g -c plugboard.cpp

rotor.o: rotor.cpp errors.h errorcheck.h rotor.h constants.h
	g++ -Wall -g -c rotor.cpp

reflector.o: reflector.cpp errors.h errorcheck.h reflector.h constants.h
	g++ -Wall -g -c reflector.cpp

errorcheck.o: errorcheck.cpp errorcheck.h errors.h constants.h
	g++ -Wall -g -c errorcheck.cpp

.PHONY: clean

clean:
	rm -f *.o enigma