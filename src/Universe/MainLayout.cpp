#include <Universe/MainLayout.hpp>

MainLayout::MainLayout(QWidget* parent) : QVBoxLayout(parent) {
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);

    QWidget* bgWidget = new QWidget();
    bgWidget->setAutoFillBackground(true);

    QPixmap backgroundPixmap(":/Wallpaper.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundPixmap);
    bgWidget->setPalette(palette);
    bgWidget->setFixedSize(backgroundPixmap.size());

    addWidget(bgWidget);
    QPushButton* btn = new QPushButton("Hello World!", bgWidget);
    QVBoxLayout* bgLayout = new QVBoxLayout(bgWidget);
    bgLayout->addWidget(btn);
    bgLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    dockWidget = new DockWidget();
    bgLayout->addWidget(dockWidget);
}

DockWidget* MainLayout::dock() { return dockWidget; }