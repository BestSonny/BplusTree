#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include "BPlusTree.hpp"
#include "utils.h"
using namespace std;

#define DELIMITERS "(),"

int main(int argc, char* argv[]) {
  ifstream file(argv[1]);  // pass file name as argment
  string linebuffer;
  vector<string> elems;
  float key;
  string value;
  string r;
  const size_t n = 3;
  bool insert_status = false;
  BPlusTree<float, string> bt;
  while (file && getline(file, linebuffer)) {
    if (linebuffer.length() == 0) continue;
    split(linebuffer, DELIMITERS, elems);
    string operation = elems.at(0);
    if ( operation == "Insert") {
      assert (elems.size() == 3);
      key = stof(elems.at(1));
      value = elems.at(2);
      bt.put(key, value);
    } else if (operation == "Search") {
      assert (elems.size() == 3 || elems.size() == 2);
      if (elems.size() == 2) {
        multimap<float, string> res = bt.getrange(stof(elems.at(1)),stof(elems.at(1)));
        if (res.size() == 0)
          std::cout << "Null" << '\n';
        else{
          multimap<float, string>::iterator it;
          for (it = res.begin(); it != res.end(); ++it)
              std::cout << "(" << it->first << ", " << it->second << "),";
          std::cout << std::endl;
        }
      }else{
        multimap<float, string> res = bt.getrange(stof(elems.at(1)),stof(elems.at(2)));
        multimap<float, string>::iterator it;
        for (it = res.begin(); it != res.end(); ++it)
            std::cout << "(" << it->first << ", " << it->second << "),";
        std::cout << std::endl;
      }
    } else {
      cout << "error" << endl;
      exit(0);
    }

    for (std::vector<string>::iterator it = elems.begin() ; it != elems.end(); ++it) {
      cout << ' ' << *it;
    }
    cout << endl;
    elems.clear();
  }
  return 0;
}
