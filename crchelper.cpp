// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "crchelper.h"
#include <stdexcept>
#include <QRegularExpression>

unsigned int CRCHelper::tryParsePolynom(const QString &text)
{
    QString prepared = text.simplified().remove(' ');
    if(prepared.isEmpty())
        throw std::runtime_error("Пустой полином");

    unsigned int polynom = 0;

    int matchedCount = 0;
    bool converted = false;
    bool zeroDegreeFound = false;

    QRegularExpression polynomDegreePart("x\\^(\\d{1,2})", QRegularExpression::CaseInsensitiveOption);
    auto matched = polynomDegreePart.globalMatch(prepared);
    for(; matched.hasNext(); ++matchedCount)
    {
        const auto i = matched.next().captured(1);
        unsigned degree = i.toUInt(&converted, 10);
        if(!converted)
            throw std::runtime_error(std::string("Ошибка полинома в \"" + i.toStdString() + "\""));
        polynom |= (1 << degree);
        if(degree == 0)
            zeroDegreeFound = true;
    }
    if(matchedCount != prepared.count('x', Qt::CaseInsensitive))
        throw std::runtime_error("Не все части полинома удалось распознать");

    const bool hasPlusOneAtTheEnd = prepared.endsWith("+1");
    if(hasPlusOneAtTheEnd)
    {
        if(zeroDegreeFound)
            throw std::runtime_error("Ошибка полинома");
        else
            polynom |= 1;
    }

    return polynom;
}

std::vector<uint8_t> CRCHelper::tryParceByteArray(const QString& text)
{
    if(text.isEmpty())
        throw std::runtime_error("Поле исходного текста пусто");

    std::vector<uint8_t> result;

    QRegularExpression isHex("^(0x)?[0-9a-f]{2}$", QRegularExpression::CaseInsensitiveOption);
    bool parseOk = false;
    uint8_t temp = 0;

    auto tokens = text.split(' ');
    for(const auto& i : tokens)
    {
        if(i.contains(isHex))
        {
            temp = i.toUInt(&parseOk, 16);
            if(parseOk)
            {
                result.push_back(temp);
                continue;
            }
        }
        throw std::runtime_error(std::string("Ошибка разбора \"" + i.toStdString() + "\""));
    }
    return result;
}
