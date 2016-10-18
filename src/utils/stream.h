#pragma once

#include <stdint.h>

struct Reader {
    Reader(const uint8_t* data, size_t remaining)
        : data(data)
        , remaining(remaining)
        , error(false)
    {}

    void skip(unsigned offset) {
        if (remaining < offset) {
            error = true;
            return;
        }
        data += offset;
        remaining -= offset;
    }

    uint8_t get_u8() {
        if (remaining == 0) {
            error = true;
            return 0;
        }

        uint8_t res = data[0];
        data += 1;
        remaining -= 1;
        return res;
    }

    uint16_t get_u16() {
        if (remaining <= 1) {
            error = true;
            return 0;
        }

        uint16_t res;
        memcpy(&res, data, 2);
        data += 2;
        remaining -= 2;
        return res;
    }

    uint32_t get_u32() {
        if (remaining <= 3) {
            error = true;
            return 0;
        }

        uint32_t res;
        memcpy(&res, data, 4);
        data += 4;
        remaining -= 4;
        return res;
    }

    const uint8_t* data;
    size_t remaining;
    bool error;
};

struct Writer {
    Writer(uint8_t* data, size_t remaining)
        : data(data)
        , remaining(remaining)
        , error(false)
        , len(0)
    {}

    uint8_t* reserve(unsigned int reservation) {
        if (remaining < reservation) {
            error = true;
            return nullptr;
        }

        uint8_t* tmp = data;
        data += reservation;
        len += reservation;
        remaining -= reservation;
        return tmp;
    }

    void put_u8(uint8_t val) {
        if (remaining == 0) {
            error = true;
            return;
        }

        data[0] = val;
        data += 1;
        len += 1;
        remaining -= 1;
        return;
    }

    void put_u16(uint16_t val) {
        if (remaining <= 1) {
            error = true;
            return;
        }

        uint16_t* tmp = (uint16_t* ) data;
        memcpy(tmp, &val, sizeof(val));
        data += 2;
        len += 2;
        remaining -= 2;
        return;
    }

    void put_u32(uint32_t val) {
        if (remaining <= 3) {
            error = true;
            return;
        }

        memcpy(data, &val, sizeof(val));
        data += 4;
        len += 4;
        remaining -= 4;
        return;
    }

    uint8_t* data;
    size_t remaining;
    bool error;
    int len;
};
