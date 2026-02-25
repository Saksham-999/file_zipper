// ============================================================
//  main.cpp — Entry Point & Menu
//  FILE COMPRESSION & DECOMPRESSION USING HUFFMAN CODING
//  DSA Project | C++
// ============================================================

#include "huffman.hpp"

int main() {
    cout << "╔══════════════════════════════════════════════════╗\n";
    cout << "║   FILE COMPRESSION USING HUFFMAN CODING (C++)    ║\n";
    cout << "║   DSA Project                                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    int choice;
    do {
        cout << "\n📋 MENU:\n";
        cout << "  1. Compress a file\n";
        cout << "  2. Decompress a file\n";
        cout << "  3. Run Demo (no files needed)\n";
        cout << "  0. Exit\n";
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
            case 3:
                runDemo();
                break;
            case 0:
                cout << "\nGoodbye! 👋\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
