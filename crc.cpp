// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "crc.h"
#include <QDebug>

CRC::CRC() :
    m_polynom(0x65)
{ }

CRC::CRC(const unsigned int polynom) :
    m_polynom(polynom)
{ }

unsigned int CRC::calculate(const std::vector<bool> &data) const
{
    // Маска для указания на проверочный бит
    const auto calcCheckBit = getCheckBitMask();
    // Битовая маска для отсечения лишних данных
    const auto mask = getMask();
    // Степень полинома для корректного дебаг-вывода
    const unsigned degree = polynomDegree();

    unsigned int crc = 0;
    // Для каждого бита в данных...
    for(size_t i = 0; i < data.size(); ++i)
    {
        // Получаем новый младший бит
        if(data[i])
            crc |= 1;
        // Если набралось достаточно бит...
        if ((crc & calcCheckBit) != 0)
        {
            // Делим их на полином
            crc ^= m_polynom;
            qDebug().noquote() << Qt::bin << QString::number(crc, 2).rightJustified(degree+1, '0');
        }
        // Если бит не последний, то сдвигаем CRC
        if(i != data.size() - 1)
            crc <<= 1;
    }
    qDebug().noquote() << Qt::bin << QString::number(crc, 2).rightJustified(degree+1, '0');
    //Отсекаем лишние данные
    crc &= mask;
    return crc;
}

// Возвращает битовую маску для отсечения лишних данных
unsigned int CRC::getMask() const
{
    unsigned int mask = 0;
    const int digits = polynomDegree();
    for(int i = 0; i < digits + 1; ++i)
        mask |= (1 << i);
    return mask;
}

// Возвращает проверочную маску для текущего полинома
unsigned int CRC::getCheckBitMask() const
{
    const int degree = polynomDegree();
    return (1u << degree);
}

// Возвращает степень полинома
unsigned CRC::polynomDegree() const
{
    if (!m_polynom)
        return 0;

    // Ищем старший бит и возвращаем его положение - 1
    unsigned poly = m_polynom;
    unsigned ret = 1;
    while (poly >>= 1)
        ret += 1;
    return ret - 1;
}

unsigned int CRC::calculate(const uint8_t *data, size_t len) const
{
    // Получаем степень полинома
    const size_t degree = static_cast<size_t>(polynomDegree());
    // Конвертируем массив байтов в массив битов
    std::vector<bool> converted;
    // Резервируем место под требуемое кол-во бит + W-нулей
    converted.reserve(len * CHAR_BIT + degree);
    // Переносим данные из байт
    for (size_t i = 0; i < len; i++)
        for(int j = CHAR_BIT - 1; j >= 0; --j)
            converted.push_back(data[i] & (1 << j));
    // Дополнить данные нулями в конце
    for(size_t i = 0; i < degree; ++i)
        converted.push_back(false);
    // Считаем CRC для массива битов
    return calculate(converted);
}

unsigned int CRC::polynom() const
{
    return m_polynom;
}

void CRC::setPolynom(const unsigned int &polynom)
{
    m_polynom = polynom;
}
