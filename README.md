# M-Encryptor
Encryptor for ARM Cortex-M (M4/M7/M33)

## RandomGenerator

### Functionality
`RandomGenerator` is a class that generates pseudorandom numbers based on a given seed. The seed is initialized when the object is created and serves as the starting point for generating subsequent pseudorandom values.

### Algorithm
Subsequent pseudorandom values are generated using a linear congruential generator (LCG). This is a simple and fast method for generating pseudorandom numbers.

### Explanation of the Algorithm
The LCG algorithm is a straightforward and quick method to generate pseudorandom integers. The constants `a` and `c` are carefully chosen to provide a long period of generated numbers. The value `seed` is updated using the formula and then used to generate the next pseudorandom number.

## Encryptor

### Functionality
`Encryptor` is a class used for encrypting and decrypting data using a key and a pseudorandom number generator. The encryption algorithm uses bitwise operations and data shuffling.

### Encryption Algorithm

1. **Data Shuffling (shuffleData):**
   - The data in the buffer is shuffled using a pseudorandom key.
2. **Data Mixing (mix):**
   - The data is encrypted using the key and XOR and ROTATE operations.
3. **ROTATE Operation:**
   - The data is bitwise rotated left and right.

### Explanation of the Algorithm

1. **Initialization of the Key and Pseudorandom Generator:** The `Encryptor` object is initialized with the key and seed, which are passed to the constructor.
2. **Data Shuffling:** Before encryption, the data in the buffer is shuffled using the `shuffleData` function, which employs a pseudorandom number generator to randomly reorder the elements in the buffer.
3. **Data Mixing and Rotation:** The data is mixed with the key using XOR and bitwise rotation operations, increasing the difficulty of reversing the encryption without the key.
4. **Encryption:** The encrypted data is stored in the output buffer.
5. **Decryption:** The decryption process is the reverse of the encryption process. The data is first decrypted and then reshuffled back to its original order using the `unshuffleData` function.

### Example Usage

```cpp
uint32_t key[8] = {0x12345678, 0x23456789, 0x34567890, 0x45678901, 0x56789012, 0x67890123, 0x78901234, 0x89012345};
Encryptor encryptor(12345, key);
uint32_t buffer[256] = { ... }; // data to be encrypted
uint32_t encryptedBuffer[256];
uint32_t decryptedBuffer[256];

uint32_t randomKey = encryptor.encrypt(buffer, encryptedBuffer);
encryptor.decrypt(encryptedBuffer, decryptedBuffer, randomKey);
```
