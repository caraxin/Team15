CC=g++
GTEST_DIR=nginx-configparser/googletest/googletest
CFLAGS=-g -std=c++11 -lboost_system -Wall -Werror -lpthread
HEADERS=HttpResponse.h HttpRequest.h HttpMessage.h server.h connection.h nginx-configparser/config_parser.h
CLASS_SOURCES=server.cpp connection.cpp nginx-configparser/config_parser.cc
SOURCES=main.cpp $(CLASS_SOURCES)
TESTFILES=server_test.cc connection_test.cc Http_test.cc
all: webserver

webserver : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

clean :
	rm -f webserver server_test *~ *.o *.a *.gcda *.gcno *.gcov


test : $(SOURCES) $(HEADERS) $(TESTFILES)
	$(CC) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) -isystem $(GTEST_DIR)/include $(TESTFILES) $(CLASS_SOURCES) ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(CFLAGS) -fprofile-arcs -ftest-coverage -o server_test
	./server_test
	cd nginx-configparser && ./build_tests.sh && ./config_parser_test
