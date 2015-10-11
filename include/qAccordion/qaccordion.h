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
#include "accordionmodel.h"

/**
 * @brief The QAccordion class
 *
 *
 *
 */
class QAccordion : public QWidget
{
    Q_OBJECT
public:

    explicit QAccordion(QWidget *parent = 0);

    /**
     * @brief Returns the number of content panes
     * @return int
     */
    int numberOfContentPanes();

    /**
     * @brief Add a new content Pane
     * @param header Header of the content pane
     * @return Content pane index
     *
     * @details
     * Use this method to add a new content pane with the Header header.
     * The method will return the index of the new content pane, or <b>-1</b> if
     * the pane was not added because \p header already exists.
     */
    int addContentPane(QString header);
    /**
     * @brief Add a new content Pane
     * @param header Header of the content pane
     * @param contentPane The new content pane
     * @return Content pane index
     *
     * @details
     * This is an overloaded method of addContentPane(QString), that
     * allows you to provide your own content pane.
     */
    int addContentPane(QString header, QFrame *contentPane);
    /**
     * @brief Insert content pane
     * @param header Header of the content pane
     * @param index Index of the content pane
     * @return bool True if insert was successfull
     *
     * @details
     * You can use this method to insert a new content pane at given index with
     * \p header defining the Header. An empty content pane will be created that
     * you can get with getContentPane().
     *
     * Returns true if the insert was successfull.
     */
    bool insertContentPane(QString header, uint index);
    /**
     * @brief Insert content pane
     * @param header Header of the content pane
     * @param index Index of the content pane
     * @param contentPane New content pane
     * @return bool True if insert was successfull
     *
     * @details
     * This is an overloaded method of insertContentPane(QString, uint).
     * Use this method when you already created a content pane that you want to
     * insert.
     */
    bool insertContentPane(QString header, uint index, QFrame *contentPane);

    /**
     * @brief Swap the content pane
     * @param index Index of the content pane to swap
     * @param newContentPane New content pane
     * @return bool True if swap was successfull
     *
     * @details
     * With this method you can replace an existing content pane at \p index with
     * a new one \p contentPane.
     *
     * Returns true if the swap was successfull.
     *
     * The old content pane will be __deleted__.
     */
    bool swapContentPane(uint index, QFrame *newContentPane);

    /**
     * @brief Remove a content pane
     * @param index Index of the content pane
     * @return bool
     *
     * @details
     * Remove a content pane at \p index.
     *
     * Returns true if the pane was deleted and false otherwise.
     */
    bool removeContentPane(uint index);
    /**
     * @brief Remove a content pane
     * @param header Header of the content pane
     * @return bool
     *
     * @details
     * This is an overloaded method of removeContentPane(uint).
     */
    bool removeContentPane(QString header);
    /**
     * @brief Remove a content pane
     * @param contentPane The content pane to remove
     * @return bool
     *
     * @details
     * This is an overloaded method of removeContentPane(uint).
     */
    bool removeContentPane(QFrame *contentPane);

    /**
     * @brief Move content pane
     * @param currentIndex The current index of the content pane.
     * @param newIndex The new index of the current pane
     * @return bool
     *
     * @details
     * Moves a content from \p currentIndex to \p newIndex. Returns true if the
     * content pane was moved, false otherwise.
     */
    bool moveContentPane(uint currentIndex, uint newIndex);

    /**
     * @brief Enable or disable content pane
     * @param index Index of the content pane
     * @param disable
     * @return bool
     *
     * @details
     * Enable or disable a content pane at \p index. The status can be set with
     * \p  disable. This disables the header as well and it can longer be clicked.
     *
     * Method returns true if the content pane was disabled (enabled), false
     * otherwise.
     *
     * To disable all content panes use [QAccordion::setDisabled(bool)](http://doc.qt.io/qt-5.5/qwidget.html#setDisabled).
     */
    bool setDisabledContentPane(uint index, bool disable);
    /**
     * @brief Enable or disable content pane
     * @param header Content pane header
     * @param disable
     * @return bool
     *
     * @details
     * This is an overloaded function of moveContentPane(uint, uint). Instead of
     * an index you can provide the Header.
     */
    bool setDisabledContentPane(QString header, bool disable);
    /**
     * @brief Enable or disable content pane
     * @param contentPane Content pane
     * @param disable
     * @return bool
     *
     * @details
     * This is an overloaded function of moveContentPane(uint, uint). Instead of
     * an index you can provide a content pane.
     */
    bool setDisabledContentPane(QFrame *contentPane, bool disable);

