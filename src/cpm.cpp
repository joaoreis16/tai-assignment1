#include <iostream>
#include "hashTable.cpp"

using namespace std;

//////// compile and run /////////
//                              //
//  g++ -o bin/cpm src/cpm.cpp  //
//  ./bin/cpm                   //
//                              //
//////////////////////////////////

int main(int argc, char** argv) {

    HashTable* table = new HashTable(10);

    table->insertElement(45);
    table->insertElement(23);
    table->insertElement(67);

    table->printAll();
        
    cout << "fim" << endl;
    return 0;
}