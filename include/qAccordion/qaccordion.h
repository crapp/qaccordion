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

    enum ACCORDION_ERROR {
        INDEX_OUT_OF_RANGE,

    };

    explicit QAccordion(QWidget *parent = 0);

    /**
     * @brief Returns the number of content panes
     * @return int
     */
    int numberOfContentPanes();

    /**
     * @brief Add a new content Pane
     * @param header Header of the content pane
     * @return uint Content pane index
     *
     * Use this method to add a new content pane with the Header header.
     * The method will return the index of the new content pane
     */
    uint addContentPane(QString header);
    /**
     * @brief Add a new content Pane
     * @param header Header of the content pane
     * @param contentPane The new content pane
     * @return uint Content pane index
     *
     * This is an overloaded method of QAccordion#addContentPane(QString), that
     * allows you to provide your own content pane.
     */
    uint addContentPane(QString header, QFrame *contentPane);
    /**
     * @brief Insert content pane
     * @param header Header of the content pane
     * @param index Index of the content pane
     *
     * You can use this method to insert a new content pane at given index with
     * \p header defining the Header.
     */
    void insertContentPane(QString header, uint index);
    void insertContentPane(QString header, uint index, QFrame *contentPane);

    void swapContentPane(uint index, QFrame *newContentPane);

    void removeContentPane(uint index);
    void removeContentPane(QString header);
    void removeContentPane(QFrame *contentPane);

    void setDisableContentPane(uint index, bool disable);
    void setDisableContentPane(QString header, bool disable);
    void setDisableContentPane(QFrame *contentPane, bool disable);

    void setContentPaneMaxHeight(uint maxHeight);
    void setContentPaneMaxHeight(uint index, uint maxHeight);

    QFrame *getContentPane(uint index);
    int getContentPaneIndex(QFrame *contentPane);

    void setHeaderName(uint index, QString header);
    QString getHeaderName(uint index);
    void setHeaderTooltip(uint index, QString tooltip);
    QString getHeaderTooltip(uint index);
    void setHeaderNormalStylesheet(uint index, QString stylesheet);
    QString getHeaderNormalStylesheet(uint index);
    void setHeaderHoverStylesheet(uint index, QString stylesheet);
    QString getHeaderHoverStylesheet(uint index);

    void setAnimationDuration(uint duration, uint index);
    int getAnimationDuration(uint index);

    QString getError();

signals:

    void numberOfContentPanesChanged(int number);
    void openedContentPane(uint index);
    void closedContentPane(uint index);
    void accordionError(QAccordion::ACCORDION_ERROR err);

public slots:

    void openContentPane(uint index);
    void closeContentPane(uint index);

private:
    std::vector<ClickableFrame *> contentPanesHeader;
    std::vector<QFrame *> contentPanesContainer;
    std::vector<QFrame *> contentPanes;

    std::map<ClickableFrame *, std::vector<std::shared_ptr<QPropertyAnimation>>>
        paneAnimationsMap;

    int highestdHeightContentPanes;
    int maximumHeightContentPanes;

    ClickableFrame *currentlyOpen;
    QSpacerItem *spacer;

    QString headerStylesheet;
    QString headerStylesheetHover;

    QString errorString;

    void determineHighestHeight();
    void emitOpenPaneIndex();

    ClickableFrame *initHeaderFrame(QString name, int index);
    QFrame *initContainerFrame(int index);
    void *initContentFrame(QFrame *container, QFrame *content, int index);
    void initPropertyAnimation(QFrame *container, ClickableFrame *clickFrame);

    uint internalAddContentPane(QString header, QFrame *contentPane = nullptr);
    void internalInsertContentPane(QString header, uint index,
                                   QFrame *contentPane = nullptr);
    void internalRemoveContentPane(int index = -1, QString name = "",
                                   QFrame *contentPane = nullptr);
    void internalEnableDisableContentPane(bool disable, int index = -1, QString header = "",
                                    QFrame *contentPane = nullptr);
    void addInsertWidget(int index, QFrame *frame);
    int findContentPaneIndex(QString name="", QFrame *contentPane = nullptr);

    /**
     * @brief paintEvent Reimplement paintEvent to use stylesheets in derived Widgets
     * @param event
     */
    void paintEvent(QPaintEvent *event);
};

#endif // QACCORDION_H
