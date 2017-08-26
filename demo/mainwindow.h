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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <memory>
#include <queue>

#include "qaccordion/qaccordion.hpp"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // lorem ipsum api url
    const char *const ipsumApi = "http://loripsum.net/api/1/short/code";
    // a random offline ipsum in case there is no network connection
    const char *const offlineIpsum =
        "<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed plane "
        "dicit quod intellegit. Aufert enim sensus actionemque tollit omnem. "
        "Itaque ab his ordiamur. Restatis igitur vos; </p>"
        "<pre>"
        "Quod enim vituperabile est per se ipsum, id eo ipso vitium"
        "nominatum puto, vel etiam a vitio dictum vituperari."
        "Qui autem esse poteris, nisi te amor ipse ceperit?"
        "</pre>";

    Ui::MainWindow *ui;
    // Keep pointers to our contetn frames
    QFrame *addCF;
    QFrame *insertCF;
    QFrame *removeCF;
    QFrame *moveCF;

    std::unique_ptr<QNetworkAccessManager> networkManager; /**< Network manager
                                                              for ipsum api
                                                              requests.*/
    std::queue<QLabel *> labelIpsumQueue;

    void networkRequestFinished(QNetworkReply *reply);

    void contentPaneAdd(QAccordion *topAccordion);
    void contentPaneInsert(QAccordion *topAccordion);
    void contentPaneRemove(QAccordion *topAccordion);
    void contentPaneMove(QAccordion *topAccordion);

    void createIpsumLabel(QFrame *frame);
};

#endif  // MAINWINDOW_H
