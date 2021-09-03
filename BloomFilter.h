#include <array>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;


class bf {
public:
    bf(int bvSize, int NumofHash);
    int bvSize;
    void insert( string keyValue);
    bool query( string keyValue);
    void printBV();
    void updateBV(std::vector<bool> BV);
    void writeOutput(string outFile_path);

    int numHashes;
    vector<bool> bv;
};
