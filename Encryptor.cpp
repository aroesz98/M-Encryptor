/*
 * Encryptor
 * Author: Arkadiusz Szlanta
 * Date: 30 Dec 2024
 *
 * License:
 * This source code is provided for hobbyist and private use only.
 * Any commercial or industrial use, including distribution, reproduction, or
 * incorporation in commercial or industrial products or services is prohibited.
 * Use at your own risk. The author(s) hold no responsibility for any damages
 * or losses resulting from the use of this software.
 *
 */

#include <Encryptor.hpp>

extern "C" void memcpy_optimized(void *d, void *s, uint32_t len);

RandomGenerator::RandomGenerator(uint32_t s) : seed(s)
{
}

uint32_t RandomGenerator::next()
{
    seed = (1664525 * seed + 1013904223);
    return seed;
}

Encryptor::Encryptor(uint32_t seed, uint32_t key[8]) : rng(seed)
{
    memcpy_optimized(&(this->key[0]), &key[0], 32u);
}

uint32_t Encryptor::encrypt(uint32_t buffer[256], uint32_t encryptedBuffer[256])
{
    uint32_t randomKey = rng.next();
    uint32_t flatEncryptedBuffer[256];

    // Mix data in the input buffer
    shuffleData(buffer, randomKey);

    // Encryption of mixed data
    for (int i = 0; i < 256; i += 4)
    {
        uint32_t value1 = buffer[i];
        uint32_t value2 = buffer[i + 1];
        uint32_t value3 = buffer[i + 2];
        uint32_t value4 = buffer[i + 3];

        for (int k = 0; k < 8; ++k)
        {
            value1 = mix(value1, key[k]);
            value2 = mix(value2, key[k]);
            value3 = mix(value3, key[k]);
            value4 = mix(value4, key[k]);

            __asm__("ror %[result], %[value], #1" : [result] "=r" (value1) : [value] "r" (value1));
            __asm__("ror %[result], %[value], #1" : [result] "=r" (value2) : [value] "r" (value2));
            __asm__("ror %[result], %[value], #1" : [result] "=r" (value3) : [value] "r" (value3));
            __asm__("ror %[result], %[value], #1" : [result] "=r" (value4) : [value] "r" (value4));
        }

        flatEncryptedBuffer[i] = value1;
        flatEncryptedBuffer[i + 1] = value2;
        flatEncryptedBuffer[i + 2] = value3;
        flatEncryptedBuffer[i + 3] = value4;
    }

    memcpy_optimized(&encryptedBuffer[0], &flatEncryptedBuffer[0], 256u * sizeof(uint32_t));

    return randomKey;
}

void Encryptor::decrypt(uint32_t encryptedBuffer[256], uint32_t buffer[256], uint32_t randomKey) {
    uint32_t flatBuffer[256];

    for (int i = 0; i < 256; i += 4)
    {
        uint32_t value1 = encryptedBuffer[i];
        uint32_t value2 = encryptedBuffer[i + 1];
        uint32_t value3 = encryptedBuffer[i + 2];
        uint32_t value4 = encryptedBuffer[i + 3];

        for (int k = 7; k >= 0; --k)
        {
            __asm__("ror %[result], %[value], #31" : [result] "=r" (value1) : [value] "r" (value1));
            __asm__("ror %[result], %[value], #31" : [result] "=r" (value2) : [value] "r" (value2));
            __asm__("ror %[result], %[value], #31" : [result] "=r" (value3) : [value] "r" (value3));
            __asm__("ror %[result], %[value], #31" : [result] "=r" (value4) : [value] "r" (value4));

            value1 = mix(value1, key[k]);
            value2 = mix(value2, key[k]);
            value3 = mix(value3, key[k]);
            value4 = mix(value4, key[k]);
        }

        flatBuffer[i] = value1;
        flatBuffer[i + 1] = value2;
        flatBuffer[i + 2] = value3;
        flatBuffer[i + 3] = value4;
    }

    // Restore original order of data
    unshuffleData(flatBuffer, randomKey);
    memcpy_optimized(&buffer[0], &flatBuffer[0], 256u * sizeof(uint32_t));
}

void Encryptor::shuffleData(uint32_t data[256], uint32_t randomKey)
{
    RandomGenerator rng(randomKey);
    for (int i = 255; i > 0; i -= 4)
    {
        int j1 = rng.next() % (i + 1);
        int j2 = rng.next() % (i + 1);
        int j3 = rng.next() % (i + 1);
        int j4 = rng.next() % (i + 1);
        swap(data[i], data[j1]);
        if (i - 1 > 0) swap(data[i - 1], data[j2]);
        if (i - 2 > 0) swap(data[i - 2], data[j3]);
        if (i - 3 > 0) swap(data[i - 3], data[j4]);
    }
}


void Encryptor::unshuffleData(uint32_t data[256], uint32_t randomKey)
{
    RandomGenerator rng(randomKey);
    uint32_t indices[256];
    for (int i = 0; i < 256; i += 4u)
    {
        indices[i] = i;
        indices[i + 1] = i + 1u;
        indices[i + 2] = i + 2u;
        indices[i + 3] = i + 3u;
    }
    for (int i = 255; i > 0; i -= 4)
    {
        int j1 = rng.next() % (i + 1);
        int j2 = rng.next() % (i + 1);
        int j3 = rng.next() % (i + 1);
        int j4 = rng.next() % (i + 1);
        swap(indices[i], indices[j1]);
        if (i - 1 > 0) swap(indices[i - 1], indices[j2]);
        if (i - 2 > 0) swap(indices[i - 2], indices[j3]);
        if (i - 3 > 0) swap(indices[i - 3], indices[j4]);
    }

    uint32_t temp[256];
    for (int i = 0; i < 256; i += 4)
    {
        temp[indices[i]] = data[i];
        temp[indices[i + 1]] = data[i + 1];
        temp[indices[i + 2]] = data[i + 2];
        temp[indices[i + 3]] = data[i + 3];
    }

    memcpy_optimized(&data[0], &temp[0], 256 * sizeof(uint32_t));
}


uint32_t Encryptor::mix(uint32_t data, uint32_t key)
{
    return (data ^ ((key >> 16) | (key << 16)));
}

void Encryptor::swap(uint32_t& a, uint32_t& b)
{
    uint32_t temp = a;
    a = b;
    b = temp;
}
