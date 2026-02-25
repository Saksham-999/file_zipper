#include "huffman.hpp"

int main() {

    int choice;
    do {
        cout << "\nMENU:\n";
        cout << "1.Compress a file\n";
        cout << "2.Decompress a file\n";
        cout << "0.Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string inFile, outFile;
                cout << "Enter input file name  : "; cin >> inFile;
                cout << "Enter output file name : "; cin >> outFile;
                compressFile(inFile, outFile);
                break;
            }
            case 2: {
                string inFile, outFile;
                cout << "Enter compressed file  : "; cin >> inFile;
                cout << "Enter output file name : "; cin >> outFile;
                decompressFile(inFile, outFile);
                break;
            }
            case 0:
                cout << "\nExited\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
    return 0;
}
