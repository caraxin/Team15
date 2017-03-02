# Team 15

Winter CS130 Team15 Repository. Working on HTTP server with different functionality configurable using nginx standards and conforming to the following common api:
https://github.com/UCLA-CS130/webserver-api

## New Updated for Assignment 7
When you start the server as followed, type the url: localhost:2020/ in the browser.
Then you will get the homepage of ucla.
Our team(TSC) set the ProxyHandler root as '/' in the test_config to make sure all of the src/href files will be fetched successfully.
The server for our proxy is ucla.edu, which have some 302 codes sent back. And our server handles 302 codes successfully.

## Getting Started

These instructions will show you what to do to run our project and test everything

### Prerequisites

You need to be running Ubuntu 14.04 and have boost, gtest and git installed. You can do so by running the following code in terminal: 

```
sudo apt-get update
sudo apt-get install libboost-all-dev
sudo apt-get install build-essential
sudo apt-get install git
```

### Installing

In order to install our project, go to a clean folder and run:

```
git clone --recursive https://github.com/UCLA-CS130/Team15.git
```
Then, you can run:
```
cd Team15
make
```
In order to build the executable.


### Testing
We have multiple testing options.
#### Unit Tests
For unit tests, please run:
```
make
make test
```
The output will dictate whether they succeeded or not.

#### Unit Test Coverage
For test coverage tests, please run:
```
make
make test
make check
```
This will show the percentage of coverage of our tests.
#### Integration Tests
For integration tests, please run:
```
make
make integration
```

## Usage

In order to run our project with a config file in the specified format, please run this following code:
```
./webserver <filename>
```
And the output will dictate if the server was launched sucessfully or not.

Specificed format is in the following link:
https://github.com/UCLA-CS130/webserver-api
