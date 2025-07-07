#include "Inventario.h"
#include "Utilidades.h"
#include <fstream>

bool Inventario::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return false;

    productos.clear();
    int mayorID = 0;
    std::string linea;
    while (getline(archivo, linea)) {
        auto campos = dividir(linea, ',');
        if (campos.size() == 5) {
            Producto p(
                limpiarEspacios(campos[0]), limpiarEspacios(campos[1]),
                limpiarEspacios(campos[2]), std::stod(campos[3]), std::stoi(campos[4])
            );
            productos.push_back(p);
            if (campos[0].size() >= 6 && campos[0][0] == 'P') {
                try {
                    int num = std::stoi(campos[0].substr(1));
                    if (num > mayorID) mayorID = num;
                } catch (...) {}
            }
        }
    }
    archivo.close();
    contadorID = mayorID + 1;
    return true;
}

bool Inventario::guardarEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return false;

    for (const auto& p : productos) {
        archivo << p.getId() << "," << p.getNombre() << "," << p.getCategoria()
                << "," << p.getPrecio() << "," << p.getCantidad() << "\n";
    }
    archivo.close();
    return true;
}

std::string Inventario::agregarProducto(const std::string& nombre, const std::string& categoria, double precio, int cantidad) {
    std::string id = generarID();
    Producto nuevo(id, nombre, categoria, precio, cantidad);
    productos.push_back(nuevo);
    return id;
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

std::vector<Producto> Inventario::buscarPorCategoria(const std::string& categoria) {
    std::vector<Producto> resultado;
    for (const auto& p : productos) {
        if (p.getCategoria() == categoria) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

std::vector<std::string> Inventario::obtenerResumenTodos() const {
    std::vector<std::string> resumen;
    for (const auto& p : productos) {
        resumen.push_back(p.obtenerResumen());
    }
    return resumen;
}

std::vector<std::string> Inventario::generarReporteStockBajo(int limite) const {
    std::vector<std::string> reporte;
    for (const auto& p : productos) {
        if (p.getCantidad() < limite) {
            reporte.push_back(p.obtenerResumen());
        }
    }
    return reporte;
}

bool Inventario::actualizarStockPorId(const std::string& id, int cambioCantidad) {
    Producto* p = buscarPorId(id);
    if (p) {
        int nuevaCantidad = p->getCantidad() + cambioCantidad;
        if (nuevaCantidad < 0) return false;
        p->setCantidad(nuevaCantidad);
        return true;
    }
    return false;
}

std::string Inventario::generarID() {
    std::string id = "P" + std::to_string(contadorID);
    while (id.length() < 6) id.insert(1, "0");
    contadorID++;
    return id;
}

const std::vector<Producto>& Inventario::getProductos() const {
    return productos;
}
