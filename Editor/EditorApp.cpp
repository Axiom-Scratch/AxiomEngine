#include "EditorApp.h"

#include "MainWindow.h"

#include <QApplication>

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
