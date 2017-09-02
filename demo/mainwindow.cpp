// This file is part of qAccordion. An Accordion widget for Qt
// Copyright © 2015, 2017 Christian Rapp <0x2a at posteo dot org>
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

    // create a network access manager object to get some lorem ipsum :)
    this->networkManager = std::make_unique<QNetworkAccessManager>();
    QObject::connect(this->networkManager.get(),
                     &QNetworkAccessManager::finished, this,
                     &MainWindow::networkRequestFinished);

    // the demo qaccordion. The Accordion will be added programmatically
    auto *groupboxDemo = new QGroupBox();
    groupboxDemo->setTitle("Demo Accordion");
    groupboxDemo->setLayout(new QVBoxLayout());
    ui->verticalLayout->insertWidget(0, groupboxDemo);
    auto *scrollAreaTop = new QScrollArea();
    groupboxDemo->layout()->addWidget(scrollAreaTop);
    auto *topAccordion = new QAccordion();
    scrollAreaTop->setWidget(topAccordion);
    // if you are using a QScrollArea you have to tell it that the qaccrdion
    // widget is resizable
    scrollAreaTop->setWidgetResizable(true);

    // the control accordion. The Accordion was added with the Qt Designer
    // Add 4 Content Panes for Add / Insert / Remove / move operations
    this->contentPaneAdd(topAccordion);
    this->contentPaneInsert(topAccordion);
    this->contentPaneRemove(topAccordion);
    this->contentPaneMove(topAccordion);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::networkRequestFinished(QNetworkReply *reply)
{
    QLabel *ipsumLabel = this->labelIpsumQueue.front();
    this->labelIpsumQueue.pop();
    ipsumLabel->setTextFormat(Qt::TextFormat::RichText);
    ipsumLabel->setWordWrap(true);
    // use local ipsum if networkrequest failed
    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        QByteArray data = reply->readAll();
        ipsumLabel->setText(QString(data));
    } else {
        qDebug() << Q_FUNC_INFO << "Network error: " << reply->error() << "\n"
                 << reply->errorString();
        ipsumLabel->setText(this->offlineIpsum);
    }
    reply->deleteLater();
}

void MainWindow::contentPaneAdd(QAccordion *topAccordion)
{
    ui->widgetControlAccordion->setCollapsible(false);
    // good pratice is to check the return value of addContentPane. see the API
    // Reference for more details
    int indexAddPane = ui->widgetControlAccordion->addContentPane("Add Pane");

    // Get the content frame
    this->addCF = ui->widgetControlAccordion->getContentPane(indexAddPane)
                      ->getContentFrame();
    // Add controls
    this->addCF->setLayout(new QVBoxLayout());
    this->addCF->layout()->addWidget(
        new QLabel("Add a new Content Pane with the provided Header"));
    auto *headerName = new QLineEdit();
    headerName->setPlaceholderText("Header name");
    this->addCF->layout()->addWidget(headerName);
    QPushButton *addPaneButton = new QPushButton("Add Content Pane");
    // use a lambda as slot
    QObject::connect(addPaneButton, &QPushButton::clicked, [this, topAccordion,
                                                            headerName]() {
        if (headerName->text() != "") {
            // create a QFrame that acts as content frame for a new content pane
            QFrame *frame = new QFrame();
            // add a new content pane with the provided header and the content
            // frame
            int idx = topAccordion->addContentPane(headerName->text(), frame);
            if (idx != -1) {
                this->createIpsumLabel(frame);
            } else {
                this->statusBar()->showMessage(
                    "Content Pane \"" + headerName->text() + "\" already exists",
                    3000);
                // if a pane with chosen header already exists we need to delete
                // the QFrame
                delete frame;
            }
        }
    });
    this->addCF->layout()->addWidget(addPaneButton);
}

void MainWindow::contentPaneInsert(QAccordion *topAccordion)
{
    int indexInsertPane =
        ui->widgetControlAccordion->addContentPane("Insert Pane");

    // insert content pane
    this->insertCF = ui->widgetControlAccordion->getContentPane(indexInsertPane)
                         ->getContentFrame();
    this->insertCF->setLayout(new QVBoxLayout());
    this->insertCF->layout()->addWidget(new QLabel(
        "Insert a new content pane by providing a Header and a Position"));

    auto *headerPosition = new QHBoxLayout();
    dynamic_cast<QVBoxLayout *>(this->insertCF->layout())
        ->addLayout(headerPosition);
    auto *headerName = new QLineEdit();
    headerName->setPlaceholderText("Header name");
    headerPosition->addWidget(headerName);
    auto *position = new QComboBox();
    // inserting at 0 is always possible.
    position->addItem("Position 0", QVariant(0));
    QObject::connect(topAccordion, &QAccordion::numberOfContentPanesChanged,
                     [position](int number) {
                         position->clear();
                         for (int i = 0; i <= number; i++) {
                             position->addItem("Position " + QString::number(i),
                                               QVariant(i));
                         }
                     });
    headerPosition->layout()->addWidget(position);

    auto *insertPaneButton = new QPushButton("Insert Content Pane");
    this->insertCF->layout()->addWidget(insertPaneButton);
    QObject::connect(
        insertPaneButton, &QPushButton::clicked,
        [headerName, position, topAccordion, this]() {
            if (headerName->text() != "") {
                // create a QFrame that acts as content frame for a
                // new content pane
                QFrame *frame = new QFrame();
                // add a new content pane with the provided header
                // and the content frame
                bool status = topAccordion->insertContentPane(
                    position->currentData().toUInt(), headerName->text(), frame);
                if (status) {
                    this->createIpsumLabel(frame);
                } else {
                    this->statusBar()->showMessage("Content Pane \"" +
                                                       headerName->text() +
                                                       "\" already exists",
                                                   3000);
                    // if a pane with chosen header already exists we need to
                    // delete the QFrame
                    delete frame;
                }
            }
        });
}

