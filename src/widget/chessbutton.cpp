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
