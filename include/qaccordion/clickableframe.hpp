// This file is part of qAccordion. An Accordion widget for Qt
// Copyright © 2015, 2017, 2020 Christian Rapp <0x2a at posteo dot org>
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

#ifndef CLICKABLEFRAME_HPP
#define CLICKABLEFRAME_HPP

#include <QFrame>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <qnamespace.h>

#include "qaccordion_config.hpp"

/**
 * @brief The ClickableFrame class
 *
 * This class represents a clickable QFrame. It is used by a ContentPane. The
 * class is used internally.
 */
class ClickableFrame : public QFrame
{
    Q_OBJECT
public:
    enum class ICON_POSITION { LEFT, RIGHT };
    enum class TRIGGER { NONE, SINGLECLICK, DOUBLECLICK, MOUSEOVER };

    const char *const CARRET_ICON_CLOSED =
        ":/qAccordionIcons/caret-right.png"; /**< Qt qrc "path" for the closed icon */
    const char *const CARRET_ICON_OPENED =
        ":/qAccordionIcons/caret-bottom.png"; /**< Qt qrc "path" for the opened icon */

    /**
     * @brief ClickableFrame constructor
     * @param header Header of the frame
     * @param parent Parent widget or 0
     * @param f Qt::WindowFlags
     */
    explicit ClickableFrame(QString header, QWidget *parent = nullptr,
                            Qt::WindowFlags f = Qt::Widget);

    // TODO: Expose this function to the ContentPane api
    /**
     * @brief Change header trigger
     * @param tr
     *
     * @details
     * Set the trigger for the header. You may choose between single or double
     * mouseclick and mouse enter event. Or ClickableFrame::TRIGGER::NONE if you
     * want no interaction at all (you may still trigger the header
     * programmatically though).
     */
    void setTrigger(TRIGGER tr);
    /**
     * @brief Get the the trigger of the header
     * @return ClickableFrame::TRIGGER
     */
    TRIGGER getTrigger();

    /**
     * @brief Set the header string
     * @param header
     */
    void setHeader(QString header);
    /**
     * @brief Get the header string
     * @return QString
     */
    QString getHeader();
    /**
     * @brief Set the default stylesheet
     * @param stylesheet
     */
    void setNormalStylesheet(QString stylesheet);
    /**
     * @brief Get the default stylesheet
     * @return QString
     */
    QString getNormalStylesheet();
    /**
     * @brief Set mouseover stylesheet
     * @param stylesheet
     */
    void setHoverStylesheet(QString stylesheet);
    /**
     * @brief Get mouseover stylesheet
     * @return
     */
    QString getHoverStylesheet();

signals:
    /**
     * @brief Signal that is emitted when the header is triggered
     * @param pos Currently unused
     */
    void triggered(QPoint pos);

public slots:

    /**
     * @brief Set the header icon
     * @param icon
     *
     * @details
     * ContentPane will set an icon depending on its state (active or not).
     */
    void setIcon(const QPixmap &icon);

    /**
     * @brief Set icon position
     * @param pos
     */
    //void setIconPosition(ClickableFrame::ICON_POSITION pos);

private:
    QLabel *iconLabel;
    QLabel *nameLabel;

    QString hoverStylesheet;
    QString normalStylesheet;

    QString header;
    QString tooltip;

    TRIGGER headerTrigger;

    void initFrame();

protected:
    /**
     * @brief Reimplemented function to QMouseEvents
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief Enter event for mouse over effects.
     * @param event
     */
    void enterEvent(QEvent *event) override;
    /**
     * @brief Leave effect for mouse over effects.
     * @param event
     */
    void leaveEvent(QEvent *event) override;
};

#endif  // CLICKABLEFRAME_HPP
