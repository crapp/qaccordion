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

#include "qAccordion/qaccordion.h"

namespace clickcon = ClickableFrame_constants;

QAccordion::QAccordion(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    dynamic_cast<QVBoxLayout *>(this->layout())->addStretch();
    this->layout()->setSpacing(2);
    this->spacer = dynamic_cast<QSpacerItem *>(this->layout()->itemAt(0));
    this->currentlyOpen = nullptr;
    this->maximumHeightContentPanes = 200;
    this->highestdHeightContentPanes = 0;
}

uint QAccordion::addContentPane(const QString &header)
{
    return this->internalAddContentPane(header);
}

uint QAccordion::addContentPane(const QString &header, QFrame *contentPane)
{
    return this->internalAddContentPane(header, contentPane);
}

void QAccordion::insertContentPane(const QString &header, const uint &index)
{
    this->internalInsertContentPane(header, index);
}

void QAccordion::insertContentPane(const QString &header, const uint &index,
                                   QFrame *contentPane)
{
    this->internalInsertContentPane(header, index, contentPane);
}

void QAccordion::swapContentPane(const uint &index, QFrame *newContentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Index out of range " << index;
        return;
    }

    // remove the old content pane from the containers layout
    dynamic_cast<QVBoxLayout *>(this->contentPanesContainer.at(index)->layout())
        ->removeWidget(this->contentPanes.at(index));
    delete this->contentPanes.at(index);

    // add the new content pane to the appropriate vector
    this->contentPanes.at(index) = newContentPane;

    // add the new content pane the containers layout
    this->contentPanesContainer.at(index)->layout()->addWidget(
        this->contentPanes.at(index));
}

void QAccordion::removeContentPane(const uint &index)
{
    this->internalRemoveContentPane(index);
}

void QAccordion::removeContentPane(const QString &header)
{
    this->internalRemoveContentPane(-1, header);
}

void QAccordion::removeContentPane(QFrame *contentPane)
{
    this->internalRemoveContentPane(-1, "", contentPane);
}

QFrame *QAccordion::getContentPane(const uint &index)
{
    try {
        return this->contentPanes.at(index);
    } catch (const std::out_of_range &ex) {
        qDebug() << Q_FUNC_INFO << "Can not return Content Pane: " << ex.what();
        return nullptr;
    }
}

int QAccordion::getContentPaneIndex(QFrame *contentPane)
{
    int index = -1;
    auto result = std::find(this->contentPanes.begin(), this->contentPanes.end(),
                            contentPane);
    if (result != std::end(this->contentPanes)) {
        index = static_cast<int>(result - this->contentPanes.begin());
    } else {
        qDebug() << Q_FUNC_INFO << "Can not find Content Pane";
    }
    return index;
}

void QAccordion::setHeaderName(const uint &index, const QString &header)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set header name at index " << index;
        return;
    }

    this->contentPanesHeader.at(index)->setHeader(header);
}

QString QAccordion::getHeaderName(const uint &index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get Header name for index " << index;
        return "";
    }

    return this->contentPanesHeader.at(index)->getHeader();
}

void QAccordion::setHeaderTooltip(const uint &index, const QString &tooltip)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set Tooltip for index " << index;
        return;
    }

    this->contentPanesHeader.at(index)->setToolTip(tooltip);
}

QString QAccordion::getHeaderTooltip(const uint &index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get Tooltip for index " << index;
        return "";
    }
    return this->contentPanesHeader.at(index)->toolTip();
}

void QAccordion::setHeaderNormalStylesheet(const uint &index,
                                           const QString &stylesheet)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set normal stylsheet for index "
                 << index;
        return;
    }

    this->contentPanesHeader.at(index)->setNormalStylesheet(stylesheet);
}

QString QAccordion::getHeaderNormalStylesheet(const uint &index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get normal stylsheet for index "
                 << index;
        return "";
    }
    return this->contentPanesHeader.at(index)->getNormalStylesheet();
}

