#include "Inventario.h"
#include "Producto.h"
#include <iostream>
#include <limits>

void mostrarMenu() {
    std::cout << "\n======= SISTEMA DE INVENTARIO =======\n";
    std::cout << "1. Cargar inventario desde archivo\n";
    std::cout << "2. Guardar inventario en archivo\n";
    std::cout << "3. Agregar producto\n";
    std::cout << "4. Eliminar producto por ID\n";
    std::cout << "5. Buscar producto por ID\n";
    std::cout << "6. Buscar producto por nombre\n";
    std::cout << "7. Mostrar todos los productos\n";
    std::cout << "8. Reporte de stock bajo\n";
    std::cout << "9. Actualizar stock de producto (entrada/salida)\n";
    std::cout << "10. Salir\n";
    std::cout << "Seleccione una opción: ";
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Inventario inventario;
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                std::string archivo;
                std::cout << "Nombre del archivo: ";
                std::getline(std::cin, archivo);
                if (inventario.cargarDesdeArchivo(archivo))
                    std::cout << "Inventario cargado correctamente.\n";
                else
                    std::cout << "Error al cargar archivo.\n";
                break;
            }
            case 2: {
                std::string archivo;
                std::cout << "Nombre del archivo: ";
                std::getline(std::cin, archivo);
                if (inventario.guardarEnArchivo(archivo))
                    std::cout << "Inventario guardado correctamente.\n";
                else
                    std::cout << "Error al guardar archivo.\n";
                break;
            }
            case 3: {
                std::string id, nombre;
                double precio;
                int cantidad;

                std::cout << "ID: ";
                std::getline(std::cin, id);
                std::cout << "Nombre: ";
                std::getline(std::cin, nombre);
                std::cout << "Precio: ";
                std::cin >> precio;
                std::cout << "Cantidad: ";
                std::cin >> cantidad;
                limpiarBuffer();

                Producto p(id, nombre, precio, cantidad);
                inventario.agregarProducto(p);
                std::cout << "Producto agregado con éxito.\n";
                break;
            }
            case 4: {
                std::string id;
                std::cout << "ID del producto a eliminar: ";
                std::getline(std::cin, id);
                if (inventario.eliminarProductoPorId(id))
                    std::cout << "Producto eliminado.\n";
                else
                    std::cout << "No se encontró el producto.\n";
                break;
            }
            case 5: {
                std::string id;
                std::cout << "ID a buscar: ";
                std::getline(std::cin, id);
                Producto* p = inventario.buscarPorId(id);
                if (p) p->mostrarProducto();
                else std::cout << "Producto no encontrado.\n";
                break;
            }
            case 6: {
                std::string nombre;
                std::cout << "Nombre a buscar: ";
                std::getline(std::cin, nombre);
                auto resultados = inventario.buscarPorNombre(nombre);
                if (resultados.empty()) {
                    std::cout << "No se encontraron coincidencias.\n";
                } else {
                    for (const auto& p : resultados)
                        p.mostrarProducto();
                }
                break;
            }
            case 7:
                inventario.mostrarTodos();
                break;

            case 8:
                inventario.generarReporteStockBajo();
                break;

            case 9: {
                std::string id;
                int cambio;
                std::cout << "ID del producto: ";
                std::getline(std::cin, id);
                std::cout << "Cantidad a cambiar (positiva para entrada, negativa para salida): ";
                std::cin >> cambio;
                limpiarBuffer();

                if (inventario.actualizarStockPorId(id, cambio))
                    std::cout << "Stock actualizado correctamente.\n";
                else
                    std::cout << "No se pudo actualizar el stock.\n";
                break;
                }

            case 10:
                std::cout << "Saliendo del sistema...\n";
                break;

            

            default:
                std::cout << "Opción inválida.\n";
        }

    } while (opcion != 10);

    return 0;
}
