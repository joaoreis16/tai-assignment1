#include <iostream>
using namespace std;

//////// compile and run /////////
//                              //
//  g++ -o bin/cpm src/cpm.cpp  //
//  ./bin/cpm                   //
//                              //
//////////////////////////////////

int main(int argc, char** argv) {
    printf("You have entered %s arguments\n", argc);
  
    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
        
  
    return 0;
}