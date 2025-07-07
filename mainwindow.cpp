#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnCargar, &QPushButton::clicked, this, &MainWindow::cargarInventario);
    connect(ui->btnGuardar, &QPushButton::clicked, this, &MainWindow::guardarInventario);
    connect(ui->btnAgregar, &QPushButton::clicked, this, &MainWindow::agregarProducto);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &MainWindow::eliminarProducto);
    connect(ui->btnBuscarCategoria, &QPushButton::clicked, this, &MainWindow::buscarPorCategoria);
    connect(ui->btnStockBajo, &QPushButton::clicked, this, &MainWindow::mostrarStockBajo);
    connect(ui->tablaProductos, &QTableWidget::doubleClicked, this, &MainWindow::editarStockProducto);

    QStringList headers = {"ID", "Nombre", "Categoría", "Precio", "Cantidad"};
    ui->tablaProductos->setColumnCount(5);
    ui->tablaProductos->setHorizontalHeaderLabels(headers);
    ui->tablaProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setStyleSheet("QPushButton { background-color: #D0E8FF; } "
                  "QLineEdit { background-color: #FFF; padding: 2px; } "
                  "QTableWidget { background-color: #FFF; gridline-color: #CCC; }");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::cargarInventario() {
    QString archivo = QFileDialog::getOpenFileName(this, "Abrir Inventario", "", "CSV (*.csv)");
    if (!archivo.isEmpty()) {
        if (inventario.cargarDesdeArchivo(archivo.toStdString()))
            QMessageBox::information(this, "Éxito", "Inventario cargado.");
        else
            QMessageBox::warning(this, "Error", "No se pudo cargar.");
    }
    mostrarProductos();
}

void MainWindow::guardarInventario() {
    QString archivo = QFileDialog::getSaveFileName(this, "Guardar Inventario", "", "CSV (*.csv)");
    if (!archivo.isEmpty()) {
        if (inventario.guardarEnArchivo(archivo.toStdString()))
            QMessageBox::information(this, "Éxito", "Inventario guardado.");
        else
            QMessageBox::warning(this, "Error", "No se pudo guardar.");
    }
}

void MainWindow::agregarProducto() {
    QString nombre = QInputDialog::getText(this, "Agregar Producto", "Nombre:");
    if (nombre.isEmpty()) return;

    QString categoria = QInputDialog::getText(this, "Agregar Producto", "Categoría:");
    if (categoria.isEmpty()) return;

    bool okPrecio;
    double precio = QInputDialog::getDouble(this, "Agregar Producto", "Precio:", 0, 0, 1e9, 2, &okPrecio);
    if (!okPrecio || precio <= 0) return;

    bool okCantidad;
    int cantidad = QInputDialog::getInt(this, "Agregar Producto", "Cantidad:", 0, 0, 1e9, 1, &okCantidad);
    if (!okCantidad) return;

    inventario.agregarProducto(nombre.toStdString(), categoria.toStdString(), precio, cantidad);
    mostrarProductos();
}

void MainWindow::eliminarProducto() {
    int fila = ui->tablaProductos->currentRow();
    if (fila < 0) return;

    QString id = ui->tablaProductos->item(fila, 0)->text();
    if (inventario.eliminarProductoPorId(id.toStdString()))
        QMessageBox::information(this, "Éxito", "Producto eliminado.");
    else
        QMessageBox::warning(this, "Error", "No se pudo eliminar.");

    mostrarProductos();
}

void MainWindow::buscarPorCategoria() {
    QString categoria = ui->txtCategoria->text();
    if (categoria.isEmpty()) return;

    auto resultados = inventario.buscarPorCategoria(categoria.toStdString());
    ui->tablaProductos->setRowCount(resultados.size());

    for (int i = 0; i < resultados.size(); ++i) {
        ui->tablaProductos->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(resultados[i].getId())));
        ui->tablaProductos->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(resultados[i].getNombre())));
        ui->tablaProductos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(resultados[i].getCategoria())));
        ui->tablaProductos->setItem(i, 3, new QTableWidgetItem(QString::number(resultados[i].getPrecio())));
        ui->tablaProductos->setItem(i, 4, new QTableWidgetItem(QString::number(resultados[i].getCantidad())));
    }
}

void MainWindow::mostrarProductos() {
    auto lista = inventario.getProductos();
    ui->tablaProductos->setRowCount(lista.size());

    for (int i = 0; i < lista.size(); ++i) {
        ui->tablaProductos->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(lista[i].getId())));
        ui->tablaProductos->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(lista[i].getNombre())));
        ui->tablaProductos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(lista[i].getCategoria())));
        ui->tablaProductos->setItem(i, 3, new QTableWidgetItem(QString::number(lista[i].getPrecio())));
        ui->tablaProductos->setItem(i, 4, new QTableWidgetItem(QString::number(lista[i].getCantidad())));
    }
}

void MainWindow::mostrarStockBajo() {
    auto stockBajo = inventario.generarReporteStockBajo();
    if (stockBajo.empty()) {
        QMessageBox::information(this, "Stock", "No hay productos con stock bajo.");
        return;
    }
    QString reporte;
    for (const auto& linea : stockBajo)
        reporte += QString::fromStdString(linea) + "\n";

    QMessageBox::information(this, "Stock Bajo", reporte);
}

void MainWindow::editarStockProducto() {
    int fila = ui->tablaProductos->currentRow();
    if (fila < 0) return;

    QString id = ui->tablaProductos->item(fila, 0)->text();
    bool ok;
    int cambio = QInputDialog::getInt(this, "Editar Stock", "Cantidad a agregar o restar (negativo para salida):", 0, -1e9, 1e9, 1, &ok);
    if (!ok) return;

    if (inventario.actualizarStockPorId(id.toStdString(), cambio))
        QMessageBox::information(this, "Éxito", "Stock actualizado.");
    else
        QMessageBox::warning(this, "Error", "Operación inválida o stock insuficiente.");

    mostrarProductos();
}
