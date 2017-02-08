#include "server.h"
#include <iostream>
#include <boost/version.hpp>
#include "nginx-configparser/config_parser.h"
#include <vector>
#include <unordered_map>
#include "requestconfig.h"
using namespace std;

string getRootFromChild(NginxConfig config) {
  if (config.statements_.size() == 0)
    return "";

  for (int k = 0; k < (int) config.statements_.size(); k++) {
    if (config.statements_[k]->tokens_[0] == "root") {
      return config.statements_[k]->tokens_[1];
    }
  }

  return "";
}


void parseValuePairs(unordered_map<string, string> &valuePairs,vector<requestconfig> &vec, NginxConfig config) {
  for (int k = 0; k < (int) config.statements_.size(); k++) { 

    if (config.statements_[k]->tokens_.size() == 1) {
      parseValuePairs(valuePairs, vec, *(config.statements_[k]->child_block_));
      continue;
    }

    if (config.statements_[k]->tokens_[0] == "path") {
      string token1 = config.statements_[k]->tokens_[1];
      string token2 = config.statements_[k]->tokens_[2];
      string token3 = (config.statements_[k]->child_block_ == NULL) ? ""
        : getRootFromChild(*(config.statements_[k]->child_block_));
      if (token3 == "") {
        vec.push_back(requestconfig(token1, token2));
      } else {
        vec.push_back(requestconfig(token1, token2, token3));
      }
    } else {
      string token1 = config.statements_[k]->tokens_[0];
      string token2 = config.statements_[k]->tokens_[1];
      valuePairs.insert({token1, token2});
    }
  }
}


int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage:" << argv[0] << " config" << std::endl;
    return 1;
  }

  NginxConfigParser config_parser;
  NginxConfig config;

  bool validParse = config_parser.Parse(argv[1], &config);
  if (!validParse) return -1;

  unordered_map<string, string> valuePairs;
  vector<requestconfig> vec;

  parseValuePairs(valuePairs, vec, config);

  //We need to construct something like this, currently hard coded:
  //vec.push_back(requestconfig("/echo","EchoHandler"));
  //vec.push_back(requestconfig("/static","StaticFileHandler","."));

  Team15::server::server s(valuePairs["server_name"],valuePairs["listen"],vec);   
  s.run();

  return 0;
}
