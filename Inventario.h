#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "Producto.h"
#include <vector>
#include <string>

class Inventario {
private:
    std::vector<Producto> productos;
    int contadorID = 1;
    std::string generarID();

public:
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    bool guardarEnArchivo(const std::string& nombreArchivo) const;

    std::string agregarProducto(const std::string& nombre, const std::string& categoria, double precio, int cantidad);
    void agregarProducto(const Producto& nuevo);

    bool eliminarProductoPorId(const std::string& id);
    Producto* buscarPorId(const std::string& id);
    std::vector<Producto> buscarPorNombre(const std::string& nombre);
    std::vector<Producto> buscarPorCategoria(const std::string& categoria);

    std::vector<std::string> generarReporteStockBajo(int limite = 5) const;
    bool actualizarStockPorId(const std::string& id, int cambioCantidad);
    std::vector<std::string> obtenerResumenTodos() const;
    std::vector<Producto>& getProductos(); // para que MainWindow acceda directamente

    const std::vector<Producto>& getProductos() const;  // nuevo getter

    bool guardarRutaUltimoArchivo(const std::string& ruta) const;
    std::string obtenerRutaUltimoArchivo() const;

};

#endif // INVENTARIO_H

