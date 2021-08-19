#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent* event) {

    ui->horizontalSpacer->changeSize(0, 0);
    ui->horizontalSpacer_2->changeSize(0, 0);
    ui->verticalSpacer->changeSize(0, 0);
    ui->verticalSpacer_2->changeSize(0, 0);

    float remaining_width = ui->centralwidget->width() - border_width * 2.0;
    float remaining_height = ui->centralwidget->height() - border_width * 2.0;

    //    qDebug() << remaining_width << ':' << remaining_height;

    if (remaining_height <= remaining_width * HW_ratio) {
        // Height is not enough
        int delta
            = round((remaining_width - remaining_height / HW_ratio) / 2.0);
        qDebug() << delta;
        ui->horizontalSpacer->changeSize(delta, 0);
        ui->horizontalSpacer_2->changeSize(delta, 0);
    } else {
        // Width is not enough
        int delta
            = round((remaining_height - remaining_width * HW_ratio) / 2.0);
        qDebug() << delta;
        ui->verticalSpacer->changeSize(0, delta);
        ui->verticalSpacer_2->changeSize(0, delta);
    }

    ui->horizontalLayout->update();
    ui->verticalLayout->update();

    //    qDebug() << ui->centerFrame->width() << ':' <<
    //    ui->centerFrame->height(); qDebug() << ui->centralwidget->width() <<
    //    ':' << ui->centralwidget->height()
    //             << '\n';
}
