// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "huffman.h"
#include <queue>
#include <functional>
#include <cmath>
#include <stdexcept>

static constexpr char NullValue = 0;

// Возвращает словарь со знаками и их количеством в тексте
Huffman::FrequencyTable Huffman::getCharsAndItsCount(const std::string &text)
{
    std::map<char, size_t> result;
    for(const auto& ch : text)
        result[ch] += 1;
    return result;
}

// Создает дерево для кода Хаффмана
Huffman::TreeNode::ptr Huffman::generateTree(const Huffman::FrequencyTable &table)
{
    using namespace Huffman;
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
Huffman::CodesTable Huffman::getCodes(Huffman::TreeNode::ptr root)
{
    using namespace Huffman;
    // Ошибка, если корень не существует
    if(!root)
        throw std::runtime_error("Ошибка генерации кодов");

    std::map<char, std::string> result;

    // Рекурсивная лямбда для поиска вглубь
    std::function<void(TreeNode::ptr, std::string)> getCode;
    getCode = [&](TreeNode::ptr root, std::string str)
    {
        if(!root)
            return;

        // Если в текущей ноде есть символ
        if(root->ch != NullValue)
        {
            // Добавляем его в результат
            result[root->ch] = str;
            return;
        }
        else
        {
            // Продолжаем поиск
            getCode(root->left, str + "0");
            getCode(root->right, str + "1");
        }
    };
    getCode(root, "");
    return result;
}

// Кодирование
std::string Huffman::encode(const std::string& text, const Huffman::CodesTable &table)
{
    std::string result;
    try
    {
        for(const auto& ch : text)
            result += table.at(ch);
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("Ошибка кодирования");
    }
    return result;
}

// Декодирование
std::string Huffman::decode(const std::string &text, const Huffman::CodesTable &table)
{
    std::string encodedText = text;
    std::string decodedText;
    std::string buffer;

    // Пока не обработали все символы закодированного текста...
    for(const auto& ch : encodedText)
    {
        // Получаем новый символ
        buffer += ch;
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
    // Если все символы обработаны, но в буфере остались символы,
    // то считаем что произошла ошибка
    if(!buffer.empty())
        throw std::runtime_error("Ошибка декодирования");

    return decodedText;
}

// Возвращает среднюю длину кода
double Huffman::avgCodeLenght(const std::string &text, const Huffman::FrequencyTable &freq, const Huffman::CodesTable &codes)
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
double Huffman::entropy(const std::string &text, const Huffman::FrequencyTable &table)
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
