#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <string>
#include <memory>

namespace Huffman
{
    struct TreeNode
    {
        using ptr = std::shared_ptr<TreeNode>;

        TreeNode(char _ch, const size_t& _count) :
            ch(_ch),
            count(_count),
            left(nullptr),
            right(nullptr)
        {}
        char ch;
        size_t count;
        TreeNode::ptr left;
        TreeNode::ptr right;
    };

    using FrequencyTable = std::map<char, size_t>;
    using CodesTable = std::map<char, std::string>;

    FrequencyTable getCharsAndItsCount(const std::string& text);
    TreeNode::ptr generateTree(const FrequencyTable& table);
    CodesTable getCodes(Huffman::TreeNode::ptr root);
    std::string encode(const std::string &text, const CodesTable& table);
    std::string decode(const std::string &text, const CodesTable& table);
    double avgCodeLenght(const std::string& text, const FrequencyTable& freq, const CodesTable& codes);
    double entropy(const std::string &text, const Huffman::FrequencyTable &table);
};

#endif // HUFFMAN_H
