filesysTest: main2.o sdisk.o filesys1.o
	c++ main2.o sdisk.o filesys1.o -o filesysTest

sdiskTest: main1.o sdisk.o
	c++ main1.o sdisk.o -o sdiskTest

filesys.o: filesys1.cpp
	c++ -c filesys1.cpp
	
main2.o: main2.cpp
	c++ -c main2.cpp

main1.o: main1.cpp
	c++ -c main1.cpp

sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp

clean:
	rm *.o sdiskTest filesysTest disk1 test1
