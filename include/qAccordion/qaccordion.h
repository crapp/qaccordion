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

#ifndef QACCORDION_H
#define QACCORDION_H

#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QSpacerItem>
#include <QString>
#include <QPropertyAnimation>
#include <QDebug>

#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "clickableframe.h"

class QAccordion : public QWidget
{
    Q_OBJECT
public:
    explicit QAccordion(QWidget *parent = 0);

    uint addContentPane(const QString &header);
    uint addContentPane(const QString &header, QFrame *contentPane);
    void insertContentPane(const QString &header, const uint &index);
    void insertContentPane(const QString &header, const uint &index,
                           QFrame *contentPane);

    void swapContentPane(const uint &index, QFrame *newContentPane);

    void removeContentPane(const uint &index);
    void removeContentPane(const QString &header);
    void removeContentPane(QFrame *contentPane);

    QFrame *getContentPane(const uint &index);
    int getContentPaneIndex(QFrame *contentPane);

    void setHeaderName(const uint &index, const QString &header);
    QString getHeaderName(const uint &index);
    void setHeaderTooltip(const uint &index, const QString &tooltip);
    QString getHeaderTooltip(const uint &index);
    void setHeaderNormalStylesheet(const uint &index, const QString &stylesheet);
    QString getHeaderNormalStylesheet(const uint &index);
    void setHeaderHoverStylesheet(const uint &index, const QString &stylesheet);
    QString getHeaderHoverStylesheet(const uint &index);

signals:

public slots:

private:
    std::vector<ClickableFrame *> contentPanesHeader;
    std::vector<QFrame *> contentPanesContainer;
    std::vector<QFrame *> contentPanes;

    std::map<QFrame *, std::vector<std::shared_ptr<QPropertyAnimation>>>
        paneAnimationsMap;

    int highestdHeightContentPanes;
    int maximumHeightContentPanes;

    ClickableFrame *currentlyOpen;
    QSpacerItem *spacer;

    QString headerStylesheet;
    QString headerStylesheetHover;

    void determineHighestHeight();

    ClickableFrame *initHeaderFrame(const QString &name, const int &index);
    QFrame *initContainerFrame(const int &index);
    void *initContentFrame(QFrame *container, QFrame *content, const int &index);
    void initPropertyAnimation(QFrame *container, ClickableFrame *clickFrame);

    uint internalAddContentPane(const QString &header,
                                QFrame *contentPane = nullptr);
    void internalInsertContentPane(const QString &header, const uint &index,
                                   QFrame *contentPane = nullptr);
    void internalRemoveContentPane(int index = -1, const QString &name = "",
                                   QFrame *contentPane = nullptr);
    void addInsertWidget(const int &index, QFrame *frame);

    /**
     * @brief paintEvent Reimplement paintEvent to use stylesheets in derived Widgets
     * @param event
     */
    void paintEvent(QPaintEvent *event);
};

#endif // QACCORDION_H
