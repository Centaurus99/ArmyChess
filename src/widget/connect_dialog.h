#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog {
    Q_OBJECT

signals:
    // Notify the main program to connect
    void Connect(const QString& ip, const int& port);

private:
    Ui::ConnectDialog* ui;

    // Check whether input is legal
    bool Check();

private slots:
    // Try to connect
    void TryConnect();

public slots:
    // The main program failed to connect
    void ConnectionFail(const QString& msg = "连接失败，请重试");

public:
    explicit ConnectDialog(QWidget* parent = nullptr);
    ~ConnectDialog();
};

#endif // CONNECT_DIALOG_H
