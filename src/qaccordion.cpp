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
    this->maximumHeightContentPanes = 120;
    this->highestdHeightContentPanes = 0;
}

int QAccordion::numberOfContentPanes() { return this->contentPanes.size(); }

uint QAccordion::addContentPane(QString header)
{
    return this->internalAddContentPane(std::move(header));
}

uint QAccordion::addContentPane(QString header, QFrame *contentPane)
{
    return this->internalAddContentPane(std::move(header), contentPane);
}

void QAccordion::insertContentPane(QString header, uint index)
{
    this->internalInsertContentPane(header, index);
}

void QAccordion::insertContentPane(QString header, uint index,
                                   QFrame *contentPane)
{
    this->internalInsertContentPane(header, index, contentPane);
}

void QAccordion::swapContentPane(uint index, QFrame *newContentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Index out of range " << index;
        emit this->accordionError(
            QAccordion::ACCORDION_ERROR::INDEX_OUT_OF_RANGE);
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

void QAccordion::removeContentPane(uint index)
{
    this->internalRemoveContentPane(index);
}

void QAccordion::removeContentPane(QString header)
{
    this->internalRemoveContentPane(-1, header);
}

void QAccordion::removeContentPane(QFrame *contentPane)
{
    this->internalRemoveContentPane(-1, "", contentPane);
}

void QAccordion::setDisableContentPane(uint index, bool disable)
{
    this->internalEnableDisableContentPane(disable, index);
}

void QAccordion::setDisableContentPane(QString header, bool disable)
{
    this->internalEnableDisableContentPane(disable, -1, std::move(header));
}

void QAccordion::setDisableContentPane(QFrame *contentPane, bool disable)
{
    this->internalEnableDisableContentPane(disable, -1, "", contentPane);
}

void QAccordion::setContentPaneMaxHeight(uint maxHeight)
{
    if (this->currentlyOpen != nullptr) {
        this->currentlyOpen->setMaximumHeight(maxHeight);
    }

    for (const auto &headerFrame : this->contentPanesHeader) {
        this->paneAnimationsMap.at(headerFrame).at(0)->setEndValue(maxHeight);
        this->paneAnimationsMap.at(headerFrame).at(1)->setStartValue(maxHeight);
    }
}

void QAccordion::setContentPaneMaxHeight(uint index, uint maxHeight)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO
                 << "Can not set maximum hight for content pane at index "
                 << index << ". Index out of range";
        return;
    }

    if (this->currentlyOpen != nullptr &&
        this->contentPanesHeader.at(index) == this->currentlyOpen) {
        this->currentlyOpen->setMaximumHeight(maxHeight);
    }

    this->paneAnimationsMap.at(this->contentPanesHeader.at(index))
        .at(0)
        ->setEndValue(maxHeight);
    this->paneAnimationsMap.at(this->contentPanesHeader.at(index))
        .at(1)
        ->setStartValue(maxHeight);
}

QFrame *QAccordion::getContentPane(uint index)
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

void QAccordion::setHeaderName(uint index, QString header)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set header name at index " << index
                 << ". Index out of range";
        return;
    }

    this->contentPanesHeader.at(index)->setHeader(header);
}

QString QAccordion::getHeaderName(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get Header name for index " << index
                 << ". Index out of range";
        return "";
    }

    return this->contentPanesHeader.at(index)->getHeader();
}

void QAccordion::setHeaderTooltip(uint index, QString tooltip)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set Tooltip for index " << index
                 << ". Index out of range";
        return;
    }

    this->contentPanesHeader.at(index)->setToolTip(tooltip);
}

QString QAccordion::getHeaderTooltip(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get Tooltip for index " << index
                 << ". Index out of range";
        return "";
    }
    return this->contentPanesHeader.at(index)->toolTip();
}

void QAccordion::setHeaderNormalStylesheet(uint index, QString stylesheet)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set normal stylsheet for index "
                 << index << ". Index out of range";
        return;
    }

    this->contentPanesHeader.at(index)->setNormalStylesheet(stylesheet);
}

QString QAccordion::getHeaderNormalStylesheet(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get normal stylsheet for index "
                 << index << ". Index out of range";
        return "";
    }
    return this->contentPanesHeader.at(index)->getNormalStylesheet();
}

void QAccordion::setHeaderHoverStylesheet(uint index, QString stylesheet)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set hover stylsheet for index "
                 << index << ". Index out of range";
        return;
    }

    this->contentPanesHeader.at(index)->setHoverStylesheet(stylesheet);
}

QString QAccordion::getHeaderHoverStylesheet(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not get hover stylsheet for index "
                 << index << ". Index out of range";
        return "";
    }
    return this->contentPanesHeader.at(index)->getHoverStylesheet();
}

void QAccordion::setAnimationDuration(uint duration, uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set duration for index " << index
                 << ". Index out of range";
        return;
    }

    for (const auto &animation :
         this->paneAnimationsMap.at(this->contentPanesHeader.at(index))) {
        animation->setDuration(duration);
    }
}

