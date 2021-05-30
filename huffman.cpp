#include "huffman.h"
#include <functional>
#include <queue>
#include <QDebug>
#include <iostream>
#include <functional>

static constexpr char NullValue = 0;

Huffman::Huffman()
{

}

Huffman::FrequencyTable Huffman::getCharsAndItsCount(const std::string &text) const
{
    std::map<char, size_t> result;
    for(const auto& ch : text)
        result[ch] += 1;
    return result;
}

Huffman::TreeNode::ptr Huffman::generateTree(const Huffman::FrequencyTable &table) const
{
    auto comp = [](const TreeNode::ptr& l, const TreeNode::ptr& r) -> bool
    {
        return (l->count > r->count);
    };

    std::priority_queue<TreeNode::ptr, std::vector<TreeNode::ptr>, decltype (comp) > collection(comp);
    for(const auto& entry : table)
    {
        collection.push(std::make_shared<TreeNode>(entry.first, entry.second));
    }

    while(collection.size() != 1)
    {
        TreeNode::ptr left = collection.top();
        collection.pop();
        TreeNode::ptr right = collection.top();
        collection.pop();

        auto top = std::make_shared<TreeNode>(NullValue, left->count+right->count);
        top->left  = left;
        top->right = right;
        collection.push(top);
    }
    return collection.top();
}

std::map<char, std::string> Huffman::getCodes(Huffman::TreeNode::ptr root) const
{
    if(!root)
        throw std::runtime_error("Ошибка генерации кодов");

    std::map<char, std::string> result;

    std::function<void(TreeNode::ptr, std::string)> getCode;
    getCode = [&](Huffman::TreeNode::ptr root, std::string str)
    {
        if(!root)
            return;

        if(root->ch != NullValue)
        {
            result[root->ch] = str;
            return;
        }
        else
        {
            getCode(root->left, str + "0");
            getCode(root->right, str + "1");
        }
    };
    getCode(root, "");
    return result;
}
