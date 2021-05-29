#ifndef CRC_H
#define CRC_H

class CRC
{
public:
    typedef unsigned char uint8_t;

    CRC();
    uint8_t calculate(uint8_t* data, size_t len);

    unsigned polynom() const;
    void setPolynom(const unsigned &polynom);

private:
    unsigned m_polynom;
};

#endif // CRC_H
