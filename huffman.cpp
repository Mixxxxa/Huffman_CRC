#include "huffman.h"
#include <functional>

Huffman::Huffman()
{

}

void Huffman::fillTable(const QString &text)
{
//    using namespace std::placeholders;

//    auto existInTable = std::bind(std::find_if, m_codeTable.cbegin(), m_codeTable.cend(), [](const CodeTableEntry& entry)
//    {
//        return entry.character == ch;
//    });

//    auto existInTable = [&](const char& ch)
//    {
//        auto it = std::find_if(m_codeTable.cbegin(), m_codeTable.cend(), [ch](const CodeTableEntry& entry)
//        {
//            return entry.character == ch;
//        });
//    };

//    for(const auto& ch : text)
//    {
//        auto it = std::find_if(m_codeTable.cbegin(), m_codeTable.cend(), [ch](const CodeTableEntry& entry)
//        {
//            return entry.character == ch;
//        });
//    }
}
