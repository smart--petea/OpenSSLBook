common.o: common.c
	gcc common.c -lssl -lcrypto
