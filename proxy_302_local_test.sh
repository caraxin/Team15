#!/bin/bash

# Build the binary
make clean
make

# Start the server
echo "
# This is a comment.

port 2020;  # This is also a comment.
server_name 127.0.0.1;

path /4040 ProxyHandler {
	server_host localhost;
	server_port 4040;
}

# Default response handler if no handlers match.
default NotFoundHandler{}" > proxy_config

echo "
# This is a comment.
port 4040;  # This is also a comment.
server_name 127.0.0.1;

path /static StaticHandler{
	root /static;
}

path /test1 TestHandler1{}

path /test2 TestHandler2{}

# Default response handler if no handlers match.
default NotFoundHandler{}" > server4040_config

echo "
# This is a comment.
port 8080;  # This is also a comment.
server_name 127.0.0.1;

path /static StaticHandler{
	root /static;
}

# Default response handler if no handlers match.
default NotFoundHandler{}" > server8080_config

./webserver proxy_config &>/dev/null &
./webserver server4040_config &>/dev/null &
./webserver server8080_config &>/dev/null &

sleep 5
# Send request to proxy_server
curl -i -s localhost:2020/4040/test1 > proxy_response1
curl -i -s localhost:4040/static > server1_response
DIFF1_PROXY=$(diff proxy_response1 server1_response)
EXIT_STATUS_1=$?

curl -i -s localhost:2020/4040/test2 > proxy_response2
curl -i -s localhost:8080/static > server2_response
DIFF2_PROXY=$(diff proxy_response2 server2_response)
EXIT_STATUS_2=$?

echo "Testing the Proxy 302 Code Locally!"
echo "---------------------------------------------------------------------|"
echo "CASE1:                                                               |"
echo "Client       Request        -- localhost:2020/4040/test1             |"
echo "Proxy(2020)  Request        -- localhost:4040/test1                  |"
echo "Server(4040) Relative Path  -- Location: /static                     |"
# Error handling
if [ "$EXIT_STATUS_1" -eq 0 ]
then
    echo "SUCCESS: Integration test1 for 302 Code passed                   |"
else
    echo "FAILED: Integration test1 for 302 Code failed                    |"
    echo "diff:"
    echo $DIFF1_PROXY
fi

echo "----------------------------------------------------------------------|"
echo "CASE2:   																|"
echo "Client       Request        -- localhost:2020/4040/test2              |"
echo "Proxy(2020)  Request        -- localhost:4040/test2                   |"
echo "Server(4040) Absolute Path  -- Location: http://localhost:8080/static |"
# Error handling
if [ "$EXIT_STATUS_2" -eq 0 ]
then
    echo "SUCCESS: Integration test2 for 302 Code passed					|"
else
    echo "FAILED: Integration test2 for 302 Code FAILED 					|"
    echo "diff:"
    echo $DIFF2_PROXY
fi
echo "----------------------------------------------------------------------|"

# Shutdown the webserver and cleanup
echo "Cleaning up and shutting down"
pkill webserver
make clean
rm -f proxy_config
rm -f server4040_config
rm -f server8080_config
rm -f proxy_response1
rm -f server1_response
rm -f proxy_response2
rm -f server2_response

exit "$EXIT_STATUS_1"


