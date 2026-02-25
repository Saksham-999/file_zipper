#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <iomanip>

using namespace std;


//Huffman Tree Node (Binary Tree Node)
struct HuffmanNode {
    char data;           // Character stored 
    int  freq;           // Frequency of character
    HuffmanNode* left;   //left child
    HuffmanNode* right;  //right child

    HuffmanNode(char d, int f);
};

//Min-Heap Comparator (for Priority Queue)
//Greedy algorithm(pick two nodes with lowest frequency)
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b);
};
//  HUFFMAN TREE CLASS
class HuffmanTree {
private:            
    HuffmanNode* root;   //stores starting node of tree (for tree traversal) 
    unordered_map<char, string> codes;    // stores char  -> binary code (during compression)
    unordered_map<string, char> reverse;  // stores binary code -> char (during decompression)
    //unordered_map is a C++ standard library container which stores stores key-value pairs


    void generateCodes(HuffmanNode* node, const string& code);  //recursively traverse tree and generate huffman codes
    void freeTree(HuffmanNode* node);     //deletes dynamically allocated nodes
    void printTree(HuffmanNode* node, int space, int gap); //displays tree(for debugging)

public:
    HuffmanTree();  //Initializes tree.
    ~HuffmanTree();  //Automatically frees memory

    unordered_map<char, int> buildFrequencyTable(const string& text); //reads text and counts and stored using unordered_map<char,int>
    void   build(const unordered_map<char, int>& freq); //creates huffman tree using priority queue and greedy algorithm 
    string encode(const string& text);  //converts text to binary string
    string decode(const string& bitStr);  //converts binary to original text
    void   printCodeTableCSV();  //displays generated codes (just for demonstration)
    void   printTree();   //displays tree 
    
};

//file input/output (compress /decompress)
void saveCompressed(const string& filename,
                    const unordered_map<char, int>& freqTable,
                    const string& encoded);

void loadCompressed(const string& filename,
                    unordered_map<char, int>& freqTable,
                    string& encoded);

void compressFile  (const string& inputFile,  const string& outputFile);
void decompressFile(const string& inputFile,  const string& outputFile);

#endif