    /**
     * @brief Set the maximum height of the content pane containers.
     * @param maxHeight
     *
     * @details
     * Every content pane is in a container that defines the visible maximum
     * height. With this function you can change this setting for all content
     * panes.
     *
     * @note
     * This setting does not set the maximum height of the content pane. It is
     * the users responsibilty to make sure everything in the content pane is
     * visible. This can be influenced by either changing the maximum height of
     * container or for example by adding a [QScrollArea](http://doc.qt.io/qt-5.5/qscrollarea.html).
     */
    void setContentPaneMaxHeight(uint maxHeight);
    /**
     * @brief Set the maximum height of a content pane container.
     * @param index Index of the content pane
     * @param maxHeight
     *
     * @details
     * This an overloaded function of setContentPaneMaxHeight(uint). Use this
     * function to set the maximum height of one container.
     */
    void setContentPaneMaxHeight(uint index, uint maxHeight);

    /**
     * @brief Get content pane
     * @param index Index of the content pane
     * @return QFrame* or nullptr
     *
     * @details
     * Get a content pane (QFrame*) with \p index. This method will return a
     * __nullptr__ if the content pane does not exist.
     */
    QFrame *getContentPane(uint index);
    /**
     * @brief Get the index of a content pane
     * @param contentPane QFrame*
     * @return Index of the content pane
     *
     * @details
     * Get the index of a provided content pane (QFrame*) this method will return
     * <b>-1</b> if the content pane does not exist.
     */
    int getContentPaneIndex(QFrame *contentPane);
    /**
     * @brief Get the number of content panes
     * @return Number of content panes
     */
    int getNumberOfContentPanes();

    /**
     * @brief Set header of a content pane
     * @param index Index of the content pane
     * @param header
     * @return boolean
     *
     * @details
     * Set the header of a content pane at \p index.
     * Returns false if \p index is out of range.
     */
    bool setHeader(uint index, QString header);
    /**
     * @brief Return the header of a content pane
     * @param index Index of the content pane
     * @return Header
     *
     * @details
     * Get the header of a content pane at \p index. Returns an empty String if
     * \p index is out of range
     */
    QString getHeader(uint index);
    /**
     * @brief Set header tooltip
     * @param index Index of the content pane
     * @param tooltip String to show as tooltip
     * @return boolean
     *
     * @details
     * Set a string as header tooltip that will be shown when the mouse hovers
     * over the header area.
     * Returns false if \p index is out of range.
     */
    bool setHeaderTooltip(uint index, QString tooltip);
    /**
     * @brief Get the tooltip for a content pane
     * @param index Index of the content pane
     * @return Tooltip as QString
     *
     * @details
     * Get the header tooltip as QString for content pane at \p index.
     * Returns an empty string if the \p index is out of range.
     */
    QString getHeaderTooltip(uint index);
    /**
     * @brief Set a stylesheet for the header frame
     * @param index Index of the content pane
     * @param stylesheet CSS Style Sheet as string
     *
     * @details
     * You can use [Cascading Style Sheets](http://doc.qt.io/qt-5/stylesheet.html) as supported by Qt to
     * style the header. This is the standard style sheet. You may also set a
     * style for mouse over with setHeaderHoverStylesheet().
     * Returns false if \p index is out of range.
     */
    bool setHeaderStylesheet(uint index, QString stylesheet);
    /**
     * @brief Get the current header style sheet
     * @param index Index of the content pane
     * @return CSS string
     *
     * @details
     * Get the css of the content pane header at \p index. Returns an empty string
     * if \p index is out of range.
     */
    QString getHeaderStylesheet(uint index);
    /**
     * @brief Set a stylesheet for the header frame when the mouse hovers over it
     * @param index Index of the content pane
     * @param stylesheet CSS Style Sheet as string
     * @return boolean
     *
     * @details
     * Set a \p stylesheet for the header for a special effect when the mouse
     * hovers over it.
     * Returns false if \p index is out of range.
     * @sa
     * setHeaderStylesheet() for additional details.
     */
    bool setHeaderHoverStylesheet(uint index, QString stylesheet);
    /**
     * @brief Get the mouse over header style sheet
     * @param index Index of the content pane
     * @return CSS Style Sheet as string
     *
     * @details
     * Returns the mouse over header style sheet or an empty string of \p index
     * is out of range.
     */
    QString getHeaderHoverStylesheet(uint index);

