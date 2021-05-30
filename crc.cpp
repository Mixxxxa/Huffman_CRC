#include "crc.h"
#include <vector>
#include <QDebug>
#include <bitset>

CRC::CRC() :
    m_polynom(0x65)
{
}

//auto getBitset(size_t size)
//{
//    switch (size)
//    {
//    case 1:
//        return std::bitset<1>(false);
//    }
//}

unsigned char CRC::crc8(unsigned char *pcBlock, unsigned char len)
{
    unsigned char crc = 0x00;
    unsigned char i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
        {
            crc = crc & 0x80 ? (crc << 1) ^ m_polynom : crc << 1;
            qDebug() << crc;
        }

    }

    return crc;
}

//CRC::uint8_t CRC::calculate(uint8_t *data, size_t len)
//{
//    //return crc8(data, len);
//    uint8_t crc = 0;
//    size_t i, j;
//    for (i = 0; i < len; i++) {
//        crc ^= data[i];
//        for (j = 0; j < 8; j++) {
//            qDebug() << crc;
//            if ((crc & 0x80) != 0)
//                crc = (uint8_t)((crc << 1) ^ m_polynom);
//            else
//                crc <<= 1;
//            qDebug() << crc;
//        }
//    }
//    return crc;
//}

#include "crchelper.h"

unsigned short CRC::getMask() const
{
    unsigned short mask = 0;
    const int digits = CRCHelper::findFirstSignificant(m_polynom);
    for(int i = 0; i < digits; ++i)
    {
        mask |= (1 << i);
    }
    return mask;
}

unsigned short CRC::calculate(uint8_t *data, size_t len)
{
    unsigned short crc = 0;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
            {
                crc = (unsigned short)((crc << 1) ^ m_polynom);
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    crc &= getMask();
    return crc;
}

unsigned short CRC::polynom() const
{
    return m_polynom;
}

void CRC::setPolynom(const unsigned short &polynom)
{
    m_polynom = polynom;
}
