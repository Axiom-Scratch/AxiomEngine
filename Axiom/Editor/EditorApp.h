#pragma once

#include <memory>

class QApplication;

namespace Axiom::Editor
{
    class MainWindow;

    class EditorApp
    {
    public:
        explicit EditorApp(QApplication& app);
        ~EditorApp();

        int Run();

    private:
        QApplication& m_App;
        std::unique_ptr<MainWindow> m_MainWindow;
    };
}
