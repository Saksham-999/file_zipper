#include "huffman.hpp"

//Huffman Node constructor
HuffmanNode::HuffmanNode(char d, int f){
    data = d;
    freq = f;
    left = right = nullptr;
}

//min-heap comparator (as huffman requires to pick 2 lowest freaquency nodes)
bool Compare::operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->freq > b->freq; //returns true if a should come after b (so smallest frequency stays at top)
}


//huffmantree private helpers

// Recursively generate binary codes by traversing the tree
void HuffmanTree::generateCodes(HuffmanNode* node, const string& code) {
    if (!node){
        return;  //stops recursion if node doesn't exist
    }

    // Leaf node(an actual character)
    if (!node->left && !node->right) //if both left child and right child is null
    {                                      
        codes[node->data] = code;           //node->data is the character stored in the leaf node  
        reverse[code] = node->data;      //code is the path you took from the root to this node(binary string)
        return;
    }
    generateCodes(node->left,  code + "0"); // left  edge -> 0
    generateCodes(node->right, code + "1"); // right edge -> 1
}

// Free all tree nodes (postorder traversal because children must be deleted before parent to prevent memory leakage)
void HuffmanTree::freeTree(HuffmanNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

// Print tree rotated 90° clockwise (right subtree on top)
void HuffmanTree::printTree(HuffmanNode* node, int space = 0, int gap = 5) {  //gap controls width between levels
    if (!node){
        return;
    }
    space += gap;   //each deeper level of the tree means more spaces to the right
                    //After space += gap,space represents the indentation of the child, not the current node so we subtract gap to get the current node later on 
    printTree(node->right, space, gap);  //prints right subtree first 
    cout << "\n" << string(space - gap, ' ');   // print spaces before node and (space - gap) ensures parent and child nodes are aligned correctly
   
    if (node->data == '\0')    //checking if the node is internal (only leaf nodes contain data as charater )
        cout << "[" << node->freq << "]\n";  //prints frequency of combined children in square brackets

    else {     //else it is leaf node
        string ch = (node->data == '\n') ? "\\n" :  //if character is newline print \n 
                    (node->data == ' ')  ? "SP"  : string(1, node->data);   //else if the character is space print 'SP' otherwise print normal characters a,b,c,d  bla bla bla
        cout << "'" << ch << "'(" << node->freq << ")\n"; // print like 'a'(3)
    }
    printTree(node->left, space, gap);  //recursive call to print left subtree next
}


//constructor to start tree empty and destructor to auto free memory 
HuffmanTree::HuffmanTree() : root(nullptr) {}
HuffmanTree::~HuffmanTree() { freeTree(root); }


//huffmantree  public methods

//Count character frequencies
unordered_map<char, int> HuffmanTree::buildFrequencyTable(const string& text) {
    unordered_map<char, int> freq;
    for (char c : text){  //harek loop ma text ko ek ek character c hudai janxa
        freq[c]++;        //harek specific character ko count store hunxa 
    }
    return freq;    //freq have pairs of (character,frequency)
}

//build huffman tree using min-heap (Greedy Algorithm)

void HuffmanTree::build(const unordered_map<char, int>& freq) {
    // Push all characters into the priority queue (min-heap)
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;  //priority queue is a special container in C++ , like a heap the largest/smallest element is always at the top
    //syntax: priority_queue<Type, Container, Comparator> pq; <each element of the heap is a pointer to HuffmanNode,
    //                                                          container storing the nodes is a vector,
    //                                                          Compare(custom comparator) ensures root always has the smallest frequency node>
    for (auto& pair : freq)   //pair is each element pair in freq
        minHeap.push(new HuffmanNode(pair.first, pair.second));      // for every character a node is created and pushed to heap

    //heap stores address and each address points to a node in memory 
    //initially heap stores pointers to individual character nodes 
    //during merging , when a merged node  is pushed in heap being priority queue it reorganizes in order(when a node is pushed into a priority queue, it is first added at the bottom and then repeatedly swapped with its parent until heap order is restored.)
    //and after the repeated merging is completed , minHeap only contains a pointer to the root node 

    // Greedy merging(repeatedly merging the two lowest frequency nodes until only one node remains)
    while (minHeap.size() > 1) {
        HuffmanNode* left  = minHeap.top(); minHeap.pop();   //since character with lowest frequency is at top we take the two top element
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        // Internal node: combined frequency (no character)
        HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left  = left;
        merged->right = right;

        minHeap.push(merged);
    }

    root = minHeap.top(); //minHeap ma root node ko pointer matra xa tara teslai access garna .top() use gareko 
    generateCodes(root, ""); // aba root node bataw traverse gardai harek character ko lagi binary code genearte garxa
    //ani generate vako huffman binary haru codes ra reverse(HuffmanTree object ko private data) ma store variable ma store vayera basxa
}
//NOTE: so the root has pointer to its left and right subtree and its left and right subtree has pinter to their left and right subtree and so on
//Huffman Tree is  nested pointers and we use this to generate codes 



//Encoding text into a binary bit string
string HuffmanTree::encode(const string& text) {
    string encoded = "";
    for (char c : text)
        encoded += codes[c];
    return encoded;
}

//Decoding bit string back to original text
string HuffmanTree::decode(const string& bitStr) {
    string decoded = "";
    HuffmanNode* curr = root;
    for (char bit : bitStr) {      // the loop iterates one bit at a time
        curr = (bit == '0') ? curr->left : curr->right;  //if current bit is 0 move to left otherwise move to right
        //checking if we reached a leaf node(actual character)
        if (!curr->left && !curr->right) {
            decoded += curr->data;               //curr = leaf node huda decoded character i.e. curr->data thahunxa
            curr = root; // restart from root
        }
    }
    return decoded; //returns the altogether decoded strings
}

// to print code table in csv (for checking)
void HuffmanTree::printCodeTableCSV() {
    cout << "Char,Code,Bits\n";          // header
    for (auto& p : codes) {
        string ch = (p.first == '\n') ? "\\n" : (p.first == ' ')  ? "SPC" : string(1, p.first);  //string(length,character) is used to convert that char to a string of length 1
        cout << ch << "," << p.second << "," << p.second.size() << "\n";
    }
}
//A wrapper contains original thing to make it easier to use, add extra features, or make it compatible with another system.
// Public wrapper for printTree because outside code cannot access root directly as root node is private
void HuffmanTree::printTree() { 
    printTree(root); 
}

//Save Compressed File
// Format: [table size] [char+freq pairs] [padding count] [packed bytes(compressed data)]
//since have know table size first so char+freq pairs ka samma xa thaha hunxa tespaxi 
//padding count = number of extra zeros added to make last byte complete
//stored in file so decoder knows how many bits at the end are not real data
void saveCompressed(const string& filename, const unordered_map<char, int>& freqTable, const string& encoded)   //freqTable is map:frequency (char → int)
                                                                                                                //encoded is huffman encoded text
{
    //ofstream(output file stream(used for writing files) is a class and out is an object of type ofstream
    ofstream out(filename, ios::binary);  //ios::binary means open the file in binary mode because because we are writing raw bytes
   //ios::binary is just a flag from the ios namespace that disable any automatic text conversions and let you write/read raw bytes exactly as they are in memory
    
   if (!out) {   //checking if file opened successfully
        cerr << "Error: Cannot open file for writing.\n"; 
        return; 
    }

    // Write frequency table size
    int tableSize = freqTable.size();   //freqTable contains character -> frequency
    out.write(reinterpret_cast<char*>(&tableSize), sizeof(int));  //convert (pointer to int i.e &tableSize) to (pointer to char) so write() can write the raw bytes to the file as this memory address is treated as raw bytes instead of an int
    //write(const char* data, streamsize size); it wants pointer to raw bytes (char*) and how many bytes to write 
    //In C++, a char* can access any data type in memory because all data stored in computer memory is ultimately just a sequence of bytes, and a char represents exactly one byte (sizeof(char) = 1).   1byte = 8bit btw
    //When any variable (like an int, float, object, or structure) is converted to a char*, the pointer simply treats that variable’s memory address as raw byte data and reads it one byte at a time without caring about the original data type. 
    //This means char* does not interpret values it only accesses the underlying memory representation directly, which is why it can be used to read, copy, or write any kind of data (such as when saving integers or structures to a binary file using out.write()), since every datatype in C++ is fundamentally stored as bytes in memory.

    // Write each char and its frequency
    for (auto& p : freqTable) {
        out.write(&p.first, sizeof(char));   //p.first vaneko already character ho so type cast garna pardaina
        out.write(reinterpret_cast<const char*>(&p.second), sizeof(int));
    }

    // Write padding count (extra 0 bits to fill the last byte)
    int padding = (8 - encoded.size() % 8) % 8; //encoded.size() % 8 tells us bits left after full bytes and 8 lai minus number of extra 0 bits needed to fill last byte thahahunxa last ma 8% just ensure gareko ho ki tyo number 8 vanda dherai na hos
    out.write(reinterpret_cast<char*>(&padding), sizeof(int));

    // Pack bits into bytes and write
    string padded = encoded + string(padding, '0');
    for (size_t i = 0; i < padded.size(); i += 8) {  //Loop through every 8 bits
        bitset<8> byte(padded.substr(i, 8)); //Take 8-character substring and bitset<8> converts a string like "10101011"
        char c = static_cast<char>(byte.to_ulong());//byte.to_ulong() converts the 8 bits to  a number between 0–255
        out.write(&c, 1);// writes 1 byte to the file
    }
    //Now, string of bits is now stored as actual bytes, not text '0' and '1'.
    out.close();
    cout << "Compressed file saved: " << filename << "\n";
}

//Loading Compressed File
void loadCompressed(const string& filename, unordered_map<char, int>& freqTable, string& encoded)
{
    ifstream in(filename, ios::binary);
    if (!in) {
         cerr << "Error: Cannot open compressed file.\n";
         return;
        }

    // Read frequency table
    int tableSize;
    in.read(reinterpret_cast<char*>(&tableSize), sizeof(int)); //reads first 4 bytes and the pointer moves forward
    for (int i = 0; i < tableSize; i++) {
        char c; 
        int f;
        in.read(&c, sizeof(char));  //reads next 1 byte and the pointer moved forward
        in.read(reinterpret_cast<char*>(&f), sizeof(int)); //reads next 4 bytes and the pointer moves forward
        freqTable[c] = f;
    }  //now after the loop is completed the pointer is exactly at padding count

    // Read padding
    int padding;
    in.read(reinterpret_cast<char*>(&padding), sizeof(int)); //reads next 4 bytes and pointer moves forward

    // Read compresses bytes
    encoded = "";
    char byte;
    while (in.read(&byte, 1))   //reads until file ends 1 byte at a time
    {
        bitset<8> bits(static_cast<unsigned char>(byte));   //converting byte to bits
        encoded += bits.to_string(); //convert bits to string so we reconstruct full Huffman bit stream
        //encoded = "01000001..."
    }

    // Strip padding bits from the end
    if (padding > 0){
        encoded = encoded.substr(0, encoded.size() - padding);  //remove fake zeros added earlier
        //before : 101011000(3 padding bits), after : 101011
    }
    in.close(); //close file(releases file source)
}

//  COMPRESSION(Read file, encode, save .huff)
void compressFile(const string& inputfile, const string& outputfile) 
{
    ifstream in(inputfile);//creates an input file stream object 'in' that establishes a connection between this program and the specified file so it can be read
    //asks OS to open file to read
    //reads a large block of bytes from disk into internal buffer all at once (not one by one directly from disk).
    //internal buffer is RAM memory used for fast reading

    if (!in) {    //if file opening failed
        cerr << "Error: Input file not found.\n"; 
        return;
    }
    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>()); //eads file character by character and stores everything into one string
    in.close();

    if (text.empty()) { 
        cerr << "Error: File is empty.\n"; 
        return; 
    }

    cout << "\n Original file  : " << inputfile;
    cout << "\n Original size  : " << text.size() << " bytes\n";

    HuffmanTree tree;   //creating huffman tree object 
    //build frequency table
    auto freqTable = tree.buildFrequencyTable(text);
    //build huffman tree
    tree.build(freqTable);

    cout << "\nHuffman Code Table CSV:";
    tree.printCodeTableCSV();

    cout << "\nHuffman Tree (rotated 90°):";
    tree.printTree();
    
    //encoding text 
    string encoded = tree.encode(text);

    double origBits = text.size() * 8.0;  //1 byte = 8 bits
    double compBits = encoded.size();  //we didnot multiply combits by 8 because encoded text is already in bits
    //compression ratio
    double ratio    = (1.0 - compBits / origBits) * 100.0;

    cout << "\nCompression Stats:\n";
    cout << "Original bits:" << (int)origBits << "\n";
    cout << "Compressed bits :" << (int)compBits << "\n";
    cout << fixed << setprecision(2);//fixed tells cout to always print numbers in fixed-point decimal format not scientific notation
    //from <iomanip> library ,tells cout how many digits to show after the decimal point when printing floating point numbers.
    cout << "Space saved:" << ratio << "%\n";

    saveCompressed(outputfile, freqTable, encoded);
}

//DECOMPRESSION(Load .huff, decode, save text file)
void decompressFile(const string& inputFile, const string& outputFile) {
    unordered_map<char, int> freqTable;
    string encoded;

    loadCompressed(inputFile, freqTable, encoded); //freqTable and encoded are passed by reference so the function can modify the original variables

    HuffmanTree tree;   //creates a new HuffmanTree object
    tree.build(freqTable);  //uses freqTable to reconstruct the exact same Huffman tree that was used during compression
    string decoded = tree.decode(encoded);  //decoded now contains the full original text exactly as it was before compression

    ofstream out(outputFile); //pen output file for writing
    if (!out) { 
        cerr << "Error: Cannot open output file.\n"; 
        return; 
    }
    out << decoded;  //writes the full original text into outputFile
    out.close();     //closes the file stream, flushing all data to disk.
 
    cout << "\nDecompressed file saved: " << outputFile << "\n";
    cout << "Recovered " << decoded.size() << " characters.\n";
}
