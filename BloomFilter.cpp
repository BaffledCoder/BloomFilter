#include "BloomFilter.h"
#include "MurmurHash3.h"
#include "MurmurHash3.cpp"
#define LOG(x) std::cout<< std::endl << "LOG: "<< x << std::endl<< std::endl;

using namespace std;


// two private vars for bloomfilter, bv and num of hashes
bf::bf(int bvSize, int NumofHash):
        bv(bvSize),
        bvSize(bvSize),
        numHashes(NumofHash){}

void bf::printBV(){
    int bvSize = bv.size();
    for (int i = 0; i < bvSize; i++) {
        std::cout << bv.at(i) << ' ';
    }
    cout << endl;
}

void bf::updateBV(std::vector<bool> BV)
{
    bv = BV;
}

void bf::writeOutput(string outFile_path)
{
    ofstream outfile(outFile_path, ios::out | ios::trunc | ios::binary);
    outfile << bvSize;
    outfile << "\n";
    outfile << numHashes;
    outfile << "\n";
    std::ostream_iterator<bool> output_iterator(outfile);
    std::copy(bv.begin(), bv.end(), output_iterator);
    outfile.close();

}

bf  readBVfile(string inFile_path)
{
    int M;
    int k;
    ifstream inFile(inFile_path);
    inFile >> M;
    inFile >> k;
    string BV_str;
    vector<bool> BV;
    inFile >> BV_str;
    inFile.close();
//    cout <<endl<< "Here is the query result: \n";

    for ( int i = 0 ; i < BV_str.length(); i++) {
        BV.push_back(BV_str[i] == '1');
    }
    bf BloomFilter(M, k);
    BloomFilter.updateBV(BV);
    return BloomFilter;
}

// Using Murmurhash function to hash, which will return two parts
array<uint64_t, 2> mmh( string keyValue) {
    // define input and output for mmh
    //output:
    array<uint64_t, 2> hashValue;

    //input:
    size_t keyLen = keyValue.length() + 1;
    string *keyptr = &keyValue;

    //cout << keyValue << endl;
    //cout << keyLen << endl;

    // Hash it!
    MurmurHash3_x64_128(keyptr, keyLen, 0, hashValue.data());

    // return hashvalue array, which has 2 uint64_t.
    return hashValue;
}

// build a iterable hash function
inline uint64_t ithHash(uint8_t i,
                        uint64_t mmhA,
                        uint64_t mmhB,
                        uint64_t bvSize) {

//    std::cout << endl << (mmhA + i * mmhB) % bvSize << endl;
    return (mmhA + i * mmhB) % bvSize;
}

// Insert function
void bf::insert(string keyValue) {

    // Hash it!
    array<uint64_t, 2> hashValues = mmh(keyValue);

    //Update bv
    for (int i = 0; i < numHashes; i++) {
//        std::cout << "The " << i << "-th hash values are:" << endl;
        bv[ithHash(i, hashValues[0], hashValues[1], bv.size())] = true;
    }
}

// query function
bool bf::query(string keyValue)  {

    //Hash it!
    array<uint64_t, 2> hashValues = mmh(keyValue);

    //query the hashvalues
    for (int i = 0; i < numHashes; i++) {
        if (!bv[ithHash(i, hashValues[0], hashValues[1], bv.size())]) {
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------


int main(int argc, char* argv[]) {
    // if has only one file name, output the descriptions.
    int N;
    int M;
    int k;
    float FPR;
    string key_file;
    string bv_file;
    string outFile_path;
    string fun_apply;
    ifstream inFile;
    ofstream outFile;

    if (argc == 1) {
        std::cout << "This program is used for building bloom filter from a given input string file. \n-k <key file> \n-f <fpr> \n-n <num. distinct keys> \n-o <output file>";
        return 0;
    }

        // Read in args
    else {
        // First argument is the function want to use.
        fun_apply = argv[1];

        if (fun_apply == "build")
        {
            // -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
            for (int i = 2; i < argc; i += 2) {
                // Define the arg for this iteration.
                string arg_type = argv[i];
                //cout << arg_type << "\n";

                // Read in the arg of this iteration.
                string arg = argv[i + 1];
                //cout << arg << "\n";

                if (arg_type == "-k") {
                    //Frist identity -k

                    stringstream key_file_str(arg);
                    key_file_str >> key_file;
                }
                else if (arg_type == "-f") {
                    //Then identify -f
                    stringstream FPR_str(arg);
                    FPR_str >> FPR;
                }
                    //Then identify -n
                else if (arg_type == "-n") {
                    stringstream N_str(arg);
                    N_str >> N;
                }
                    // Finally identify -o
                else if (arg_type == "-o") {
                    stringstream outFile_str(arg);
                    outFile_str >> outFile_path;
//                    LOG(outFile_path)
                }
            }

            //build -k list.txt -f 0.001 -n 10 -o out.txt

            M = ceil(-((N*log(FPR)) / pow(log(2), 2.0)));
            k = ceil((M / N) * log(2));

            bf BloomFilter(M, k);
            inFile.open(key_file);
            if (!inFile.is_open()) {
                exit(EXIT_FAILURE);
            }

            string word;
            inFile >> word;
            while (inFile.good()) {
                BloomFilter.insert(word);
                inFile >> word;
            }
            BloomFilter.writeOutput(outFile_path);
            //std::cout << "M = " << M << "\nk = " << k << "\nKey File is " << key_file << "\n";
            std::cout << "M = " << M << "\nk = " << k << "\n";
//            std::cout << "BV is: \n";
//            BloomFilter.printBV();
        }
        else {

           // Building up query function
           // query -i out.txt -q list.txt -o queryResults.txt
           for (int i = 2; i < argc; i += 2) {
                       // Define the arg for this iteration.
                       string arg_type = argv[i];
                       //cout << arg_type << "\n";

                       // Read in the arg of this iteration.
                       string arg = argv[i + 1];
                       //cout << arg << "\n";

                       if (arg_type == "-i") {
                           //First identity -k

                           stringstream bv_file_str(arg);
                           bv_file_str >> bv_file;
                           //cout << "BV file is " << bv_file << "\n";

                       }
                       else if (arg_type == "-q") {
                           //Then identity -q

                           stringstream key_file_str(arg);
                           key_file_str >> key_file;
                           //cout << "key file is " << key_file << "\n";

                       }
                       else if (arg_type == "-o") {
                           //Then identity -o

                           stringstream out_file_str(arg);
                           out_file_str >> outFile_path;
                           //cout << "key file is " << key_file << "\n";
                       }
           }
           bf BloomFilter = readBVfile(bv_file);

            ofstream queryFile(outFile_path, ios::out | ios::trunc | ios::binary);

            inFile.open(key_file);
            string key;
            inFile >> key;
            while (inFile.good()) {
//                std::cout << endl << key << endl;
                bool query_result = BloomFilter.query(key);

                if(query_result){
//                    std::cout << "TRUE" << endl;
                    queryFile << key;
                    queryFile << "\t";
                    queryFile << 1;
                    queryFile << "\n";
                }else{
//                    std::cout << "FALSE" << endl;
                    queryFile << key;
                    queryFile << "\t";
                    queryFile << 0;
                    queryFile << "\n";
                }

                inFile >> key;
            }

            queryFile.close();

            }
        }
    return 0;

}
