// This file is part of qAccordion. An Accordion widget for Qt
// Copyright © 2015 Christian Rapp <0x2a at posteo dot org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumSize(QSize(640, 480));
    this->setWindowIcon(QPixmap(":/icons/accordion_cc_grey.svg"));

    int indexFirst = ui->widget->addContentPane("FirstTest");
    QFrame *firstFrame = ui->widget->getContentPane(indexFirst);
    firstFrame->setLayout(new QVBoxLayout());
    firstFrame->layout()->addWidget(new QLabel("Ich bin ein Bi Ba Butzemann"));
    dynamic_cast<QVBoxLayout *>(firstFrame->layout())->addStretch();
    ui->widget->addContentPane("SecondTest");
    ui->widget->insertContentPane("InsertAfterOne", 1);

    QObject::connect(ui->pushButton, &QPushButton::clicked, [this]() {
        // this->ui->widget->removeContentPane(uint(0));
        QFrame *frame = new QFrame();
        frame->setLayout(new QVBoxLayout());
        frame->layout()->addWidget(
            new QLabel("This frame is swapping content of InsertAfter One"));
        frame->layout()->addWidget(new QLabel("Here is a second label"));
        ui->widget->swapContentPane(1, frame);
    });

    QObject::connect(ui->pushButton_2, &QPushButton::clicked, [this]() {
        QFrame *f = ui->widget->getContentPane(0);
        qDebug() << Q_FUNC_INFO << "2Height: " << f->height();
        qDebug() << Q_FUNC_INFO
                 << "2SizeHint Height: " << f->sizeHint().height();
    });

    QObject::connect(ui->pushButton_3, &QPushButton::clicked, [this]() {
        QFrame *f = ui->widget->getContentPane(1);
        qDebug() << Q_FUNC_INFO << "3Height: " << f->height();
        qDebug() << Q_FUNC_INFO
                 << "3SizeHint Height: " << f->sizeHint().height();
    });
}

MainWindow::~MainWindow() { delete ui; }
