#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    memset(enable_, 0, sizeof(enable_));
    ui->setupUi(this);
    game_ = new Game;
    InitChess();
}

MainWindow::~MainWindow() {
    delete ui;
    delete game_;
}

QRect MainWindow::GetOriginalChessRect(int x, int y) {
    QPoint p1 = calibration_point[x < 6 ? 0 : 2],
           p2 = calibration_point[x < 6 ? 1 : 3];
    if (x >= 6)
        x -= 6;

    // Center position
    QPoint pos { (int)round(p1.x() + (float)(p2.x() - p1.x()) * y / 4.0),
        (int)round(p1.y() + (float)(p2.y() - p1.y()) * x / 5.0) };
    // Upper left corner position
    pos -= QPoint { Original_chess_width / 2, Original_chess_height / 2 };

    return QRect { pos, QSize { Original_chess_width, Original_chess_height } };
}

void MainWindow::InitChess() {
    for (int i = 0; i < 60; ++i) {
        chess_[i] = new ChessButton(i, ui->centerFrame);
        connect(chess_[i], &ChessButton::chess_clicked, this,
            &MainWindow::on_chess_clicked);
        chess_Rect[i]
            = GetOriginalChessRect(game_->nodes[i].x(), game_->nodes[i].y());
    }
}

QString MainWindow::GetResourceName(Chess* chess) {
    if (chess == nullptr)
        return "";
    if (chess->hidden)
        return ":/image/png/hidden_chess.png";
    char filename[50];
    sprintf(filename, ":/image/png/%d_%d.png", chess->role, chess->camp);
    return QString(filename);
}

void MainWindow::UpdateChessEnable(const int& number) {
    qDebug() << "UpdateEnable:" << number;

    chess_[number]->setEnabled(enable_[number]);
}

void MainWindow::UpdateAllChessEnable() {
    for (int i = 0; i < 60; ++i) {
        UpdateChessEnable(i);
    }
    ui->centerFrame->update();
}

void MainWindow::SetChessEnable(const int& number, const bool& is_enable) {
    enable_[number] = is_enable;
    UpdateChessEnable(number);
}

void MainWindow::SetAllChessEnable(const bool& is_enable) {
    memset(enable_, is_enable, sizeof(enable_));
    UpdateAllChessEnable();
}

void MainWindow::ChessEnableSyncWithGame() {
    for (int i = 0; i < 60; ++i) {
        enable_[i] = (game_->nodes[i].chess != nullptr);
        UpdateChessEnable(i);
    }
    ui->centerFrame->update();
}

void MainWindow::UpdateChess(const int& number) {
    qDebug() << "Update:" << number;

    // Reload icon resource
    chess_[number]->loadIcon(GetResourceName(game_->nodes[number].chess));

    // Update Ability
    UpdateChessEnable(number);

    // Set suitable position and size
    float width_ratio = (float)ui->centerFrame->width() / Original_width;
    float height_ratio = (float)ui->centerFrame->height() / Original_height;
    chess_[number]->setGeometry(chess_Rect[number].x() * width_ratio,
        chess_Rect[number].y() * height_ratio,
        chess_Rect[number].width() * width_ratio,
        chess_Rect[number].height() * height_ratio);

    // Set icon
    chess_[number]->setIcon();
}

void MainWindow::UpdateAllChess() {
    for (int i = 0; i < 60; ++i) {
        UpdateChess(i);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {

    ui->horizontalSpacer->changeSize(0, 0);
    ui->horizontalSpacer_2->changeSize(0, 0);
    ui->verticalSpacer->changeSize(0, 0);
    ui->verticalSpacer_2->changeSize(0, 0);

    float remaining_width = ui->centralwidget->width() - border_width * 2.0;
    float remaining_height = ui->centralwidget->height() - border_width * 2.0;

    // qDebug() << remaining_width << ':' << remaining_height;

    if (remaining_height <= remaining_width * HW_ratio) {
        // Height is not enough
        int delta
            = round((remaining_width - remaining_height / HW_ratio) / 2.0);
        // qDebug() << delta;
        ui->horizontalSpacer->changeSize(delta, 0);
        ui->horizontalSpacer_2->changeSize(delta, 0);
    } else {
        // Width is not enough
        int delta
            = round((remaining_height - remaining_width * HW_ratio) / 2.0);
        // qDebug() << delta;
        ui->verticalSpacer->changeSize(0, delta);
        ui->verticalSpacer_2->changeSize(0, delta);
    }

    UpdateAllChess();
    ui->horizontalLayout->update();
    ui->verticalLayout->update();

    //    qDebug() << ui->centerFrame->width() << ':' <<
    //    ui->centerFrame->height(); qDebug() << ui->centralwidget->width() <<
    //    ':' << ui->centralwidget->height()
    //             << '\n';
}

void MainWindow::on_actionflush_triggered() {
    UpdateAllChess();
    ui->centerFrame->update();
    ChessEnableSyncWithGame();
}

void MainWindow::on_chess_clicked(const int& number) {
    qDebug() << "Clicked:" << number;
    if (slect_ == -1) {
        if (game_->nodes[number].chess->hidden) {
            game_->nodes[number].chess->hidden = 0;
            UpdateChess(number);
        } else {
            slect_ = number;
            SetAllChessEnable(0);
            SetChessEnable(number, 1);
        }
    } else {
        if (slect_ == number) {
            slect_ = -1;
            ChessEnableSyncWithGame();
        }
    }
}

void MainWindow::on_actionflushframe_triggered() { ui->centerFrame->update(); }
