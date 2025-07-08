#include "Producto.h"

Producto::Producto(std::string id_, std::string nombre_, std::string categoria_, double precio_, int cantidad_, int stockMinimo_)
    : id(id_), nombre(nombre_), categoria(categoria_), precio(precio_), cantidad(cantidad_), stockMinimo(stockMinimo_) {}

std::string Producto::getId() const { return id; }
std::string Producto::getNombre() const { return nombre; }
std::string Producto::getCategoria() const { return categoria; }
double Producto::getPrecio() const { return precio; }
int Producto::getCantidad() const { return cantidad; }
int Producto::getStockMinimo() const { return stockMinimo; }

void Producto::setNombre(const std::string& nuevoNombre) { nombre = nuevoNombre; }
void Producto::setCategoria(const std::string& nuevaCategoria) { categoria = nuevaCategoria; }
void Producto::setPrecio(double nuevoPrecio) { precio = nuevoPrecio; }
void Producto::setCantidad(int nuevaCantidad) { cantidad = nuevaCantidad; }
void Producto::setStockMinimo(int nuevoMinimo) { stockMinimo = nuevoMinimo; }
std::string Producto::obtenerResumen() const {
    return id + " - " + nombre + " (" + categoria + ") - $" + std::to_string(precio) + " - Stock: " + std::to_string(cantidad);
}
