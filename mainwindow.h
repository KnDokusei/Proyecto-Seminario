#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Inventario.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cargarInventario();
    void guardarInventario();
    void agregarProducto();
    void eliminarProducto();
    void buscarPorCategoria();
    void mostrarProductos();
    void mostrarStockBajo();
    void editarStockProducto();

private:
    Ui::MainWindow *ui;
    Inventario inventario;
};

#endif // MAINWINDOW_H
