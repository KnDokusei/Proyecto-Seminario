#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QMap>
#include <algorithm>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>



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
    connect(ui->btnEditarMinimo, &QPushButton::clicked, this, &MainWindow::editarStockMinimo);

    connect(ui->tablaProductos, &QTableWidget::itemSelectionChanged, this, [this]() {
        int fila = ui->tablaProductos->currentRow();
        if (fila < 0) {
            actualizarGraficoCategoria();
            return;
        }

        QString id = ui->tablaProductos->item(fila, 0)->text();
        Producto* p = inventario.buscarPorId(id.toStdString());
        if (p) actualizarGraficoStock(*p);
    });

    QStringList headers = {"ID", "Nombre", "Categoría", "Precio", "Cantidad"};
    ui->tablaProductos->setColumnCount(5);
    ui->tablaProductos->setHorizontalHeaderLabels(headers);
    ui->tablaProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Crear y conectar el gráfico visual
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Reemplaza el placeholder en el layout por el gráfico real
    auto layout = qobject_cast<QHBoxLayout*>(ui->centralwidget->layout());
    layout->replaceWidget(ui->chartPlaceholder, chartView);
    ui->chartPlaceholder->hide();

    // Estilo visual
    setStyleSheet("QPushButton { background-color: #D0E8FF; } "
                  "QLineEdit { background-color: #FFF; padding: 2px; } "
                  "QTableWidget { background-color: #FFF; gridline-color: #CCC; }");

    actualizarGraficoCategoria();
    // Cargar automáticamente archivo anterior si existe
    std::string rutaAnterior = inventario.obtenerRutaUltimoArchivo();
    if (!rutaAnterior.empty()) {
        QFile file(QString::fromStdString(rutaAnterior));
        if (file.exists()) {
            inventario.cargarDesdeArchivo(rutaAnterior);
            mostrarProductos();
        }
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::cargarInventario() {
    QString archivo = QFileDialog::getOpenFileName(this, "Abrir Inventario", "", "CSV (*.csv)");
    if (!archivo.isEmpty()) {
        std::string ruta = archivo.toStdString();

        if (inventario.cargarDesdeArchivo(ruta)) {
            QMessageBox::information(this, "Éxito", "Inventario cargado.");
            inventario.guardarRutaUltimoArchivo(ruta);  // ← Guarda la ruta para futuras sesiones
            mostrarProductos();
        } else {
            QMessageBox::warning(this, "Error", "No se pudo cargar el archivo.");
        }
    }
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

    for (int i = 0; i < static_cast<int>(lista.size()); ++i) {
        const auto& prod = lista[i];
        int cantidad = prod.getCantidad();

        QTableWidgetItem* itemID = new QTableWidgetItem(QString::fromStdString(prod.getId()));
        QTableWidgetItem* itemNombre = new QTableWidgetItem(QString::fromStdString(prod.getNombre()));
        QTableWidgetItem* itemCategoria = new QTableWidgetItem(QString::fromStdString(prod.getCategoria()));
        QTableWidgetItem* itemPrecio = new QTableWidgetItem(QString::number(prod.getPrecio()));
        QTableWidgetItem* itemCantidad = new QTableWidgetItem(QString::number(cantidad));

        // Si el stock es menor que 5, marcar toda la fila con rojo suave
        if (cantidad < 5) {
            QColor rojoSuave(255, 180, 180); // fondo rojo claro legible
            itemID->setBackground(rojoSuave);
            itemNombre->setBackground(rojoSuave);
            itemCategoria->setBackground(rojoSuave);
            itemPrecio->setBackground(rojoSuave);
            itemCantidad->setBackground(rojoSuave);
        }

        ui->tablaProductos->setItem(i, 0, itemID);
        ui->tablaProductos->setItem(i, 1, itemNombre);
        ui->tablaProductos->setItem(i, 2, itemCategoria);
        ui->tablaProductos->setItem(i, 3, itemPrecio);
        ui->tablaProductos->setItem(i, 4, itemCantidad);
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

void MainWindow::actualizarGraficoCategoria() {
    QPieSeries* series = new QPieSeries();
    QMap<QString, int> conteo;

    for (const auto& p : inventario.getProductos())
        conteo[QString::fromStdString(p.getCategoria())]++;

    for (auto it = conteo.begin(); it != conteo.end(); ++it)
        series->append(it.key(), it.value());

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribución por Categoría");
    chart->legend()->setAlignment(Qt::AlignRight);

    chartView->setChart(chart);
}

void MainWindow::actualizarGraficoStock(const Producto& p) {
    QBarSet* actual = new QBarSet("Actual");
    QBarSet* minimo = new QBarSet("Mínimo recomendado");

    *actual << p.getCantidad();
    *minimo << 5;

    QBarSeries* series = new QBarSeries();
    series->append(actual);
    series->append(minimo);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Stock de '" + QString::fromStdString(p.getNombre()) + "'");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categorias;
    categorias << "Stock";

    QBarCategoryAxis* ejeX = new QBarCategoryAxis();
    ejeX->append(categorias);
    chart->addAxis(ejeX, Qt::AlignBottom);
    series->attachAxis(ejeX);

    QValueAxis* ejeY = new QValueAxis();
    ejeY->setLabelFormat("%d");
    ejeY->setTitleText("Unidades");
    ejeY->setTickType(QValueAxis::TicksFixed);
    ejeY->setTickInterval(1);              // paso fijo de 1
    ejeY->setMinorTickCount(0);           // sin sub-divisiones

    int maxY = std::max(p.getCantidad(), 5) + 1;  // para dejar un poco de margen visual
    ejeY->setRange(0, maxY);                     // forzar eje de 0 a máximo

    ejeY->setLabelFormat("%d");  // Mostrar solo números enteros
    ejeY->setTitleText("Unidades");
    chart->addAxis(ejeY, Qt::AlignLeft);
    series->attachAxis(ejeY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView->setChart(chart);
}

void MainWindow::editarStockMinimo() {
    int fila = ui->tablaProductos->currentRow();
    if (fila < 0) return;

    QString id = ui->tablaProductos->item(fila, 0)->text();
    Producto* p = inventario.buscarPorId(id.toStdString());
    if (!p) return;

    bool ok;
    int nuevoMin = QInputDialog::getInt(this, "Stock mínimo", "Nuevo mínimo recomendado:",
                                        p->getStockMinimo(), 0, 9999, 1, &ok);
    if (ok) {
        p->setStockMinimo(nuevoMin);
        mostrarProductos();
        actualizarGraficoStock(*p);
    }
}
