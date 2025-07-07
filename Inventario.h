#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "Producto.h"
#include <vector>
#include <string>

class Inventario {
private:
    std::vector<Producto> productos;

public:
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    bool guardarEnArchivo(const std::string& nombreArchivo) const;

    void agregarProducto(const Producto& nuevo);
    bool eliminarProductoPorId(const std::string& id);
    Producto* buscarPorId(const std::string& id);
    std::vector<Producto> buscarPorNombre(const std::string& nombre);

    void mostrarTodos() const;
    void generarReporteStockBajo(int limite = 5) const;
    bool actualizarStockPorId(const std::string& id, int cambioCantidad);
};

#endif // INVENTARIO_H
