#include "crc.h"
#include <vector>

CRC::CRC() :
    m_polynom(0x65)
{
}

CRC::uint8_t CRC::calculate(uint8_t *data, size_t len)
{
    uint8_t crc = 0;
    size_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ m_polynom);
            else
                crc <<= 1;
        }
    }
    return crc;
}

unsigned CRC::polynom() const
{
    return m_polynom;
}

void CRC::setPolynom(const unsigned &polynom)
{
    m_polynom = polynom;
}
