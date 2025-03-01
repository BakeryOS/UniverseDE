#pragma once

#include <core.hpp>

class DockWidget : public QWidget {
    Q_OBJECT

  public:
    QList<QPushButton*> buttons;
    explicit DockWidget(QWidget* parent = nullptr);

    void addButton(const QIcon& icon, std::function<void()> onClick);
    // void animateButtons();

  protected:
    void paintEvent(QPaintEvent* event) override;

    void updateLayout();
    QSequentialAnimationGroup* animationGroup;

    QHBoxLayout* layout;
};
