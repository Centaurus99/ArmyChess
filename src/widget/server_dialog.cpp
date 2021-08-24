#include "server_dialog.h"
#include "ui_server_dialog.h"

ServerDialog::ServerDialog(
    const std::vector<QString>& IPs, const qint16& port, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ServerDialog) {

    ui->setupUi(this);

    // Set IP labels
    for (const auto& ip : IPs) {
        QLabel* iplabel = new QLabel(ui->groupBox);
        iplabel->setObjectName(QString::fromUtf8("label"));
        iplabel->setAlignment(Qt::AlignCenter);
        iplabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        iplabel->setText(ip);
        ui->verticalLayout_2->addWidget(iplabel);
    }
    // Set port label
    ui->label->setText(QString::number(port));
}

ServerDialog::~ServerDialog() { delete ui; }
