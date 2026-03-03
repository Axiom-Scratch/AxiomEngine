#include "Editor/EditorApp.h"

#include "Core/Log.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char** argv)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    Axiom::Log::Init();
    Axiom::Editor::EditorApp editor(app);
    return editor.Run();
}
