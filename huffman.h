#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <string>
#include <memory>

class Huffman
{
public:
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

    Huffman();
    FrequencyTable getCharsAndItsCount(const std::string& text) const;
    TreeNode::ptr generateTree(const FrequencyTable& table) const;
    std::map<char, std::string> getCodes(Huffman::TreeNode::ptr root) const;

private:
};

#endif // HUFFMAN_H
