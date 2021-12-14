

INCLUDES = -I ../  -I ../../test -I ../../utils

test_tuntap:tuntap.o test_tuntap.o utils.o
	gcc  -o test_tuntap tuntap.o test_tuntap.o utils.o $(INCLUDES)

tuntap.o: ../tuntap.c
	gcc   -c $< $(INCLUDES)

test_tuntap.o:test_tuntap.c
	gcc -c  $< $(INCLUDES)

utils.o:../../utils/utils.c
	gcc -c $< $(INCLUDES)

clean:
	rm -fr *.o test_tuntap

functest:clean test_tuntap
	@echo "testing"
	./test_tuntap