int QAccordion::getAnimationDuration(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not set duration for index " << index
                 << ". Index out of range";
        return -1;
    }

    return this->paneAnimationsMap.at(this->contentPanesHeader.at(index))
        .at(0)
            ->duration();
}

QString QAccordion::getError()
{
    return this->errorString;
}

void QAccordion::openContentPane(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not open Content Pane at index " << index
                 << ". Index out of range";
        return;
    }
}

void QAccordion::closeContentPane(uint index)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not close Content Pane at index "
                 << index << ". Index out of range";
        return;
    }

    if (this->currentlyOpen == this->contentPanesHeader.at(index)) {
        std::shared_ptr<QPropertyAnimation> animation =
            this->paneAnimationsMap.at(this->contentPanesHeader.at(index))[1];
        animation->start();
        this->contentPanesHeader.at(index)
            ->setCaretPixmap(clickcon::CARRET_ICON_CLOSED);
        this->currentlyOpen = nullptr;
    }
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

void QAccordion::emitOpenPaneIndex()
{
    if (this->currentlyOpen != nullptr) {
        auto result =
            std::find(this->contentPanesHeader.begin(),
                      this->contentPanesHeader.end(), this->currentlyOpen);
        if (result != std::end(this->contentPanesHeader))
            emit openContentPane(
                static_cast<int>(result - this->contentPanesHeader.begin()));
    }
}

ClickableFrame *QAccordion::initHeaderFrame(QString name, int index)
{
    ClickableFrame *cframe = new ClickableFrame(std::move(name));
    if (index == -1) {
        this->contentPanesHeader.push_back(cframe);
    } else {
        this->contentPanesHeader.insert(this->contentPanesHeader.begin() + index,
                                        cframe);
    }
    return cframe;
}

QFrame *QAccordion::initContainerFrame(int index)
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

void *QAccordion::initContentFrame(QFrame *container, QFrame *content, int index)
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
    showAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::Linear));
    hideAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::Linear));

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

uint QAccordion::internalAddContentPane(QString header, QFrame *contentPane)
{
    ClickableFrame *clickFrame = this->initHeaderFrame(std::move(header), -1);

    QFrame *container = this->initContainerFrame(-1);
    this->initContentFrame(container, contentPane, -1);

    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(this->layout()->count() - 1, clickFrame);
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(this->layout()->count() - 1, container);

    this->initPropertyAnimation(container, clickFrame);

    // this->determineHighestHeight();
    emit numberOfContentPanesChanged(this->contentPanes.size());

    return this->contentPanes.size() - 1;
}

void QAccordion::internalInsertContentPane(QString header, uint index,
                                           QFrame *contentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not insert Content Pane at index "
                 << index << ". Index out of range";
        return;
    }
    ClickableFrame *clickFrame = this->initHeaderFrame(std::move(header), index);

    QFrame *container = this->initContainerFrame(index);
    this->initContentFrame(container, contentPane, index);

    int widgetIndex = 2 * index + 1;

    this->addInsertWidget(widgetIndex - 1, clickFrame);
    this->addInsertWidget(widgetIndex, container);

    this->initPropertyAnimation(container, clickFrame);

    emit numberOfContentPanesChanged(this->contentPanes.size());

    // this->determineHighestHeight();
}
// TODO: This QString could be made a const reference. But the compiler recognize
// this.
void QAccordion::internalRemoveContentPane(int index, QString name,
                                           QFrame *contentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not remove content pane at index "
                 << index << ". Index out of range";
        return;
    }
    if (index == -1) {
        index = this->findContentPaneIndex(std::move(name), contentPane);
        if (index == -1)
            return;
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

    emit numberOfContentPanesChanged(this->contentPanes.size());
}

void QAccordion::internalEnableDisableContentPane(bool disable, int index,
                                                  QString header,
                                                  QFrame *contentPane)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << "Can not disable content pane at index "
                 << index << ". Index out of range";
        return;
    }

    if (index == -1) {
        index = this->findContentPaneIndex(std::move(header), contentPane);
        if (index == -1)
            return;
    }

    this->contentPanesHeader.at(index)->setDisabled(disable);
}

void QAccordion::addInsertWidget(int index, QFrame *frame)
{
    dynamic_cast<QVBoxLayout *>(this->layout())->insertWidget(index, frame);
}

int QAccordion::findContentPaneIndex(QString name, QFrame *contentPane)
{
    int index = -1;
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
            index = static_cast<int>(result - this->contentPanesHeader.begin());
        } else {
            qDebug() << Q_FUNC_INFO
                     << "Can not remove / disable Content Pane with Header "
                     << name;
        }
    }
    if (contentPane != nullptr) {
        auto result = std::find(this->contentPanes.begin(),
                                this->contentPanes.end(), contentPane);
        if (result != std::end(this->contentPanes)) {
            index = static_cast<int>(result - this->contentPanes.begin());
        } else {
            qDebug() << Q_FUNC_INFO
                     << "Can not remove / disable Content Pane as this "
                        "frame is not managed by qAccordion";
        }
    }
    return index;
}

void QAccordion::paintEvent(QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
