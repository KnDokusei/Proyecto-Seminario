#include "Inventario.h"
#include "Utilidades.h"
#include <iostream>
#include <fstream>

bool Inventario::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return false;

    productos.clear();
    std::string linea;
    while (getline(archivo, linea)) {
        auto campos = dividir(linea, ',');
        if (campos.size() == 4) {
            Producto p(
                limpiarEspacios(campos[0]),
                limpiarEspacios(campos[1]),
                std::stod(campos[2]),
                std::stoi(campos[3])
            );
            productos.push_back(p);
        }
    }

    archivo.close();
    return true;
}

bool Inventario::guardarEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return false;

    for (const auto& p : productos) {
        archivo << p.getId() << ","
                << p.getNombre() << ","
                << p.getPrecio() << ","
                << p.getCantidad() << "\n";
    }

    archivo.close();
    return true;
}

void Inventario::agregarProducto(const Producto& nuevo) {
    productos.push_back(nuevo);
}

bool Inventario::eliminarProductoPorId(const std::string& id) {
    for (auto it = productos.begin(); it != productos.end(); ++it) {
        if (it->getId() == id) {
            productos.erase(it);
            return true;
        }
    }
    return false;
}

Producto* Inventario::buscarPorId(const std::string& id) {
    for (auto& p : productos) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

std::vector<Producto> Inventario::buscarPorNombre(const std::string& nombre) {
    std::vector<Producto> resultado;
    for (const auto& p : productos) {
        if (p.getNombre().find(nombre) != std::string::npos) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

void Inventario::mostrarTodos() const {
    for (const auto& p : productos) {
        p.mostrarProducto();
    }
}

void Inventario::generarReporteStockBajo(int limite) const {
    std::cout << "\n--- Productos con stock bajo (< " << limite << ") ---\n";
    for (const auto& p : productos) {
        if (p.getCantidad() < limite) {
            p.mostrarProducto();
        }
    }
}

bool Inventario::actualizarStockPorId(const std::string& id, int cambioCantidad) {
    Producto* p = buscarPorId(id);
    if (p != nullptr) {
        int nuevaCantidad = p->getCantidad() + cambioCantidad;
        if (nuevaCantidad < 0) {
            std::cout << "Error: no hay suficiente stock para esa operación.\n";
            return false;
        }
        p->setCantidad(nuevaCantidad);
        return true;
    }
    std::cout << "Producto no encontrado.\n";
    return false;
}
