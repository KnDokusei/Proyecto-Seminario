QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InventarioQt
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           Producto.cpp \
           Inventario.cpp \
           Utilidades.cpp

HEADERS += mainwindow.h \
           Producto.h \
           Inventario.h \
           Utilidades.h

FORMS += mainwindow.ui
