#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// Classe di esempio da serializzare e deserializzare
class Esempio {
public:
    int id;
    char nome[50];
};

// Funzione per serializzare l'oggetto e scriverlo su file
void serializza(const std::string& filename, const Esempio& obj) {
    std::ofstream ofs(filename, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(&obj), sizeof(Esempio));
    ofs.close();
}

// Funzione per deserializzare l'oggetto dal file
void deserializza(const std::string& filename, Esempio& obj) {
    std::ifstream ifs(filename, std::ios::binary);
    ifs.read(reinterpret_cast<char*>(&obj), sizeof(Esempio));
    ifs.close();
}

int main() {
    // Creazione di un oggetto e serializzazione
    Esempio obj;
    obj.id = 1;
    std::strcpy(obj.nome, "NomeEsempio");

    std::string filename = "data.bin";
    serializza(filename, obj);

    // Deserializzazione
    Esempio objDeserializzato;
    deserializza(filename, objDeserializzato);

    // Stampa dell'oggetto deserializzato
    std::cout << "ID: " << objDeserializzato.id << std::endl;
    std::cout << "Nome: " << objDeserializzato.nome << std::endl;

    return 0;
}
