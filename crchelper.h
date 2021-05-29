#ifndef CRCHELPER_H
#define CRCHELPER_H

#include <vector>
#include <QString>

class CRCHelper
{
public:
    CRCHelper() = delete;
    static unsigned int tryParsePolynom(const QString& text);
    static std::vector<uint8_t> tryParceByteArray(const QString &text);
};

#endif // CRCHELPER_H
