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
        codes[node->data] = code;            //node->data is the character stored in the leaf node  
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
    return freq;
}

