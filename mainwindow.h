#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Inventario.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "ui_mainwindow.h"



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
    void editarStockMinimo();

private:
    Ui::MainWindow *ui;
    Inventario inventario;
    QChartView* chartView;

    void actualizarGraficoCategoria();
    void actualizarGraficoStock(const Producto& p);
};

#endif // MAINWINDOW_H
