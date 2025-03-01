#include <Universe/Widgets/Dock.hpp>
#include <Utils/utils.hpp>

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QSequentialAnimationGroup>
#include <QVBoxLayout>

DockWidget::DockWidget(QWidget* parent) : QWidget(parent), layout(new QHBoxLayout()), animationGroup(nullptr) {

    setFixedHeight(80);
    setAttribute(Qt::WA_TranslucentBackground);

    auto* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(20);
    effect->setColor(QColor(0, 0, 0, 80));
    effect->setOffset(0, 2);
    setGraphicsEffect(effect);

    layout->setSpacing(15);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setAlignment(Qt::AlignCenter);

    auto* container = new QWidget(this);
    container->setLayout(layout);
    container->setAttribute(Qt::WA_TranslucentBackground);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(container, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    setLayout(mainLayout);

    setStyleSheet(getStyle("dock.qss"));
}

void DockWidget::addButton(const QIcon& icon, std::function<void()> onClick) {
    auto* button = new QPushButton();
    button->setIcon(icon);
    button->setIconSize(QSize(40, 40));
    button->setFixedSize(50, 50);
    button->setFlat(true);

    connect(button, &QPushButton::clicked, this, [cb = std::move(onClick)]() {
        if (cb) cb();
    });

    layout->addWidget(button);
    buttons.append(button);

    updateLayout();
    // animateButtons();
}

void DockWidget::updateLayout() {
    layout->setAlignment(Qt::AlignCenter);
    layout->update();
}

// void DockWidget::animateButtons() {
//     int count = buttons.size();
//     int spacing = 70;
//
//     if (animationGroup && animationGroup->state() == QAbstractAnimation::Running) {
//         animationGroup->stop();
//     }
//
//     animationGroup = new QSequentialAnimationGroup(this);
//
//     for (int i = 0; i < count; i++) {
//         QPropertyAnimation* anim = new QPropertyAnimation(buttons[i], "geometry");
//         anim->setDuration(300);
//         anim->setEasingCurve(QEasingCurve::OutCubic);
//         anim->setEndValue(QRect(i * spacing, 0, 50, 50));
//         animationGroup->addAnimation(anim);
//     }
//
//     animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
// }

void DockWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(255, 255, 255, 150));
    gradient.setColorAt(1.0, QColor(255, 255, 255, 100));

    painter.setBrush(gradient);
    painter.setPen(QColor(255, 255, 255, 90));

    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 20, 20);
}
