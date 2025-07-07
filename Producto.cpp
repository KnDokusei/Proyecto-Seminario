#include "Producto.h"

Producto::Producto() : id(""), nombre(""), categoria(""), precio(0.0), cantidad(0) {}

Producto::Producto(const std::string& id, const std::string& nombre, const std::string& categoria, double precio, int cantidad)
    : id(id), nombre(nombre), categoria(categoria), precio(precio), cantidad(cantidad) {}

std::string Producto::getId() const { return id; }
std::string Producto::getNombre() const { return nombre; }
std::string Producto::getCategoria() const { return categoria; }
double Producto::getPrecio() const { return precio; }
int Producto::getCantidad() const { return cantidad; }

void Producto::setNombre(const std::string& nuevoNombre) { nombre = nuevoNombre; }
void Producto::setCategoria(const std::string& nuevaCategoria) { categoria = nuevaCategoria; }
void Producto::setPrecio(double nuevoPrecio) { precio = nuevoPrecio; }
void Producto::setCantidad(int nuevaCantidad) { cantidad = nuevaCantidad; }

std::string Producto::obtenerResumen() const {
    return "ID: " + id + " | Nombre: " + nombre + " | Categoria: " + categoria +
           " | Precio: $" + std::to_string(precio) + " | Cantidad: " + std::to_string(cantidad);
}
