#ifndef SERVER_DIALOG_H
#define SERVER_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <vector>

namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog {
    Q_OBJECT

public:
    /// @param IPs Host IPs
    /// @param port Server port
    /// @param parent QObject parent
    ServerDialog(const std::vector<QString>& IPs, const qint16& port,
        QWidget* parent = nullptr);
    ~ServerDialog();

private:
    Ui::ServerDialog* ui;
};

#endif // SERVER_DIALOG_H
