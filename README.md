AES is a block cipher that operates on byte-sized chunks of data. In order to make the AES implementation taurus-fully-homomorphic, each of the 3 operations, i.e., XOR, multiplication and S-Box substitution, are broken into operations on 4-bits of information. 

The multiplication operator is implemented as 6 unary multiplication operators, i.e., mul_2, mul_3, mul_9, mul_b, mul_d and mul_e. Each of the operators are broken into 2 Lookup Tables, one that stores the least-significant 4 bits of the result and the other that stores the 4 most-significant bits of the result. This is done so as to allow for parallelism and taking the base as 2^4 i.e., 16, which is the ideal value for the base as it balances the trade-off between the number of bootstrapping operations and the time required per bootstrapping operation to give the least total running time.

We first expand the key in plain text using the standard AES Key expansion algorithm and then use homomorphic encryption on the expanded keys as this is much faster in comparison to encoding the original key and then using homomorphic computation to expand the key.
We have created a common function named LUT, which stands for Lookup Table, that uses the tree-based method as explained earlier to evaluate a function based on the lookup tables passed as a parameter to the function. This function allows for all 3 types of operations i.e., XOR, multiplication in GF(256) and S-Box substitution to be performed efficiently.

In order to compute the lookup tables required for invoking the LUT function, we first created the standard lookup tables of size 256 (8x8) in plaintext for all the operations, followed by separating each byte value into 2 4-bit entities and then encrypting them. One of these values populates one entry of the LSB Lookup-table and the other populates one entry of the MSB Lookup-table for the operation.

We wrote a standard AES implementation by taking reference from https://github.com/ceceww/aes.git that uses the LUT function to perform the elementary operations like XOR, GF(256) multiplication and S-Box substitution. We created helper functions for each of the steps of AES, i.e., RoundKeyAddition, SubBytes, ShiftRows and MixColumn. We used these functions serially to create a function to perform 1 round of AES and called the function repeatedly to complete the AES encryption of the message in a fully-homomorphic fashion.

The main AES code is contained in the file "encrypt.cpp" in the folder "aes-master".
The code for the LUT is written in the file "LUT.cpp".
The entire tfhe library referenced in the paper is used in the code and is contained in the folder "tfhe-master".

The files "todo", "temp.cpp" can be ignored.

