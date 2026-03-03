#include "Editor/EditorApp.h"

#include "Editor/MainWindow.h"

#include <QApplication>

namespace Axiom::Editor
{
    EditorApp::EditorApp(QApplication& app)
        : m_App(app), m_MainWindow(std::make_unique<MainWindow>())
    {
    }

    EditorApp::~EditorApp() = default;

    int EditorApp::Run()
    {
        m_MainWindow->show();
        return m_App.exec();
    }
}
