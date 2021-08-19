#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow* ui;

    // The aspect ratio of the chessboard
    const float HW_ratio = 924.0 / 651.0;
    // Width of border around centerFrame
    const int border_width = 9;
};
#endif // MAINWINDOW_H
