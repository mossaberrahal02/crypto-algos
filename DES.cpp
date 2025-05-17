#include "libraries.hpp"

using namespace std;

// Définition des tables IP, E, S-box, etc. (partiellement implémentées ici)

// Permutation Initiale IP
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Table d'expansion E
const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7,
    8, 9, 8, 9, 10, 11, 12, 13, 12, 13,
    14, 15, 16, 17, 16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

// Fonction de permutation générique
void permute(const bitset<64>& input, const int* table, int n, bitset<64>& output) {
    for (int i = 0; i < n; i++) {
        output[i] = input[table[i] - 1];
    }
}

// Fonction f : Transformation appliquée sur la moitié droite (R)
bitset<32> f_function(const bitset<32>& R, const bitset<48>& K) {
    bitset<48> expandedR;
    permute(R, E, 48, expandedR);  // Expansion de R (32 -> 48 bits)

    // XOR entre R et K (sous-clé)
    bitset<48> xorResult = expandedR ^ K;

    // Appliquer les S-boxes et permutation (simplifié ici)
    bitset<32> output;  // Cette partie serait un travail supplémentaire de substitution et permutation
    return output;
}

// Fonction de chiffrement : une ronde de DES
void des_round(bitset<32>& L, bitset<32>& R, const bitset<48>& K) {
    bitset<32> newL = R;
    bitset<32> newR = L ^ f_function(R, K);  // XOR entre L et f(R, K)

    L = newL;
    R = newR;
}

// Fonction principale
int main() {
    // Exemple de texte clair (64 bits) et clé (56 bits)
    bitset<64> input("0000000100100011010001010110011110001001101010111100111111100001");  // 64 bits de texte clair
    bitset<56> key("0000000000000001000000000000001100000010000100000100000000000000");  // 56 bits de clé

    // Appliquer la permutation initiale (IP)
    bitset<64> permutedInput;
    permute(input, IP, 64, permutedInput);

    // Séparer les 64 bits en L et R (L et R sont des parties de 32 bits chacune)
    bitset<32> L, R;
    for (int i = 0; i < 32; i++) {
        L[i] = permutedInput[i];
        R[i] = permutedInput[i + 32];
    }

    // Générer les sous-clés à partir de la clé de 56 bits
    vector<bitset<48>> subkeys(16);  // Tableau de 16 sous-clés de 48 bits
    // Génération des sous-clés ici (processus complet avec décalages et permutations)

    // Effectuer les 16 rondes de DES
    for (int i = 0; i < 16; i++) {
        des_round(L, R, subkeys[i]);
    }

    // Après les rondes, on fusionne L et R
    bitset<64> finalBlock;
    for (int i = 0; i < 32; i++) {
        finalBlock[i] = L[i];
        finalBlock[i + 32] = R[i];
    }

    // Appliquer la permutation finale (FP)
    bitset<64> finalResult;
    permute(finalBlock, IP, 64, finalResult);  // La permutation finale FP est la même que IP dans le cas de DES

    // Afficher le résultat final
    cout << "Résultat chiffré : " << finalResult << endl;
    return 0;
}
