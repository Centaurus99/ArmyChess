#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framework/game.h"
#include "widget/chessbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // Set chessboard in the center with a specific aspect ratio
    void resizeEvent(QResizeEvent* event);

private slots:
    void on_actionflush_triggered();

private:
    Ui::MainWindow* ui;

    // Game backend
    Game* game_;

    // Original size of chessboard
    const int Original_width = 651;
    const int Original_height = 924;
    const int Original_chess_width = 120;
    const int Original_chess_height = 60;

    // Calibration Point used to set chess
    //  0, ...,  *
    //  :   :    :
    //  *, ...,  1
    // ------------
    //  2, ...,  *
    //  :   :    :
    //  *, ...,  3
    QPoint calibration_point[4] = { QPoint { 63, 40 }, QPoint { 588, 356 },
        QPoint { 63, 558 }, QPoint { 588, 877 } };

    // The aspect ratio of the chessboard
    const float HW_ratio = (float)Original_height / (float)Original_width;
    // Width of border around centerFrame
    const int border_width = 9;

    // Buttons as chess
    ChessButton* chess_[60];
    // Original chess position and size
    QRect chess_Rect[60];

    // Calculate the original position and size of chess
    QRect GetOriginalChessRect(int x, int y);
    // Creat Chess
    void InitChess();

    // Get the icon resource name of this chess
    QString GetResourceName(Chess* chess);

    // Chess positon and icon of chess according to chessboard
    void UpdateChess(const int& number);
    void UpdateAllChess();
};
#endif // MAINWINDOW_H
