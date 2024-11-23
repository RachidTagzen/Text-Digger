// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// The MurmurHash3 algorithm was created by Austin Appleby and is in the public domain.
// This enhanced version was authored by Rachid Tagzen and is also released into the public
// domain.
// The author hereby disclaims any copyright to this source code.
//
// This enhanced version of MurmurHash3 incorporates modern C++17 features
// for improved performance and safety, making it suitable for producing
// consistent hash values across platforms.
//
// Key enhancements include:
// 1. **Code Structure and Readability**:
//    - Utilizes modern C++ features (constexpr, std::byte, std::endian)
//      for better readability and maintainability.
//    - Clearer type casting with std::to_integer enhances safety against
//      potential type conversion issues.
//
// 2. **Endianness Handling**:
//    - Implements checks for endianness using std::endian,
//      improving portability across different platforms, unlike the original version
//      which assumes little-endian by default.
//
// 3. **Use of constexpr**:
//    - Functions are declared as constexpr, enabling
//      compile-time evaluation and improved optimization by the compiler.
//
// 4. **Code Efficiency**:
//    - Reduced branching through the use of [[fallthrough]]
//      in switch-case statements, resulting in cleaner and more efficient execution.
//
// 5. **Memory Usage**:
//    - Adopts std::byte for data manipulation, streamlining
//      memory handling and improving efficiency compared to the original version’s
//      overhead from macros and redundant type definitions.
//
// Note: The x86 and x64 versions yield different results as they are optimized
// for their respective architectures. While you can compile and run any version
// on any platform, performance may be suboptimal for non-native architectures.
//
// For future updates to this file, see [.....].
//
// Release Date: 2024/10/17 (22:33)
//
// Licensing: This code is released into the public domain. You are free to use, modify,
// and distribute it without any restrictions. For more information on dedicating works
// to the public domain, see Creative Commons Zero (CC0) or equivalent.
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------

#pragma once

#include <span>
#include <cstdint>
#include <bit>
#include <cstring>



// *********************************************************************************************************************
// *********************************************************************************************************************
// *********************************************************************************************************************

//-----------------------------------------------------------------------------
// Rotate Left (ROTL) functions for 32-bit and 64-bit unsigned integers.
//
// These functions perform bitwise circular shifts (rotate left) on the input
// integer `x` by `r` positions. The left shift (`x << r`) moves the bits of `x`
// to the left, and the overflowed bits are wrapped around to the right side
// using a right shift (`x >> (32 - r)` for 32-bit and `x >> (64 - r)` for 64-bit).
//
// - `ROTL32`: For 32-bit integers, rotates the bits of `x` left by `r` positions.
// - `ROTL64`: For 64-bit integers, rotates the bits of `x` left by `r` positions.
//
// These operations are often used in hashing algorithms or cryptographic
// functions to ensure a good bit-mixing property and are constexpr for
// potential compile-time evaluation.
//-----------------------------------------------------------------------------
static constexpr std::uint32_t ROTL32(std::uint32_t x, int r) {
    return (x << r) | (x >> (32 - r));
}

static constexpr std::uint64_t ROTL64(std::uint64_t x, int r) {
    return (x << r) | (x >> (64 - r));
}



//-----------------------------------------------------------------------------
// Block read functions for 32-bit and 64-bit unsigned integers.
//
// These functions are responsible for reading a block of data from an array
// of 32-bit or 64-bit unsigned integers. They simply return the value at the
// `i`-th index from the `blocks` array.
//
// - `getblock32_`: Reads a 32-bit block from an array of `uint32_t`.
// - `getblock64_`: Reads a 64-bit block from an array of `uint64_t`.
//
// Notes:
// - If your platform has specific requirements for memory access, such as
//   handling unaligned reads or endian-swapping, you should perform those
//   operations here.
// - These are `constexpr` functions, meaning they can be evaluated at compile-time
//   when used in a constant expression.
//
// This pattern is common in hash functions or data manipulation code where
// reading blocks of data at fixed intervals is required.
//-----------------------------------------------------------------------------
constexpr uint32_t getblock32(const uint32_t* blocks, int i) {
    return blocks[i];
}

