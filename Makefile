CC=g++

CFLAGS=-g -std=c++11 -lboost_system -Wall -Werror -lpthread
HEADERS=HttpResponse.h HttpRequest.h HttpMessage.h server.h

all: webserver

webserver : main.cpp server.cpp $(HEADERS)
	$(CC) main.cpp server.cpp $(CFLAGS) -o $@

clean :
	rm -f webserver *~ *.o
