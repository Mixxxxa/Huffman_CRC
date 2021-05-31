#include "huffman.h"
#include <queue>
#include <functional>
#include <cmath>
#include <stdexcept>

static constexpr char NullValue = 0;

Huffman::Huffman()
{ }

// Возвращает словарь со знаками и их количеством в тексте
Huffman::FrequencyTable Huffman::getCharsAndItsCount(const std::string &text) const
{
    std::map<char, size_t> result;
    for(const auto& ch : text)
        result[ch] += 1;
    return result;
}

// Создает дерево для кода Хаффмана
Huffman::TreeNode::ptr Huffman::generateTree(const Huffman::FrequencyTable &table) const
{
    // Компаратор для сравнения нод
    auto comp = [](const TreeNode::ptr& l, const TreeNode::ptr& r) -> bool
    {
        return (l->count > r->count);
    };

    // priority_queue потому что сама сортирует
    std::priority_queue<TreeNode::ptr, std::vector<TreeNode::ptr>, decltype (comp) > collection(comp);
    // Создаем ноды для каждого символа и отправляем в очередь
    for(const auto& entry : table)
        collection.push(std::make_shared<TreeNode>(entry.first, entry.second));

    // До тех пор, пока в очереди не останется только один элемент...
    while(collection.size() != 1)
    {
        // Забираем два самых малых элемента из очереди
        TreeNode::ptr left = collection.top();
        collection.pop();
        TreeNode::ptr right = collection.top();
        collection.pop();

        // Создаем новый элемент со значением, равным сумме двух предыдущих,
        // а их записываем как листья нового элемента
        auto top = std::make_shared<TreeNode>(NullValue, left->count+right->count);
        top->left  = left;
        top->right = right;
        // Отправляем новый элемент в очередь
        collection.push(top);
    }
    // Возвращаем корень дерева
    return collection.top();
}

// Возвращает словарь со знаками и их кодами
Huffman::CodesTable Huffman::getCodes(Huffman::TreeNode::ptr root) const
{
    if(!root)
        throw std::runtime_error("Ошибка генерации кодов");

    std::map<char, std::string> result;

    // Рекурсивная лямбда для поиска вглубь
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

// Кодирование
std::string Huffman::encode(const std::string& text, const Huffman::CodesTable &table) const
{
    std::string result;
    for(const auto& ch : text)
        result += table.at(ch);
    return result;
}

// Декодирование
std::string Huffman::decode(const std::string &text, const Huffman::CodesTable &table) const
{
    std::string encodedText = text;
    std::string decodedText;
    std::string buffer;

    while (!encodedText.empty())
    {
        buffer += encodedText.at(0);
        // Не самое лучшее решение, но по-быстрому пойдет...
        encodedText.erase(encodedText.begin());
        for(const auto& [key, value] : table)
        {
            if(value == buffer)
            {
                decodedText += key;
                buffer.clear();
                break;
            }
        }
    }
    return decodedText;
}

// Возвращает среднюю длину кода
double Huffman::avgCodeLenght(const std::string &text, const Huffman::FrequencyTable &freq, const Huffman::CodesTable &codes) const
{
    const double textLength = static_cast<double>(text.size());

    double avgLength = 0;
    for(const auto& [ch, count] : freq)
    {
        const double p = static_cast<double>(count) / textLength;
        const double codeSize = static_cast<double>(codes.at(ch).size());
        avgLength += p * codeSize;
    }
    return avgLength;
}

// Возвращает энтропию кода
double Huffman::entropy(const std::string &text, const Huffman::FrequencyTable &table) const
{
    const double textLength = static_cast<double>(text.size());
    double ret = 0;

    for(const auto& [ch, count] : table)
    {
        const double p = static_cast<double>(count) / textLength;
        ret += p * log2(p);
    }
    return abs(ret);
}
