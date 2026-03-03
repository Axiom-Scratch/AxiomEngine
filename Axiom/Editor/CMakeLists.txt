find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(EditorApp
    main.cpp
    EditorApp.cpp
    MainWindow.cpp
    ViewportWidget.cpp
    EditorApp.h
    MainWindow.h
    ViewportWidget.h
)

target_link_libraries(EditorApp
    PRIVATE
        Axiom
        Qt6::Widgets
        Qt6::OpenGLWidgets
)