constexpr uint64_t getblock64(const uint64_t* blocks, int i) {
    return blocks[i];
}



//-----------------------------------------------------------------------------
// Finalization mix functions for 32-bit and 64-bit unsigned integers.
//
// These functions perform a final mixing step to force the avalanche effect,
// ensuring that even small differences in input data will result in significantly
// different output hashes. The avalanche effect is crucial for hash functions to
// minimize collisions and ensure a good distribution of hash values.
//
// - `fmix32_`: Finalizes a 32-bit hash (`h`) by applying a series of bitwise
//   XOR shifts and multiplications with prime constants to mix the bits.
//   This process ensures that all bits of `h` influence each other.
//
//   Steps:
//   1. XOR `h` with itself shifted right by 16 bits.
//   2. Multiply `h` by the prime constant `0x85ebca6b`.
//   3. XOR `h` with itself shifted right by 13 bits.
//   4. Multiply `h` by another prime constant `0xc2b2ae35`.
//   5. XOR `h` with itself shifted right by 16 bits.
//   6. Return the mixed result.
//
// - `fmix64_`: Finalizes a 64-bit hash (`k`) with similar XOR shifts and
//   multiplications, but using larger prime constants for 64-bit values.
//
//   Steps:
//   1. XOR `k` with itself shifted right by 33 bits.
//   2. Multiply `k` by the prime constant `magic1` (0xff51afd7ed558ccd).
//   3. XOR `k` with itself shifted right by 33 bits.
//   4. Multiply `k` by another prime constant `magic2` (0xc4ceb9fe1a85ec53).
//   5. XOR `k` with itself shifted right by 33 bits.
//   6. Return the final mixed result.
//
// These functions are typically the last step in hashing algorithms, such as
// MurmurHash, where it's critical to produce a well-mixed, unpredictable hash
// value from the intermediate hash state.
//-----------------------------------------------------------------------------
constexpr uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

constexpr uint64_t fmix64(uint64_t k) {
    constexpr uint64_t magic1 = 0xff51afd7ed558ccd;
    constexpr uint64_t magic2 = 0xc4ceb9fe1a85ec53;

    k ^= k >> 33;
    k *= magic1;
    k ^= k >> 33;
    k *= magic2;
    k ^= k >> 33;
    return k;
}



