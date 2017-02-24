#!/bin/bash

# Build the binary
make clean
make

# Start the server
echo "
# This is a comment.

port 2020;  # This is also a comment.
server_name 127.0.0.1;

path / StaticHandler {
  root /foo/bar;
}

path /echo EchoHandler;

# Default response handler if no handlers match.
default NotFoundHandler;" > test_config
#./webserver test_config &>/dev/null &
./webserver test_config &

# Send request to server
curl -i -s localhost:4000 > test_response

# Verify the response from the server works as expected
DIFF=$(diff expected_response test_response)
EXIT_STATUS=$?

# Error handling
if [ "$EXIT_STATUS" -eq 0 ]
then
    echo "SUCCESS: Integration test passed"
else
    echo "FAILED: Integration test failed"
    echo "diff:"
    echo $DIFF
fi

# Shutdown the webserver and cleanup
echo "Cleaning up and shutting down"
pkill webserver
make clean
rm -f test_response

exit "$EXIT_STATUS"
