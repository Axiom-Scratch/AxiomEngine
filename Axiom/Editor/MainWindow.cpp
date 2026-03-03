#include "Editor/MainWindow.h"

#include "Editor/ViewportWidget.h"

namespace Axiom::Editor
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowTitle("Axiom Editor");
        resize(1280, 720);

        m_Viewport = new ViewportWidget(this);
        setCentralWidget(m_Viewport);
    }
}
