#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <typeinfo>
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
  ofstream myfile("output_file.txt");
  BPlusTree<float, string> bt(10);
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
        if (res.size() == 0){
          myfile << "Null" << std::endl;
        }
        else{
          multimap<float, string>::iterator it;
          for (it = res.begin(); it != res.end(); ++it)
              myfile << it->second << ", ";
          myfile.seekp(-2,myfile.cur);
          myfile << std::endl;
        }
      }else{
        multimap<float, string> res = bt.getrange(stof(elems.at(1)),stof(elems.at(2)));
        multimap<float, string>::iterator it;
        for (it = res.begin(); it != res.end(); ++it)
            myfile << "(" << it->first << ", " << it->second << "), ";
        myfile.seekp(-2,myfile.cur);
        myfile << std::endl;
      }
    } else {
      cout << "error" << endl;
      exit(0);
    }
    elems.clear();
  }
  myfile.close();
  return 0;
}
