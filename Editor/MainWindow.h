#pragma once

#include <QMainWindow>

class ViewportWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    ViewportWidget* m_Viewport = nullptr;
};
