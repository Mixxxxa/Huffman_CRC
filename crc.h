#ifndef CRC_H
#define CRC_H

#include <vector>

class CRC
{
public:
    CRC();
    CRC(const unsigned int polynom);

    unsigned int calculate(const std::vector<bool>& data) const;
    unsigned int calculate(const uint8_t *data, size_t len) const;

    unsigned polynomDegree() const;
    unsigned int polynom() const;
    void setPolynom(const unsigned int &polynom);

private:
    unsigned int m_polynom;

    unsigned int getMask() const;
    unsigned int getCheckBitMask() const;
};

#endif // CRC_H
