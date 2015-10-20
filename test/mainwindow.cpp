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
                     &QNetworkAccessManager::finished, this,
                     &MainWindow::networkRequestFinished);

    int indexAddPane = ui->widgetControlAccordion->addContentPane("Add Pane");
    int indexInsertPane =
        ui->widgetControlAccordion->addContentPane("Insert Pane");
    int indexRemovePane =
        ui->widgetControlAccordion->addContentPane("Remove Pane");
    int indexMoveContentP =
        ui->widgetControlAccordion->addContentPane("Move Pane");

    QFrame *addPane = ui->widgetControlAccordion->getContentPane(indexAddPane)
                          ->getContentFrame();
    addPane->setLayout(new QVBoxLayout());
    QLineEdit *headerName = new QLineEdit();
    headerName->setPlaceholderText("Header name");
    addPane->layout()->addWidget(headerName);
    QPushButton *addPaneButton = new QPushButton("Add Content Pane");
    QObject::connect(addPaneButton, &QPushButton::clicked, [this, headerName]() {
        if (headerName->text() != "") {
            // create a QFrame that will hold our content
            QFrame *frame = new QFrame();
            // add a new content pane with the provided header and the content
            // frame
            if (this->ui->scrollAreaWidgetAccordion->addContentPane(
                    headerName->text(), frame) != -1) {
                // add some basic stuff to our QFrame
                frame->setLayout(new QVBoxLayout());
                QLabel *ipsumLabel = new QLabel();
                frame->layout()->addWidget(ipsumLabel);
                this->labelIpsumQueue.push(ipsumLabel);
                dynamic_cast<QVBoxLayout *>(frame->layout())->addStretch();

                QNetworkRequest rquest;
                rquest.setUrl(QUrl(this->ipsumApi));
                this->networkManager->get(rquest);
            } else {
                // if a pane with chosen header already exists we need to delete
                // the QFrame
                delete frame;
            }
        }
    });
    addPane->layout()->addWidget(addPaneButton);

    // insert content pane
    QFrame *insertPane =
        ui->widgetControlAccordion->getContentPane(indexInsertPane)
            ->getContentFrame();
    insertPane->setLayout(new QVBoxLayout());
    insertPane->layout()->addWidget(new QLabel("Ich bin ein biba butzemann"));

    QObject::connect(ui->pushButton, &QPushButton::clicked, [this]() {
        this->ui->scrollAreaWidgetAccordion->moveContentPane(0, 2);
    });
    QObject::connect(ui->pushButton_2, &QPushButton::clicked, [this]() {
        this->ui->scrollAreaWidgetAccordion->moveContentPane(2, 1);
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::networkRequestFinished(QNetworkReply *reply)
{
    QLabel *ipsumLabel = this->labelIpsumQueue.front();
    this->labelIpsumQueue.pop();
    ipsumLabel->setTextFormat(Qt::TextFormat::RichText);
    ipsumLabel->setWordWrap(true);
    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        QByteArray data = reply->readAll();
        ipsumLabel->setText(QString(data));
    } else {
        qDebug() << Q_FUNC_INFO << "Netowrk error: " << reply->error() << "\n"
                 << reply->errorString();
        ipsumLabel->setText(this->offlineIpsum);
    }
    reply->deleteLater();
}

void MainWindow::networkRequestError(QNetworkReply::NetworkError code)
{
    //    qDebug() << reply;
    //    reply->deleteLater();
}
