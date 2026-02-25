# Huffman File Compressor

A file compression and decompression tool implemented in C++ using the Huffman coding algorithm.

## How It Works

Huffman coding is a greedy algorithm that assigns shorter binary codes to more frequent characters and longer codes to less frequent ones. This reduces the total number of bits needed to represent a file.

The process works in the following steps:

1. **Frequency Analysis** — Count how often each character appears in the input file.
2. **Tree Construction** — Build a binary tree using a min-heap (priority queue), repeatedly merging the two lowest-frequency nodes.
3. **Code Generation** — Traverse the tree to assign a unique binary code to each character (left edge = `0`, right edge = `1`).
4. **Encoding** — Replace each character in the file with its binary code and pack the bits into bytes.
5. **Decoding** — Rebuild the same tree from the saved frequency table and traverse it bit-by-bit to recover the original text.

## File Structure

```
├── huffman.hpp   # Class and function declarations
├── huffman.cpp   # Full implementation (tree, encode, decode, file I/O)
└── main.cpp      # Menu-driven entry point
```

## Compressed File Format

The `.huff` output file stores:
- Frequency table size (int)
- Character + frequency pairs (char + int each)
- Padding bit count (int)
- Packed binary data (bytes)

The frequency table is embedded in the file so the tree can be reconstructed exactly during decompression — no separate metadata file needed.

## Build & Run

```bash
g++ -o huffman main.cpp huffman.cpp
./huffman
```

Then follow the menu:

```
MENU:
1. Compress a file
2. Decompress a file
0. Exit
```

## Example

```
Enter input file name  : sample.txt
Enter output file name : sample.huff

Original size  : 1024 bytes
Compressed bits: 4821
Space saved: 41.20%
```

## Requirements

- C++11 or later
- Standard library only (no external dependencies)
