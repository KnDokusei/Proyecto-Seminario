#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

class Producto {
private:
    std::string id;
    std::string nombre;
    std::string categoria;
    double precio;
    int cantidad;
    int stockMinimo;

public:
    Producto(std::string id_, std::string nombre_, std::string categoria_, double precio_, int cantidad_, int stockMinimo_ = 5);

    std::string getId() const;
    std::string getNombre() const;
    std::string getCategoria() const;
    double getPrecio() const;
    int getCantidad() const;
    int getStockMinimo() const;

    void setNombre(const std::string& nuevoNombre);
    void setCategoria(const std::string& nuevaCategoria);
    void setPrecio(double nuevoPrecio);
    void setCantidad(int nuevaCantidad);
    void setStockMinimo(int nuevoMinimo);
    std::string obtenerResumen() const;

};

#endif
