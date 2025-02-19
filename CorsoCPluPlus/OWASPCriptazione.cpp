#include <iostream>
#include <string>

std::string encryptCaesarCipher(const std::string& text, int shift) {
    std::string result = "";

    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
        result += c;
    }

    return result;
}

std::string decryptCaesarCipher(const std::string& text, int shift) {
    return encryptCaesarCipher(text, 26 - shift);
}

int main() {
    std::string text;
    int shift;

    std::cout << "Enter text to encrypt: ";
    std::getline(std::cin, text);
    std::cout << "Enter shift value: ";
    std::cin >> shift;

    std::string encryptedText = encryptCaesarCipher(text, shift);
    std::string decryptedText = decryptCaesarCipher(encryptedText, shift);

    std::cout << "Encrypted Text: " << encryptedText << std::endl;
    std::cout << "Decrypted Text: " << decryptedText << std::endl;

    return 0;
}