void QAccordion::setHeaderHoverStylesheet(const uint &index,
                                          const QString &stylesheet)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set hover stylsheet for index "
                 << index;
        return;
    }

    this->contentPanesHeader.at(index)->setHoverStylesheet(stylesheet);
}

QString QAccordion::getHeaderHoverStylesheet(const uint &index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get hover stylsheet for index "
                 << index;
        return "";
    }
    return this->contentPanesHeader.at(index)->getHoverStylesheet();
}

void QAccordion::determineHighestHeight()
{
    if (!this->contentPanes.empty()) {
        std::vector<QFrame *> localContentPanes = this->contentPanes;
        std::sort(localContentPanes.begin(), localContentPanes.end(),
                  [](QFrame *a, QFrame *b) {
                      return b->sizeHint().height() < a->sizeHint().height();
                  });
        qDebug() << Q_FUNC_INFO << "Currently highest height: "
                 << localContentPanes.at(0)->sizeHint().height();
        this->highestdHeightContentPanes =
            localContentPanes.at(0)->sizeHint().height();
    } else {
        this->highestdHeightContentPanes = 0;
    }
}

ClickableFrame *QAccordion::initHeaderFrame(const QString &name,
                                            const int &index)
{
    ClickableFrame *cframe = new ClickableFrame(name);
    if (index == -1) {
        this->contentPanesHeader.push_back(cframe);
    } else {
        this->contentPanesHeader.insert(this->contentPanesHeader.begin() + index,
                                        cframe);
    }
    return cframe;
}

QFrame *QAccordion::initContainerFrame(const int &index)
{
    QFrame *containerFrame = new QFrame();
    containerFrame->setLayout(new QVBoxLayout());
    containerFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    containerFrame->setMaximumHeight(0);
    containerFrame->setSizePolicy(QSizePolicy::Policy::Preferred,
                                  QSizePolicy::Policy::Expanding);
    if (index == -1) {
        this->contentPanesContainer.push_back(containerFrame);
    } else {
        this->contentPanesContainer.insert(
            this->contentPanesContainer.begin() + index, containerFrame);
    }
    return containerFrame;
}

void *QAccordion::initContentFrame(QFrame *container, QFrame *content,
                                   const int &index)
{
    if (content == nullptr)
        content = new QFrame();
    if (index == -1) {
        this->contentPanes.push_back(content);
    } else {
        this->contentPanes.insert(this->contentPanes.begin() + index, content);
    }
    container->layout()->addWidget(content);
}

void QAccordion::initPropertyAnimation(QFrame *container,
                                       ClickableFrame *clickFrame)
{
    std::shared_ptr<QPropertyAnimation> showAnimation =
        std::make_shared<QPropertyAnimation>(container, "maximumHeight");
    std::shared_ptr<QPropertyAnimation> hideAnimation =
        std::make_shared<QPropertyAnimation>(container, "maximumHeight");
    std::vector<std::shared_ptr<QPropertyAnimation>> animationVector{
        showAnimation, hideAnimation};

    showAnimation->setDuration(300);
    hideAnimation->setDuration(300);
    showAnimation->setStartValue(0);
    hideAnimation->setStartValue(this->maximumHeightContentPanes);
    showAnimation->setEndValue(this->maximumHeightContentPanes);
    hideAnimation->setEndValue(0);

    this->paneAnimationsMap.insert({clickFrame, animationVector});

    QObject::connect(
        clickFrame, &ClickableFrame::singleClick,
        [this, clickFrame](const QPoint &pos) {
            if (this->currentlyOpen == nullptr) {
                // open associated container
                std::shared_ptr<QPropertyAnimation> animation =
                    this->paneAnimationsMap.at(clickFrame)[0];
                animation->start();
                clickFrame->setCaretPixmap(clickcon::CARRET_ICON_OPENED);
                this->currentlyOpen = clickFrame;
                return;
            }
            if (this->currentlyOpen == clickFrame) {
                // close associated container
                std::shared_ptr<QPropertyAnimation> animation =
                    this->paneAnimationsMap.at(clickFrame)[1];
                animation->start();
                clickFrame->setCaretPixmap(clickcon::CARRET_ICON_CLOSED);
                this->currentlyOpen = nullptr;
            } else {
                // close container of currently open and open
                // clicked one
                std::shared_ptr<QPropertyAnimation> animationShow =
                    this->paneAnimationsMap.at(clickFrame)[0];
                std::shared_ptr<QPropertyAnimation> animationHide =
                    this->paneAnimationsMap.at(this->currentlyOpen)[1];
                animationShow->start();
                animationHide->start();
                clickFrame->setCaretPixmap(clickcon::CARRET_ICON_OPENED);
                this->currentlyOpen->setCaretPixmap(
                    clickcon::CARRET_ICON_CLOSED);
                this->currentlyOpen = clickFrame;
            }

        });
}

