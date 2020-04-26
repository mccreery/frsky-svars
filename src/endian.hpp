#pragma once

namespace smartport {

/**
 * @brief Copies an unsigned integer to a buffer using big-endian (MSB first).
 *
 * @param buf A buffer of at least sizeof(UInt) bytes.
 * @param x The value to write.
 */
template <typename UInt>
static void put_big_endian(char* const buf, const UInt x) {
    for (size_t i = 0; i < sizeof(UInt); i++) {
        unsigned shift = (sizeof(UInt) - 1 - i) * 8;
        buf[i] = (char)(x >> shift);
    }
}

/**
 * @brief Reads a big-endian (MSB first) unsigned integer from a buffer.
 *
 * @param buf A buffer of at least size bytes.
 * @param size The lesser of remaining bytes in the buffer or sizeof(UInt).
 * @return UInt The value read from the buffer.
 */
template <typename UInt>
static UInt get_big_endian(const char* const buf, unsigned size = sizeof(UInt)) {
    UInt x = 0;

    for (size_t i = 0; i < size; i++) {
        unsigned shift = (sizeof(UInt) - 1 - i) * 8;
        x |= buf[i] << shift;
    }
    return x;
}

}