void MainWindow::contentPaneRemove(QAccordion *topAccordion)
{
    int indexRemovePane =
        ui->widgetControlAccordion->addContentPane("Remove Pane");

    this->removeCF = ui->widgetControlAccordion->getContentPane(indexRemovePane)
                         ->getContentFrame();
    this->removeCF->setLayout(new QVBoxLayout());
    this->removeCF->layout()->addWidget(
        new QLabel("Remove a content pane by providing the Header"));
    auto *headerName = new QLineEdit();
    headerName->setPlaceholderText("Header name");
    this->removeCF->layout()->addWidget(headerName);
    QPushButton *removePaneButton = new QPushButton("Remove Content Pane");
    this->removeCF->layout()->addWidget(removePaneButton);
    QObject::connect(removePaneButton, &QPushButton::clicked, [headerName,
                                                               topAccordion,
                                                               this]() {
        if (headerName->text() != "") {
            bool status =
                topAccordion->removeContentPane(true, headerName->text());
            if (status) {
                this->statusBar()->showMessage(
                    "Content Pane \"" + headerName->text() + "\" removed", 3000);
            } else {
                this->statusBar()->showMessage(
                    "Content Pane \"" + headerName->text() + "\" does not exist",
                    3000);
            }
        }
    });
}

void MainWindow::contentPaneMove(QAccordion *topAccordion)
{
    int indexMoveContentP =
        ui->widgetControlAccordion->addContentPane("Move Pane");

    this->moveCF = ui->widgetControlAccordion->getContentPane(indexMoveContentP)
                       ->getContentFrame();
    this->moveCF->setLayout(new QVBoxLayout());
    this->moveCF->layout()->addWidget(
        new QLabel("Move a content pane to another position"));
    auto *fromToCombos = new QHBoxLayout();
    dynamic_cast<QVBoxLayout *>(this->moveCF->layout())->addLayout(fromToCombos);
    auto *fromBox = new QComboBox();
    auto *toBox = new QComboBox();
    fromToCombos->addWidget(fromBox);
    fromToCombos->addWidget(toBox);

    QPushButton *movePaneButton = new QPushButton("Move Content Pane");
    movePaneButton->setDisabled(true);
    this->moveCF->layout()->addWidget(movePaneButton);

    QObject::connect(topAccordion, &QAccordion::numberOfContentPanesChanged,
                     [fromBox, toBox, movePaneButton](int number) {
                         fromBox->clear();
                         toBox->clear();
                         if (number <= 1) {
                             movePaneButton->setDisabled(true);
                             return;
                         }
                         for (int i = 0; i < number; i++) {
                             QString label = "Position " + QString::number(i);
                             fromBox->addItem(label, QVariant(i));
                             toBox->addItem(label, QVariant(i));
                         }
                         movePaneButton->setDisabled(false);
                     });

    QObject::connect(movePaneButton, &QPushButton::clicked, [topAccordion,
                                                             fromBox, toBox,
                                                             this]() {
        if (fromBox->currentData().toInt() != toBox->currentData().toInt()) {
            bool status = topAccordion->moveContentPane(
                fromBox->currentData().toUInt(), toBox->currentData().toUInt());
            if (status) {
                this->statusBar()->showMessage(
                    "Content Pane moved from index " +
                        fromBox->currentData().toString() + " to " +
                        toBox->currentData().toString(),
                    3000);
            } else {
                this->statusBar()->showMessage(
                    "Can not move Content Pane from index " +
                        fromBox->currentData().toString() + " to " +
                        toBox->currentData().toString(),
                    3000);
            }
        }
    });
}

void MainWindow::createIpsumLabel(QFrame *frame)
{
    // add some basic stuff to our QFrame
    frame->setLayout(new QVBoxLayout());
    QLabel *ipsumLabel = new QLabel();
    frame->layout()->addWidget(ipsumLabel);
    this->labelIpsumQueue.push(ipsumLabel);
    dynamic_cast<QVBoxLayout *>(frame->layout())->addStretch();
    // make the network request
    QNetworkRequest rquest;
    rquest.setUrl(QUrl(this->ipsumApi));
    rquest.setRawHeader("Accept", "text/html");
    rquest.setRawHeader("Connection", "Keep-Alive");
    this->networkManager->get(rquest);
}