uint QAccordion::internalAddContentPane(const QString &header,
                                        QFrame *contentPane)
{
    ClickableFrame *clickFrame = this->initHeaderFrame(header, -1);

    QFrame *container = this->initContainerFrame(-1);
    this->initContentFrame(container, contentPane, -1);

    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(this->layout()->count() - 1, clickFrame);
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(this->layout()->count() - 1, container);

    this->initPropertyAnimation(container, clickFrame);

    // this->determineHighestHeight();

    return this->contentPanes.size() - 1;
}

void QAccordion::internalInsertContentPane(const QString &header,
                                           const uint &index,
                                           QFrame *contentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not insert Content Pane at index "
                 << index;
        return;
    }
    ClickableFrame *clickFrame = this->initHeaderFrame(header, index);

    QFrame *container = this->initContainerFrame(index);
    this->initContentFrame(container, contentPane, index);

    int widgetIndex = 2 * index + 1;

    this->addInsertWidget(widgetIndex - 1, clickFrame);
    this->addInsertWidget(widgetIndex, container);

    this->initPropertyAnimation(container, clickFrame);

    // this->determineHighestHeight();
}

void QAccordion::internalRemoveContentPane(int index, const QString &name,
                                           QFrame *contentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not remove content pane at index  "
                 << index;
        return;
    }
    if (index == -1) {
        if (name != "") {
            auto result = std::find_if(this->contentPanesHeader.begin(),
                                       this->contentPanesHeader.end(),
                                       [&name](ClickableFrame *header) {
                                           return header->getHeader() == name;
                                       });
            if (result != std::end(this->contentPanesHeader)) {
                // get the index by subtracting begin iterator from result
                // iterator
                // TODO: Is this cast really necessary?
                index =
                    static_cast<int>(result - this->contentPanesHeader.begin());
            } else {
                qDebug() << Q_FUNC_INFO
                         << "Can not remove Content Pane with Header " << name;
            }
        }
        if (contentPane != nullptr) {
            auto result = std::find(this->contentPanes.begin(),
                                    this->contentPanes.end(), contentPane);
            if (result != std::end(this->contentPanes)) {
                index = static_cast<int>(result - this->contentPanes.begin());
            } else {
                qDebug() << Q_FUNC_INFO << "Can not remove Content Pane as this "
                                           "frame is not managed by qAccordion";
            }
        }
    }

    dynamic_cast<QVBoxLayout *>(this->layout())
        ->removeWidget(this->contentPanesHeader.at(index));
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->removeWidget(this->contentPanesContainer.at(index));

    delete this->contentPanesHeader.at(index);
    delete this->contentPanesContainer.at(index);
    this->contentPanesHeader.at(index) = nullptr;
    this->contentPanesContainer.at(index) = nullptr;
    this->contentPanes.at(index) = nullptr;
    this->contentPanesHeader.erase(this->contentPanesHeader.begin() + index);
    this->contentPanesContainer.erase(this->contentPanesContainer.begin() +
                                      index);
    this->contentPanes.erase(this->contentPanes.begin() + index);
}

void QAccordion::addInsertWidget(const int &index, QFrame *frame)
{
    dynamic_cast<QVBoxLayout *>(this->layout())->insertWidget(index, frame);
}

void QAccordion::paintEvent(QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
