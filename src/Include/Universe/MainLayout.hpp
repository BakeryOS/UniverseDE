#pragma once

#include <QVBoxLayout>
#include <Universe/Widgets/Dock.hpp>

class MainLayout : public QVBoxLayout {
  public:
    explicit MainLayout(QWidget* parent = nullptr);
    DockWidget* dock();

  protected:
    DockWidget* dockWidget;
};