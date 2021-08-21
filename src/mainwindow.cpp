#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    memset(enable_, 0, sizeof(enable_));
    ui->setupUi(this);
    ui->centerFrame->installEventFilter(this);
    game_ = new Game;
    InitChess();
    delete game_;
    game_ = nullptr;
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
            &MainWindow::chess_clicked);
        chess_Rect[i]
            = GetOriginalChessRect(game_->nodes[i].x(), game_->nodes[i].y());
    }
}

QString MainWindow::GetResourceName(Chess* chess) {
    if (chess == nullptr || !in_game_)
        return "";
    if (chess->hidden)
        return ":/image/png/hidden_chess.png";
    char filename[50];
    sprintf(filename, ":/image/png/%d_%d.png", chess->role, chess->camp);
    return QString(filename);
}

void MainWindow::UpdateChessEnable(const int& number) {
    //    qDebug() << "UpdateEnable:" << number;
    if (!buttton_lock)
        chess_[number]->setEnabled(enable_[number]);

    ui->centerFrame->update();
}

void MainWindow::UpdateAllChessEnable() {
    for (int i = 0; i < 60; ++i) {
        UpdateChessEnable(i);
    }
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
        Chess* chess = game_->nodes[i].chess;
        if (chess == nullptr || !chess->isMovable()) {
            enable_[i] = 0;
        } else {
            enable_[i] = 1;
        }
        UpdateChessEnable(i);
    }
}

void MainWindow::EnableAccessibleChess(const int& number) {
    SetAllChessEnable(0);
    SetChessEnable(number, 1);
    std::vector<int> list = game_->GetList(number);
    for (auto x : list) {
        SetChessEnable(x, 1);
    }
}

void MainWindow::UpdateChess(const int& number) {
    //    qDebug() << "Update:" << number;

    // Reload icon resource
    if (in_game_)
        chess_[number]->loadIcon(GetResourceName(game_->nodes[number].chess));
    else
        chess_[number]->loadIcon("");

    // Set suitable position and size
    float width_ratio = (float)ui->centerFrame->width() / Original_width;
    float height_ratio = (float)ui->centerFrame->height() / Original_height;
    chess_[number]->setGeometry(chess_Rect[number].x() * width_ratio,
        chess_Rect[number].y() * height_ratio,
        chess_Rect[number].width() * width_ratio,
        chess_Rect[number].height() * height_ratio);

    // Set icon
    chess_[number]->setIcon();

    // Update Ability
    UpdateChessEnable(number);
}

void MainWindow::UpdateAllChess() {
    qDebug() << "[UpdateAllChess]" << ui->centerFrame->width() << ':'
             << ui->centerFrame->height();
    for (int i = 0; i < 60; ++i) {
        UpdateChess(i);
    }
}

void MainWindow::GameClear() {
    delete game_;
    game_ = new Game;
    buttton_lock = 0;
    select_ = -1;
    SetAllChessEnable(0);
}

void MainWindow::StartGame() {
    GameClear();
    in_game_ = 1;
    UpdateAllChess();
    ui->centerFrame->update();
    BeforeTurn();
}

void MainWindow::EndGame(const int& winner) {
    in_game_ = 0;
    SetAllChessEnable(0);
    QString winner_name = winner == 0 ? "我方" : "对方";
    QMessageBox::about(this, "游戏结束", winner_name + "获胜！  ");
}

void MainWindow::BeforeTurn() {
    game_->BeforeTurn();
    qDebug() << "[BeforeTurn]"
             << "CurrentPlayer:" << game_->GetCurrentPlayer();
    qDebug() << "[BeforeTurn]"
             << "OwnCamp:" << game_->GetOwnCamp();
    ChessEnableSyncWithGame();
    ui->label_2->setText(
        game_->GetCurrentPlayer() == 0 ? "我方走子" : "对方走子");
    switch (game_->GetOwnCamp()) {
    case -1: {
        ui->label_3->setText("阵营：待定");
        break;
    }
    case 0: {
        ui->label_3->setText("阵营：蓝色");
        break;
    }
    case 1: {
        ui->label_3->setText("阵营：红色");
        break;
    }
    }
}

void MainWindow::AfterTurn() {
    game_->AfterTurn();
    if (game_->GetWinner() != -1) {
        EndGame(game_->GetWinner());
        return;
    }
    BeforeTurn();
}

void MainWindow::Resize() {

    ui->horizontalSpacer->changeSize(0, 0);
    ui->horizontalSpacer_2->changeSize(0, 0);
    ui->verticalSpacer->changeSize(0, 0);
    ui->verticalSpacer_2->changeSize(0, 0);

    float remaining_width = ui->centralwidget->width() - border_width * 2.0;
    float remaining_height = ui->centralwidget->height() - border_height * 2.0;

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

    ui->centerFrame->update();
    ui->horizontalLayout->update();
    ui->verticalLayout->update();

    qDebug() << ui->centerFrame->width() << ':' << ui->centerFrame->height();
    qDebug() << ui->centralwidget->width() << ':' << ui->centralwidget->height()
             << '\n';
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {

    // Change buttons size when centerFrame resized
    if (obj == ui->centerFrame && event->type() == QEvent::Resize) {
        qDebug() << "[centerFrame]"
                 << "Resize!";
        UpdateAllChess();
        qDebug() << "[centerFrame]" << chess_[0]->geometry();
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::resizeEvent(QResizeEvent* event) { Resize(); }

void MainWindow::on_actionstart_triggered() { StartGame(); }

void MainWindow::chess_clicked(const int& number) {
    qDebug() << "Clicked:" << number;
    if (select_ == -1) {
        if (game_->nodes[number].chess->hidden) {
            game_->TurnOver(number);
            UpdateChess(number);
            AfterTurn();
        } else {
            select_ = number;
            EnableAccessibleChess(number);
        }
    } else {
        if (select_ == number) {
            select_ = -1;
            ChessEnableSyncWithGame();
        } else {
            game_->Capture(select_, number);
            UpdateChess(select_);
            UpdateChess(number);
            select_ = -1;
            AfterTurn();
        }
    }
}

void MainWindow::on_actionflushframe_triggered() {
    UpdateAllChess();
    ui->centerFrame->update();
}