    /**
     * @brief Set the duration of the open and close animation
     * @param duration Duration in milliseconds
     * @param index Index of the content pane.
     * @return boolean
     *
     * @details
     * Set the duration of the PropertyAnimation in milliseconds. Will return
     * false if \p index is out of range.
     */
    bool setAnimationDuration(uint duration, uint index);
    /**
     * @brief Get the duration of the open, close animation.
     * @param Index of the content pane.
     * @return Duration in milliseconds
     *
     * @details
     */
    int getAnimationDuration(uint index);

    void setHeaderFrameStyle(int style);
    int getHeaderFrameStyle();
    void setContentPaneFrameStyle(int style);
    int getContentPaneFrameStyle();

    /**
     * @brief Get error string
     * @return Error string
     *
     * @details
     * Call this method after a function call returned false for a detailed error
     * description.
     */
    QString getError();

signals:
    /**
     * @brief Signals the new number of content panes
     * @param number Number of content panes
     *
     * @details
     * Signal will be emitted if the number of content panes changes
     */
    void numberOfContentPanesChanged(int number);
    /**
     * @brief Index of the content pane that was opened
     * @param index
     *
     * @details
     * This signal will be emitted when a content was opened and it is fully
     * visible.
     */
    void openedContentPane(uint index);
    /**
     * @brief Index of the content pane that was opened
     * @param index Index
     *
     * This signal will be emitted when a content was closed and it is no longer
     * visible.
     */
    void closedContentPane(uint index);

public slots:

    /**
     * @brief Open content pane
     * @param index Index of the content pane
     *
     * Open the content pane at index. If the content pane is already open
     * nothing will haben.
     */
    void openContentPane(uint index);
    /**
     * @brief Close content pane
     * @param index Index of the content pane
     *
     * Close the content pane at \p index. If the content pane is already closed
     * nothing will haben.
     */
    void closeContentPane(uint index);

private:
    std::vector<ClickableFrame *> contentPanesHeader;
    std::vector<QFrame *> contentPanesContainer;
    std::vector<QFrame *> contentPanes;

    std::map<ClickableFrame *, std::vector<std::shared_ptr<QPropertyAnimation>>>
        paneAnimationsMap;

    std::shared_ptr<AccordionModel> model;

    int highestdHeightContentPanes;
    int maximumHeightContentPanes;

    ClickableFrame *currentlyOpen;
    QSpacerItem *spacer;

    QString headerStylesheet;
    QString headerStylesheetHover;

    int headerFrameStyle;
    int contentPaneFrameStyle;

    QString errorString;

    void determineHighestHeight();
    void emitOpenPaneIndex();

    ClickableFrame *initHeaderFrame(QString name, int index);
    QFrame *initContainerFrame(int index);
    void initContentFrame(QFrame *container, QFrame *content, int index);
    void initPropertyAnimation(QFrame *container, ClickableFrame *clickFrame);

    int internalAddContentPane(QString header, QFrame *contentPane = nullptr);
    bool internalInsertContentPane(QString header, uint index,
                                   QFrame *contentPane = nullptr);
    bool internalRemoveContentPane(int index = -1, QString name = "",
                                   QFrame *contentPane = nullptr);
    bool internalEnableDisableContentPane(bool disable, int index = -1,
                                          QString header = "",
                                          QFrame *contentPane = nullptr);

    void addInsertWidget(int index, QFrame *frame);
    int findContentPaneIndex(QString name = "", QFrame *contentPane = nullptr);

    bool checkIndexError(uint index, const QString &errMessage);

    /**
     * @brief paintEvent Reimplement paintEvent to use stylesheets in derived Widgets
     * @param event
     */
    void paintEvent(__attribute__((unused)) QPaintEvent *event);
};

#endif // QACCORDION_H
