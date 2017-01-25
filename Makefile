CC=g++

CFLAGS=-g -std=c++11 -lboost_system -Wall -Werror -lpthread
HEADERS=HttpResponse.h HttpRequest.h HttpMessage.h server.h connection.h nginx-configparser/config_parser.h
SOURCES=main.cpp server.cpp connection.cpp nginx-configparser/config_parser.cc
all: webserver

webserver : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

clean :
	rm -f webserver *~ *.o
