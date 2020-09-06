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

#include "qaccordion/clickableframe.hpp"

ClickableFrame::ClickableFrame(QString header, QWidget *parent,
                               Qt::WindowFlags f)
    : iconLabel(nullptr),
      nameLabel(nullptr),
      header(std::move(header)),
      QFrame(parent, f)
{
    this->setAttribute(Qt::WA_Hover, true);
    this->headerTrigger = TRIGGER::SINGLECLICK;
    this->setCursor(Qt::PointingHandCursor);
    QColor background = this->palette().color(QPalette::ColorRole::Background);
    QColor lighter = background.lighter(110);
    this->normalStylesheet = "";
    this->hoverStylesheet = "background-color: " + lighter.name() + ";";
    this->initFrame();
}

void ClickableFrame::setTrigger(ClickableFrame::TRIGGER tr)
{
    this->headerTrigger = tr;
    if (tr != TRIGGER::NONE) {
        this->setCursor(Qt::PointingHandCursor);
    } else {
        this->setCursor(Qt::ForbiddenCursor);
    }
}

ClickableFrame::TRIGGER ClickableFrame::getTrigger()
{
    return this->headerTrigger;
}

void ClickableFrame::setHeader(QString header)
{
    this->header = std::move(header);
    this->nameLabel->setText(this->header);
}

QString ClickableFrame::getHeader() { return this->header; }

void ClickableFrame::setIcon(const QPixmap &icon)
{
    this->iconLabel->setPixmap(icon);
}

//void ClickableFrame::setIconPosition(ClickableFrame::ICON_POSITION pos) {}

void ClickableFrame::setNormalStylesheet(QString stylesheet)
{
    this->normalStylesheet = std::move(stylesheet);
    this->setStyleSheet(this->normalStylesheet);
}

QString ClickableFrame::getNormalStylesheet() { return this->normalStylesheet; }

void ClickableFrame::setHoverStylesheet(QString stylesheet)
{
    this->hoverStylesheet = std::move(stylesheet);
}

QString ClickableFrame::getHoverStylesheet() { return this->hoverStylesheet; }

void ClickableFrame::initFrame()
{
    this->setSizePolicy(QSizePolicy::Policy::Preferred,
                        QSizePolicy::Policy::Fixed);
    this->setLayout(new QHBoxLayout());

    this->iconLabel = new QLabel();
    this->iconLabel->setPixmap(QPixmap(this->CARRET_ICON_CLOSED));
    this->layout()->addWidget(this->iconLabel);

    this->nameLabel = new QLabel();
    nameLabel->setText(this->header);
    this->layout()->addWidget(nameLabel);

    dynamic_cast<QHBoxLayout *>(this->layout())->addStretch();

    this->setStyleSheet(this->normalStylesheet);
}

void ClickableFrame::mousePressEvent(QMouseEvent *event)
{
    if (this->headerTrigger == TRIGGER::SINGLECLICK) {
        emit this->triggered(event->pos());
        event->accept();
    } else {
        event->ignore();
    }
}

void ClickableFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (this->headerTrigger == TRIGGER::DOUBLECLICK) {
        emit this->triggered(event->pos());
        event->accept();
    } else {
        event->ignore();
    }
}

// TODO: No Stylesheet change when TRIGGER::NONE?
void ClickableFrame::enterEvent(ATTR_UNUSED QEvent *event)
{
    if (this->headerTrigger != TRIGGER::NONE) {
        this->setStyleSheet(this->hoverStylesheet);
    }
}

void ClickableFrame::leaveEvent(ATTR_UNUSED QEvent *event)
{
    if (this->headerTrigger != TRIGGER::NONE) {
        this->setStyleSheet(this->normalStylesheet);
    }
}
