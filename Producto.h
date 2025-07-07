#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

class Producto {
private:
    std::string id;
    std::string nombre;
    double precio;
    int cantidad;
    std::string categoria;

public:
    Producto();
    Producto(const std::string& id, const std::string& nombre, const std::string& categoria, double precio, int cantidad);

    std::string getId() const;
    std::string getNombre() const;
    std::string getCategoria() const;
    double getPrecio() const;
    int getCantidad() const;

    void setNombre(const std::string& nuevoNombre);
    void setCategoria(const std::string& nuevaCategoria);
    void setPrecio(double nuevoPrecio);
    void setCantidad(int nuevaCantidad);

    std::string obtenerResumen() const;
};

#endif // PRODUCTO_H
