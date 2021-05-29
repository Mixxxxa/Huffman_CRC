#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <utility>
#include <QString>

class Huffman
{
public:
    struct CodeTableEntry
    {
        char character;
        unsigned count;
    };

    Huffman();
    void fillTable(const QString& text);

private:
    std::vector<CodeTableEntry> m_codeTable;
};

#endif // HUFFMAN_H
