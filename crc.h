#ifndef CRC_H
#define CRC_H

class CRC
{
public:
    typedef unsigned char uint8_t;

    CRC();
    //uint8_t calculate(uint8_t* data, size_t len);

    unsigned short polynom() const;
    void setPolynom(const unsigned short &polynom);

    unsigned short calculate(uint8_t *data, size_t len);
    unsigned short getMask() const;
private:
    unsigned char crc8(unsigned char *pcBlock, unsigned char len);
    unsigned short m_polynom;
};

#endif // CRC_H
