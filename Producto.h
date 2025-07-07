#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

class Producto {
private:
    std::string id;
    std::string nombre;
    double precio;
    int cantidad;

public:
    Producto();
    Producto(const std::string& id, const std::string& nombre, double precio, int cantidad);

    std::string getId() const;
    std::string getNombre() const;
    double getPrecio() const;
    int getCantidad() const;

    void setNombre(const std::string& nuevoNombre);
    void setPrecio(double nuevoPrecio);
    void setCantidad(int nuevaCantidad);

    void mostrarProducto() const;
};

#endif // PRODUCTO_H
