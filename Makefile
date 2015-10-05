ex5-6: common.o
	gcc ex5-6.server.c common.o -lssl -lcrypto -lpthread -o ex5-6server.out

ex5-5: common.o
	gcc ex5-5.client.c common.o -lssl -lcrypto -o ex5-5client.out

#ex5-4server.o: common.o ex5-4.server.
#	gcc ex5-4.server.c common.o -lssl -lcrypto -lpthread -o ex5-4.server.out

ex5-3client.o: common.o ex5-3.client.c
	gcc ex5-3.client.c common.o -lssl -lcrypto -o ex5-3client.out

common.o: common.c
	gcc -c common.c -lssl -lcrypto

setupCAEnv:
	bash shScripts/setupCAEnvironment.sh
