#ex5-4server.o: common.o ex5-4.server.c
#	gcc ex5-4.server.c common.o -lssl -lcrypto -lpthread -o ex5-4.server.out

ex5-3client.o: common.o ex5-3.client.c
	gcc ex5-3.client.c common.o -lssl -lcrypto -o ex5-3client.out

common.o: common.c
	gcc -c common.c -lssl -lcrypto
