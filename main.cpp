#include "server.h"
#include <iostream>
#include <boost/version.hpp>
#include "nginx-configparser/config_parser.h"
#include <vector>
#include <unordered_map>
using namespace std;

int main(int argc, char** argv) {

  NginxConfigParser config_parser;
  NginxConfig config;
  config_parser.Parse(argv[1], &config);

  unordered_map<string, string> valuePairs;

  // loops through the statements in the config and creates value pairs
  // NOTE: DOES NOT WORK FOR CHILD BLOCKS YET;

  for (int k = 0; k < (int) config.statements_.size();k++) { 
  	string token1 = config.statements_[k]->tokens_[0];
  	string token2 = config.statements_[k]->tokens_[1];
  	valuePairs.insert({token1, token2});
  }

  //printf("%s\n", valuePairs["listen"].c_str());
  //printf("%s\n", valuePairs["server_name"].c_str());

  Team15::server::server s(valuePairs["server_name"],valuePairs["listen"]);
  
  s.run();
  
  return 0;
}
