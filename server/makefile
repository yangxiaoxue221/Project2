s: server.o ser_sub.o ser_sub2.o
		gcc -o s server.o ser_sub.o ser_sub2.o -lmysqlclient -L/usr/lib64/mysql -lpthread

server.o: server.c
		gcc -c server.c

ser_sub.o: ser_sub.c
		gcc -c ser_sub.c

ser_sub2.o: ser_sub2.c
		gcc -c ser_sub2.c 
