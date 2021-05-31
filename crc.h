#ifndef CRC_H
#define CRC_H

#include <vector>

class CRC
{
public:
    CRC();

    unsigned short calculate(const std::vector<bool>& data) const;
    unsigned short calculate(uint8_t *data, size_t len) const;

    unsigned polynomDegree() const;
    unsigned short polynom() const;
    void setPolynom(const unsigned short &polynom);

private:
    unsigned short m_polynom;

    unsigned short getMask() const;
    unsigned short getCheckBitMask() const;
};

#endif // CRC_H
