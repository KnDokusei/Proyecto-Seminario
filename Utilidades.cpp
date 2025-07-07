#include "Utilidades.h"
#include <algorithm>

std::vector<std::string> dividir(const std::string& linea, char delimitador) {
    std::vector<std::string> partes;
    std::stringstream ss(linea);
    std::string parte;

    while (getline(ss, parte, delimitador)) {
        partes.push_back(parte);
    }

    return partes;
}

std::string limpiarEspacios(const std::string& cadena) {
    std::string limpia = cadena;
    limpia.erase(remove_if(limpia.begin(), limpia.end(), ::isspace), limpia.end());
    return limpia;
}
