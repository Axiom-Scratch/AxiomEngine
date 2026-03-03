find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(Editor
    main.cpp
    EditorApp.cpp
    MainWindow.cpp
    ViewportWidget.cpp
)

target_include_directories(Editor
    PRIVATE
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(Editor
    PRIVATE
        Axiom
        Qt6::Widgets
        Qt6::OpenGLWidgets
)
