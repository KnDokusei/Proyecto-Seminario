#include "Producto.h"
#include <iostream>

Producto::Producto() : id(""), nombre(""), precio(0.0), cantidad(0) {}

Producto::Producto(const std::string& id, const std::string& nombre, double precio, int cantidad)
    : id(id), nombre(nombre), precio(precio), cantidad(cantidad) {}

std::string Producto::getId() const {
    return id;
}

std::string Producto::getNombre() const {
    return nombre;
}

double Producto::getPrecio() const {
    return precio;
}

int Producto::getCantidad() const {
    return cantidad;
}

void Producto::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Producto::setPrecio(double nuevoPrecio) {
    precio = nuevoPrecio;
}

void Producto::setCantidad(int nuevaCantidad) {
    cantidad = nuevaCantidad;
}

void Producto::mostrarProducto() const {
    std::cout << "ID: " << id
              << " | Nombre: " << nombre
              << " | Precio: $" << precio
              << " | Cantidad: " << cantidad << std::endl;
}
