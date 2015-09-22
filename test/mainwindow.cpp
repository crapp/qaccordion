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

    // create a network access manager to get some lorem ipsum :)
    this->networkManager =
        std::unique_ptr<QNetworkAccessManager>(new QNetworkAccessManager());
    QObject::connect(this->networkManager.get(),
                     &QNetworkAccessManager::finished,
                     this,
                     &MainWindow::networkRequestFinished);

    ui->scrollAreaWidgetAccordion->addContentPane("Pane1");

    int indexAddPane = ui->widgetControlAccordion->addContentPane("Add Pane");
    int indexInsertPane =
        ui->widgetControlAccordion->addContentPane("Insert Pane");
    int indexRemovePane =
        ui->widgetControlAccordion->addContentPane("Remove Pane");

    QFrame *addPane = ui->widgetControlAccordion->getContentPane(indexAddPane);
    addPane->setLayout(new QVBoxLayout());
    QLineEdit *headerName = new QLineEdit();
    headerName->setPlaceholderText("Header name");
    addPane->layout()->addWidget(headerName);
    QPushButton *addPaneButton = new QPushButton("Add Content Pane");
    QObject::connect(addPaneButton, &QPushButton::clicked, [this, headerName]() {
        if (headerName->text() != "") {
            QNetworkRequest quest;
            quest.setUrl(QUrl(this->ipsumApi));
            this->networkManager->get(quest);

            QFrame *frame = new QFrame();
            frame->setLayout(new QVBoxLayout());
            QLabel *ipsumLabel = new QLabel();
            frame->layout()->addWidget(ipsumLabel);
            this->labelIpsumQueue.push(ipsumLabel);
            this->ui->scrollAreaWidgetAccordion->addContentPane(
                headerName->text(), frame);
            dynamic_cast<QVBoxLayout *>(frame->layout())->addStretch();
        }
    });
    addPane->layout()->addWidget(addPaneButton);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::networkRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        QByteArray data = reply->readAll();
        QLabel *ipsumLabel = this->labelIpsumQueue.front();
        this->labelIpsumQueue.pop();
        ipsumLabel->setTextFormat(Qt::TextFormat::RichText);
        ipsumLabel->setWordWrap(true);
        ipsumLabel->setText(QString(data));
        qDebug() << Q_FUNC_INFO << data;
    }
    reply->deleteLater();
}

void MainWindow::networkRequestError()
{
//    qDebug() << reply;
//    reply->deleteLater();
}
