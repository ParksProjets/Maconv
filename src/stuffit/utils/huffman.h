/*

Huffman decoder.

The code in this file is based on TheUnarchiver.
See README.md and docs/licenses/TheUnarchiver.txt for more information.

Copyright (C) 2019, Guillaume Gonnet

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#pragma once

#include "utils/bit_reader.h"

#include <vector>
#include <memory>

namespace maconv {
namespace stuffit {


// A node in the Huffman tree.
struct HuffmanTreeNode {
    int branches[2];
};

struct HuffmanTableEntry {
    uint32_t length;
    int32_t value;
};



// Huffman code decoder.
struct HuffmanDecoder {

    // Copy an Huffman decoder.
    HuffmanDecoder &operator=(const HuffmanDecoder &);

    // Initialize the decoder.
    void Initialize();
    void Initialize(const int *lengths, int numsymbols, int maxcodelength, bool zeros);


    // Add a new value in the decoder (high bit first).
    void AddValue(int value, uint32_t code, int length);
    void AddValue(int value, uint32_t code, int length, int repeat_pos);

    // Add a new value in the decoder (low bit first).
    void AddValueLF(int value, uint32_t code, int length);
    void AddValueLF(int value, uint32_t code, int length, int repeat_pos);


    // Get the next symbol from a bit reader.
    int NextSymbol(utils::BitReader &input);

    // Make the search table.
    void MakeTable(bool is_LE);

protected:

    // Get a pointer on a node.
    HuffmanTreeNode *NodePtr(int node) { return &tree[node]; }

    // Get/set node ID on branch |bit| of |node|.
    int Branch(int node, int bit) { return NodePtr(node)->branches[bit]; }
    void SetBranch(int node, int bit, int next) { NodePtr(node)->branches[bit] = next; }

    // Get/set left branch of |node|.
    int LeftBranch(int node) { return Branch(node, 0); }
    void SetLeftBranch(int node, int next) { SetBranch(node, 0, next); }

    // Get/set right branch of |node|.
    int RightBranch(int node) { return Branch(node, 1); }
    void SetRightBranch(int node, int next) { SetBranch(node, 1, next); }

    // Get/set leaf value.
    int LeafValue(int node) { return LeftBranch(node); }
    void SetLeafValue(int node, int v) { SetLeftBranch(node, v); SetRightBranch(node, v); }

    // Set/get whether a node is empty or not.
    void SetEmptyNode(int node) { SetLeftBranch(node, -1); SetRightBranch(node, -2); }
    bool IsEmptyNode(int node) { return LeftBranch(node) == -1 && RightBranch(node) == -2; }

    // Get information on a node.
    bool IsInvalidNode(int node) { return node < 0; }
    bool IsOpenBranch(int node, int bit) { return IsInvalidNode(Branch(node, bit)); }
    bool IsLeafNode(int node) { return LeftBranch(node) == RightBranch(node); }

    // Create a new node.
    int NewNode() { tree.push_back({0}); SetEmptyNode(tree.size()-1);
        return tree.size()-1; }


    // Make search table.
    void MakeTableRecursLE(int node, HuffmanTableEntry *table, int depth);
    void MakeTableRecursBE(int node, HuffmanTableEntry *table, int depth);


    std::vector<HuffmanTreeNode> tree;
    int num_entries, min_length, max_length;

    int table_size;
    std::unique_ptr<HuffmanTableEntry[]> table;
};


} // namespace stuffit
} // namespace maconv
