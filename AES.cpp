#include "libraries.hpp"

const unsigned char SBox[256] = {};

const unsigned char Rcon[10] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b};

void KeyExpansion(const unsigned char* key, unsigned char* roundKeys) {
    int i = 0;
    while (i < 16) {
        roundKeys[i] = key[i];
        i++;
    }

    i = 16;
    while (i < 176) {
        unsigned char temp[4];
        for (int j = 0; j < 4; ++j) {
            temp[j] = roundKeys[i - 4 + j];
        }

        if (i % 16 == 0) {
            unsigned char temp2 = temp[0];
            for (int j = 0; j < 3; ++j) {
                temp[j] = temp[j + 1];
            }
            temp[3] = temp2;

            for (int j = 0; j < 4; ++j) {
                temp[j] = SBox[temp[j]];
            }

            temp[0] ^= Rcon[i / 16];
        }

        for (int j = 0; j < 4; ++j) {
            roundKeys[i + j] = roundKeys[i - 16 + j] ^ temp[j];
        }

        i += 4;
    }
}

void textToState(const unsigned char* input, unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = input[i * 4 + j];
        }
    }
}

void stateToText(unsigned char state[4][4], unsigned char* output) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            output[i * 4 + j] = state[j][i];
        }
    }
}


void SubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = SBox[state[i][j]];
        }
    }
}

void ShiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    for (int i = 1; i < 4; ++i) {
        for (int j = 0; j < i; ++j) {
            temp = state[i][0];
            for (int k = 0; k < 3; ++k) {
                state[i][k] = state[i][k + 1];
            }
            state[i][3] = temp;
        }
    }
}

void MixColumns(unsigned char state[4][4]) {}

void AddRoundKey(unsigned char state[4][4], unsigned char* roundKey) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] ^= roundKey[i * 4 + j];
        }
    }
}


void AES_Encrypt(unsigned char* input, unsigned char* output, unsigned char* key) {
    unsigned char roundKeys[176];
    KeyExpansion(key, roundKeys);

    unsigned char state[4][4];
    textToState(input, state);

    AddRoundKey(state, roundKeys);

    for (int round = 1; round < 10; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys + round * 16);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys + 160);
    stateToText(state, output);
}


int main() {
    unsigned char key[16] = {};
    unsigned char input[16] = {};
    unsigned char output[16];

    AES_Encrypt(input, output, key);

    std::cout << "Texte chiffré : ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)output[i];
    }
    std::cout << std::endl;

    return 0;
}
