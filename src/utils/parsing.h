#pragma once

#include <string>


static inline char from_bcd(uint8_t item) {
    if (item < 0x0A)
        return '0' + item;
    return 'A' + (item - 0x0A);
}

static inline uint8_t to_bcd(char chr) {
    if (chr <= '9')
        return chr - '0';
    return (chr - 'A') + 0x0A;
}

static inline std::string parse_imsi(const uint8_t *imsi, const int len = 8) {
    std::string result;
    result.reserve(len * 2);

    for (int i = 0; i < len; ++i) {
        uint8_t item = imsi[i];
        if ((item & 0x0F) != 0x0F)
            result.push_back(from_bcd(item & 0x0F));
        item >>= 4;
        if (item != 0x0F)
            result.push_back(from_bcd(item));
    }

    return result;
}

static inline void encode_imsi(uint8_t *data, const std::string& imsi, unsigned int enc_len = 8) {
    for (unsigned i = 0; i < enc_len; ++i) {
        uint8_t nibbles = 0;
        if (imsi.length() > i * 2)
            nibbles |= to_bcd(imsi.at(i * 2));
        else
            nibbles |= 0x0F;
        if (imsi.length() > i * 2 + 1)
            nibbles |= to_bcd(imsi.at(i * 2 + 1)) << 4;
        else
            nibbles |= 0xF0;
        data[i] = nibbles;
    }
}

static inline std::string parse_msisdn(const uint8_t *msisdn, const uint16_t len, const unsigned offset = 1) {
    /* We don't care about the numbering plan and such */
    std::string result;
    result.reserve(len * 2);
    for (unsigned i = offset; i < len; ++i) {
        uint8_t item = msisdn[i];
        result.push_back(from_bcd(item & 0x0F));
        item >>= 4;
        if (item != 0x0F)
            result.push_back(from_bcd(item));
    }
    return result;
}

static inline int imsi_length(const std::string& imsi) {
    return (imsi.length() + 1) / 2;
}

static inline int msisdn_length(const std::string& msisdn) {
    return (msisdn.length() + 1) / 2;
}

static inline int encode_msisdn(uint8_t *data, const std::string& msisdn, bool write_tonnpi = true) {
    int written;

    if (write_tonnpi) {
        written = 1;
        data[0] = 0x91;
        data += 1;
    } else {
        written = 0;
    }

    int msisdn_len = msisdn_length(msisdn);
    for (int i = 0; i < msisdn_len; ++i) {
        uint8_t nibbles = 0;
        if (msisdn.length() > i * 2u)
            nibbles |= to_bcd(msisdn.at(i * 2));
        else
            nibbles |= 0x0F;
        if (msisdn.length() > i * 2u + 1)
            nibbles |= to_bcd(msisdn.at(i * 2 + 1)) << 4;
        else
            nibbles |= 0xF0;
        data[i] = nibbles;
        written += 1;
    }
    return written;
}
