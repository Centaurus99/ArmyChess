#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framework/game.h"
#include "network/network.h"
#include "widget/chessbutton.h"
#include "widget/connect_dialog.h"
#include "widget/server_dialog.h"
#include <QMainWindow>
#include <QTimer>

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

    bool eventFilter(QObject* obj, QEvent* event);

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void on_actionstart_triggered();

    void chess_clicked(const int& number);

    void on_actionsurrender_triggered();

    void TimeMaintain();

    void on_actionCreateServer_triggered();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    // Work as Client, try to connect
    void ClientConnect(const QString& ip, const qint16& port);

private:
    Ui::MainWindow* ui;

    // Game backend
    Game* game_;
    // whether game is in process
    bool in_game_ = 0;
    // Count the number of steps
    int step_count_ = 0;
    // whether button ability is locked
    bool buttton_lock_ = 0;

    // whether in the online mode
    bool online_mode_ = 0;
    // Whether this side is server
    bool is_server_ = 0;
    // Network socket
    Network* socket_ = nullptr;

    // Remaining timeout times
    int timeout_remain_[2] = { 3, 3 };
    // Remaining time
    int remaining_time_ = 0;
    // Timer
    QTimer* timer;

    // Which button has been slected
    //     -1 : None
    // 0 ~ 59 : Corresponding button
    int select_ = -1;

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
    const int border_height = 46;

    // Buttons as chess
    ChessButton* chess_[60];
    // Original chess position and size
    QRect chess_Rect[60];

    // Control whether button is enabled
    // Default value is False
    bool enable_[60];

    // Calculate the original position and size of chess
    QRect GetOriginalChessRect(int x, int y);
    // Creat Chess
    void InitChess();

    // Get the icon resource name of this chess
    QString GetResourceName(Chess* chess);

    // Update whether button is enabled
    void UpdateChessEnable(const int& number);
    void UpdateAllChessEnable();

    // Set whether button is enabled
    void SetChessEnable(const int& number, const bool& is_enable);
    void SetAllChessEnable(const bool& is_enable);

    // Set whether button is marked
    void SetChessMarked(const int& number, const bool& is_marked);
    void SetAllChessMarked(const bool& is_marked);

    // Enable decided by whether here has chess on the node and
    // whether current player can manipulate this chess
    void ChessEnableSyncWithGame();

    // When select_ is not -1, use this to mark accessible chess
    void EnableAccessibleChess(const int& number);

    // Update positon and icon of chess according to chessboard
    void UpdateChess(const int& number);
    void UpdateAllChess();

    // Game mantain function
    void GameClear();
    void StartGame();
    void EndGame(const int& winner);
    void BeforeTurn();
    void AfterTurn();

    // Surrender
    void Surrender(const int& player);

    // Set chessboard in the center with a specific aspect ratio
    void Resize();

    // Start countdown
    void TimeStart();

    // Toggle online mode
    void StartOnline(const bool& is_server);
    void EndOnline();
};
#endif // MAINWINDOW_H
