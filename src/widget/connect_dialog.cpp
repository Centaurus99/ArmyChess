#include "connect_dialog.h"
#include "ui_connect_dialog.h"
#include <QRegExp>

ConnectDialog::ConnectDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ConnectDialog) {
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
        &ConnectDialog::TryConnect);
}

ConnectDialog::~ConnectDialog() { delete ui; }

bool ConnectDialog::Check() {
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->label->setPalette(pa);
    QString ip = ui->lineEdit->text();
    qDebug() << "IP:" << ip;
    QRegExp re("((1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.){3}(1\\d{2}|2[0-"
               "4]\\d|25[0-5]|[1-9]\\d|\\d)");
    if (!re.exactMatch(ip)) {
        ui->label->setText("IP地址格式错误");
        return 0;
    }
    bool ok;
    int port = ui->lineEdit_2->text().toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        ui->label->setText("端口错误");
        return 0;
    }
    ui->label->setText("");
    return 1;
}

void ConnectDialog::TryConnect() {
    if (Check()) {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::black);
        ui->label->setPalette(pa);
        ui->label->setText(
            "正在连接 " + ui->lineEdit->text() + ":" + ui->lineEdit_2->text());
        bool ok;
        emit Connect(ui->lineEdit->text(), ui->lineEdit_2->text().toInt(&ok));
    }
}

void ConnectDialog::ConnectionFail(const QString& msg) {
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::black);
    ui->label->setPalette(pa);
    ui->label->setText(msg);
}
