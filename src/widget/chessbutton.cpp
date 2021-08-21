#include "chessbutton.h"
#include <QGraphicsDropShadowEffect>

void ChessButton::loadIcon(const QString& res_name) { icon_.load(res_name); }

void ChessButton::setIcon() {
    QIcon icon;
    if (!icon_.isNull()) {
        icon.addPixmap(icon_.scaled(width() - 8, height() - 8), QIcon::Normal);
        icon.addPixmap(
            icon_.scaled(width() - 8, height() - 8), QIcon::Disabled);
    }
    QPushButton::setIcon(icon);
    setIconSize(size());
}

ChessButton::ChessButton(
    const int& number, QWidget* parent, const QString& icon_name)
    : QPushButton { parent }
    , number_ { number } {

    // Set shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(10);
    effect->setColor(Qt::black);
    effect->setOffset(0, 3);
    setGraphicsEffect(effect);

    // Set Cursor
    setCursor(QCursor(Qt::PointingHandCursor));

    // Make it Transparent
    setFlat(true);

    // Set icon
    loadIcon(icon_name);
    if (icon_name != "") {
        setIcon();
    }

    // Connect signal
    connect(this, &ChessButton::clicked, this,
        [&]() { emit chess_clicked(number_); });
}

void ChessButton::paintEvent(QPaintEvent* event) {
    QPushButton::paintEvent(event);
    if (mark_) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::black);
        p.setBrush(QColor(150, 255, 150, 222));
        if (special_mark_) {
            p.translate(width() / 2 - 6, height() / 2 - 6);
            p.drawRect(0, 0, 12, 12);
        } else {
            p.translate(width() / 2 - 5, height() / 2 - 5);
            p.drawEllipse(0, 0, 10, 10);
        }
    }
}

void ChessButton::setMarked(const bool& mark) {
    mark_ = mark;
    special_mark_ = 0;
    update();
}

void ChessButton::setSpeciallyMarked(const bool& mark) {
    special_mark_ = mark;
    update();
}
