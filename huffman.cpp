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
void HuffmanTree::printTree(HuffmanNode* node, int space, int gap) {  //gap controls width between levels
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

