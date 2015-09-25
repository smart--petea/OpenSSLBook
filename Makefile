ex5-3client.o: common.o ex5-3.client.c
	gcc ex5-3.client.c common.o -lssl -lcrypto

common.o: common.c
	gcc -c common.c -lssl -lcrypto
