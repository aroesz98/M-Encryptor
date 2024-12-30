/*
 * Encryptor
 * Author: Arkadiusz Szlanta
 * Date: 30 Dec 2024
 */

#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include <cstdint>

class RandomGenerator
{
    public:
        RandomGenerator(uint32_t s);
        uint32_t next();

    private:
        uint32_t seed;
};

class Encryptor
{
    public:
        Encryptor(uint32_t seed, uint32_t key[8]);
        uint32_t encrypt(uint32_t buffer[256], uint32_t encryptedBuffer[256]);
        void decrypt(uint32_t encryptedBuffer[256], uint32_t buffer[256], uint32_t randomKey);

    private:
        uint32_t key[8];
        RandomGenerator rng;

        void shuffleData(uint32_t data[256], uint32_t randomKey);
        void unshuffleData(uint32_t data[256], uint32_t randomKey);
        uint32_t mix(uint32_t data, uint32_t key);
        void swap(uint32_t& a, uint32_t& b);
};

#endif /* ENCRYPTOR_HPP */
