CC = gcc -Wall

server: server.c writer.o course.o student.o socketdata.h
	$(CC) -o server server.c course.o student.o writer.o

client: client.c parser.o socketdata.h
	$(CC) -o client client.c parser.o

course.o: course.c student.h course.h mainDLL.h
	$(CC) -c course.c

student.o: student.c student.h mainDLL.h
	$(CC) -c student.c

parser.o: parser.c socketdata.h
	$(CC) -c parser.c

writer.o: writer.c
	$(CC) -c writer.c
clean:
	rm -f *.o 2102_2124
