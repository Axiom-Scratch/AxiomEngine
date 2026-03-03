#include "MainWindow.h"

#include "ViewportWidget.h"

#include <QDockWidget>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Axiom Editor");
    resize(1280, 720);

    m_Viewport = new ViewportWidget(this);
    setCentralWidget(m_Viewport);

    auto* dock = new QDockWidget("Scene", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(new QWidget(dock));
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}
