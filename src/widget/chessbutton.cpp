#include "chessbutton.h"
#include <QGraphicsDropShadowEffect>

ChessButton::ChessButton(QWidget* parent)
    : QPushButton { parent } {

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(10); // 阴影圆角的大小
    effect->setColor(Qt::black); //阴影的颜色
    effect->setOffset(0, 3);
    //    QPushButton* this = new QPushButton(ui->centerFrame);

    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setGraphicsEffect(effect);
    this->setGeometry(20, 20, 100, 100);
    this->setFlat(true);
    /*    QPixmap icon(":/image/png/vacant_chess.png");
        qDebug() << icon.width() << ':' << icon.height();
        icon = icon.scaled(this->width() - 10, this->height() - 10);
        qDebug() << icon.width() << ':' << icon.height();*/

    QIcon icon;

    icon.addPixmap(QPixmap(":/image/png/vacant_chess.png")
                       .scaled(this->width() - 8, this->height() - 8),
        QIcon::Normal);
    icon.addPixmap(QPixmap(":/image/png/vacant_chess.png")
                       .scaled(this->width() - 8, this->height() - 8),
        QIcon::Disabled);
    this->setIcon(icon);
    this->setIconSize(this->size());
    //    this->setEnabled(false);
    /*this->setStyleSheet(
        "QPushButton{border-image: url(:/image/png/vacant_chess.png);}"
        "QPushButton:hover{border-image: url(:/image/png/vacant_node.png);}"
        "QPushButton:pressed{border-image: "
        "url(:/image/png/vacant_camp.png);}");*/
}