// *********************************************************************************************************************
// *********************************************************************************************************************
// *********************************************************************************************************************
class MurmurHash3 {

public:
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    static void MurmurHash3_x86_32(const void* key, int len, std::uint32_t seed, void* out) {

        const auto data = static_cast<const std::byte*>(key);
        const int nblocks = len / 4;

        std::uint32_t h1 = seed;

        constexpr std::uint32_t c1 = 0xcc9e2d51;
        constexpr std::uint32_t c2 = 0x1b873593;

        // ------------------
        // Body
        // ------------------
        const auto blocks = reinterpret_cast<const std::uint32_t*>(data);

        for (int i = 0; i < nblocks; ++i) {
            std::uint32_t k1 = getblock32(blocks, i);

            // Convert endianness if necessary
            if constexpr (std::endian::native != std::endian::little)
                k1 = __builtin_bswap32(k1); // Swap bytes for big-endian


            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        // ------------------
        // Tail
        // ------------------
        const auto tail = data + nblocks * 4;

        std::uint32_t k1 = 0;

        switch (len & 3) {
        case 3: k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[2])) << 16; [[fallthrough]];
        case 2: k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[1])) << 8; [[fallthrough]];
        case 1: k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[0]));
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
            break;
        }

        // ------------------
        // Finalization
        // ------------------
        h1 ^= len;
        h1 = fmix32(h1);

        *reinterpret_cast<std::uint32_t*>(out) = h1;
    }


    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    static void MurmurHash3_x86_128(const void* key, int len, std::uint32_t seed, void* out) {
        const auto data = static_cast<const std::byte*>(key);
        const int nblocks = len / 16;

        std::uint32_t h1 = seed;
        std::uint32_t h2 = seed;
        std::uint32_t h3 = seed;
        std::uint32_t h4 = seed;

        constexpr std::uint32_t c1 = 0x239b961b;
        constexpr std::uint32_t c2 = 0xab0e9789;
        constexpr std::uint32_t c3 = 0x38b34ae5;
        constexpr std::uint32_t c4 = 0xa1e38b93;

        // ------------------
        // Body
        // ------------------
        const auto blocks = reinterpret_cast<const std::uint32_t*>(data);

        for (int i = 0; i < nblocks; ++i) {
            std::uint32_t k1 = getblock32(blocks, i * 4 + 0);
            std::uint32_t k2 = getblock32(blocks, i * 4 + 1);
            std::uint32_t k3 = getblock32(blocks, i * 4 + 2);
            std::uint32_t k4 = getblock32(blocks, i * 4 + 3);

            // Convert endianness if necessary
            if constexpr (std::endian::native != std::endian::little) {
                k1 = __builtin_bswap32(k1); // Swap bytes for big-endian
                k2 = __builtin_bswap32(k2);
                k3 = __builtin_bswap32(k3);
                k4 = __builtin_bswap32(k4);
            }

            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;

            h1 = ROTL32(h1, 19);
            h1 += h2;
            h1 = h1 * 5 + 0x561ccd1b;

            k2 *= c2;
            k2 = ROTL32(k2, 16);
            k2 *= c3;
            h2 ^= k2;

            h2 = ROTL32(h2, 17);
            h2 += h3;
            h2 = h2 * 5 + 0x0bcaa747;

            k3 *= c3;
            k3 = ROTL32(k3, 17);
            k3 *= c4;
            h3 ^= k3;

            h3 = ROTL32(h3, 15);
            h3 += h4;
            h3 = h3 * 5 + 0x96cd1c35;

            k4 *= c4;
            k4 = ROTL32(k4, 18);
            k4 *= c1;
            h4 ^= k4;

            h4 = ROTL32(h4, 13);
            h4 += h1;
            h4 = h4 * 5 + 0x32ac3b17;
        }

        // ------------------
        // Tail
        // ------------------
        const auto tail = data + nblocks * 16;

        std::uint32_t k1 = 0;
        std::uint32_t k2 = 0;
        std::uint32_t k3 = 0;
        std::uint32_t k4 = 0;

        switch (len & 15) {
        case 15: k4 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[14])) << 16; [[fallthrough]];
        case 14: k4 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[13])) << 8; [[fallthrough]];
        case 13: k4 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[12]));
            k4 *= c4;
            k4 = ROTL32(k4, 18);
            k4 *= c1;
            h4 ^= k4; [[fallthrough]];

        case 12: k3 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[11])) << 24; [[fallthrough]];
        case 11: k3 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[10])) << 16; [[fallthrough]];
        case 10: k3 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[9])) << 8; [[fallthrough]];
        case 9:  k3 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[8]));
            k3 *= c3;
            k3 = ROTL32(k3, 17);
            k3 *= c4;
            h3 ^= k3; [[fallthrough]];

        case 8:  k2 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[7])) << 24; [[fallthrough]];
        case 7:  k2 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[6])) << 16; [[fallthrough]];
        case 6:  k2 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[5])) << 8; [[fallthrough]];
        case 5:  k2 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[4]));
            k2 *= c2;
            k2 = ROTL32(k2, 16);
            k2 *= c3;
            h2 ^= k2; [[fallthrough]];

        case 4:  k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[3])) << 24; [[fallthrough]];
        case 3:  k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[2])) << 16; [[fallthrough]];
        case 2:  k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[1])) << 8; [[fallthrough]];
        case 1:  k1 ^= static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(tail[0]));
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
            break;
        }

        // ------------------
        // Finalization
        // ------------------
        h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

        h1 += h2; h1 += h3; h1 += h4;
        h2 += h1; h3 += h1; h4 += h1;

        h1 = fmix32(h1);
        h2 = fmix32(h2);
        h3 = fmix32(h3);
        h4 = fmix32(h4);

        h1 += h2; h1 += h3; h1 += h4;
        h2 += h1; h3 += h1; h4 += h1;

        *reinterpret_cast<std::uint32_t*>(out) = h1;
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(out) + 4) = h2;
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(out) + 8) = h3;
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(out) + 12) = h4;
    }


    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    static void MurmurHash3_x64_128(const void* key, int len, std::uint32_t seed, void* out) {
        const auto data = static_cast<const std::byte*>(key);  // Cast to std::byte* for arithmetic
        const int nblocks = len / 16;

        std::uint64_t h1 = seed;
        std::uint64_t h2 = seed;

        constexpr std::uint64_t c1 = 0x87c37b91114253d5;
        constexpr std::uint64_t c2 = 0x4cf5ad432745937f;

        // ------------------
        // Body
        // ------------------
        const auto blocks = reinterpret_cast<const std::uint64_t*>(data);

        for (int i = 0; i < nblocks; ++i) {
            std::uint64_t k1 = blocks[i * 2 + 0];
            std::uint64_t k2 = blocks[i * 2 + 1];

            // Convert endianness if necessary
            if constexpr (std::endian::native != std::endian::little) {
                k1 = __builtin_bswap64(k1); // Swap bytes for big-endian
                k2 = __builtin_bswap64(k2);
            }

            k1 *= c1;
            k1 = ROTL64(k1, 31);
            k1 *= c2;
            h1 ^= k1;

            h1 = ROTL64(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;

            k2 *= c2;
            k2 = ROTL64(k2, 33);
            k2 *= c1;
            h2 ^= k2;

            h2 = ROTL64(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
        }

        // ------------------
        // Tail
        // ------------------
        const auto tail = data + nblocks * 16; // Use data which is now a byte pointer

        std::uint64_t k1 = 0;
        std::uint64_t k2 = 0;

        switch (len & 15) {
        case 15: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[14])) << 48; [[fallthrough]];
        case 14: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[13])) << 40; [[fallthrough]];
        case 13: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[12])) << 32; [[fallthrough]];
        case 12: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[11])) << 24; [[fallthrough]];
        case 11: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[10])) << 16; [[fallthrough]];
        case 10: k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[9])) << 8; [[fallthrough]];
        case 9:  k2 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[8]));
            k2 *= c2;
            k2 = ROTL64(k2, 33);
            k2 *= c1;
            h2 ^= k2; [[fallthrough]];

        case 8:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[7])) << 56; [[fallthrough]];
        case 7:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[6])) << 48; [[fallthrough]];
        case 6:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[5])) << 40; [[fallthrough]];
        case 5:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[4])) << 32; [[fallthrough]];
        case 4:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[3])) << 24; [[fallthrough]];
        case 3:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[2])) << 16; [[fallthrough]];
        case 2:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[1])) << 8; [[fallthrough]];
        case 1:  k1 ^= static_cast<std::uint64_t>(std::to_integer<std::uint8_t>(tail[0]));
            k1 *= c1;
            k1 = ROTL64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
            break;
        }

        // ------------------
        // Finalization
        // ------------------
        h1 ^= len;
        h2 ^= len;

        h1 += h2;
        h2 += h1;

        h1 = fmix64(h1);
        h2 = fmix64(h2);

        h1 += h2;
        h2 += h1;

        *reinterpret_cast<std::uint64_t*>(out) = h1;
        *reinterpret_cast<std::uint64_t*>(reinterpret_cast<std::byte*>(out) + 8) = h2; // Fix pointer arithmetic for output
    }

};
