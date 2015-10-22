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
    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);
    this->spacer = dynamic_cast<QSpacerItem *>(this->layout()->itemAt(0));

}

int QAccordion::numberOfContentPanes() { return this->contentPanes.size(); }

int QAccordion::addContentPane(QString header)
{
    return this->internalAddContentPane(std::move(header));
}

int QAccordion::addContentPane(QString header, QFrame *contentFrame)
{
    return this->internalAddContentPane(std::move(header), contentFrame);
}

int QAccordion::addContentPane(ContentPane *cpane)
{
    return this->internalAddContentPane("", nullptr, cpane);
}

bool QAccordion::insertContentPane(uint index, QString header)
{
    return this->internalInsertContentPane(index, header);
}

bool QAccordion::insertContentPane(uint index, QString header,
                                   QFrame *contentFrame)
{
    return this->internalInsertContentPane(index, header, contentFrame);
}

bool QAccordion::insertContentPane(uint index, ContentPane *cpane)
{
    return this->internalInsertContentPane(index, "", nullptr, cpane);
}

bool QAccordion::swapContentPane(uint index, ContentPane *cpane)
{
    if (this->checkIndexError(index, "Can not swap content pane at index " +
                                         QString::number(index) +
                                         ". Index out of range.")) {
        return false;
    }

    if (this->findContentPaneIndex("", nullptr, cpane) != -1) {
        this->errorString = "Can not swap content pane as new pane is already "
                            "managed by accordion widget";
        return false;
    }

    // remove the old content pane from the accordion layout
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->removeWidget(this->contentPanes.at(index));
    delete this->contentPanes.at(index);

    // add the new content pane to the appropriate vector
    this->contentPanes.at(index) = cpane;

    // add the new content pane to the layout
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(index, this->contentPanes.at(index));

    return true;
}

bool QAccordion::removeContentPane(uint index)
{
    return this->internalRemoveContentPane(index);
}

bool QAccordion::removeContentPane(QString header)
{
    return this->internalRemoveContentPane(-1, header);
}

bool QAccordion::removeContentPane(QFrame *contentframe)
{
    return this->internalRemoveContentPane(-1, "", contentframe);
}

bool QAccordion::removeContentPane(ContentPane *contentPane)
{
    return this->internalRemoveContentPane(-1, "", nullptr, contentPane);
}

bool QAccordion::moveContentPane(uint currentIndex, uint newIndex)
{
    if (this->checkIndexError(currentIndex, "Can not move from " +
                                                QString::number(currentIndex) +
                                                ". Index out of range.") ||
        this->checkIndexError(newIndex, "Can not move to " +
                                            QString::number(newIndex) +
                                            ". Index out of range.")) {
        return false;
    }

    QVBoxLayout *layout = dynamic_cast<QVBoxLayout *>(this->layout());
    ContentPane *movePane = this->contentPanes.at(currentIndex);

    int newIndexCalc = newIndex;

    if (newIndex > currentIndex) {
        newIndexCalc--;
    }

    layout->removeWidget(movePane);
    layout->insertWidget(newIndexCalc, movePane);

    this->contentPanes.erase(this->contentPanes.begin() + currentIndex);
    this->contentPanes.insert(this->contentPanes.begin() + newIndexCalc,
                              movePane);

    return true;
}

bool QAccordion::setDisabledContentPane(uint index, bool disable)
{
    return this->internalEnableDisableContentPane(disable, index);
}

bool QAccordion::setDisabledContentPane(QString header, bool disable)
{
    return this->internalEnableDisableContentPane(disable, -1,
                                                  std::move(header));
}

bool QAccordion::setDisabledContentPane(QFrame *contentPane, bool disable)
{
    return this->internalEnableDisableContentPane(disable, -1, "", contentPane);
}

ContentPane *QAccordion::getContentPane(uint index)
{
    try {
        return this->contentPanes.at(index);
    } catch (const std::out_of_range &ex) {
        qDebug() << Q_FUNC_INFO << "Can not return Content Pane: " << ex.what();
        return nullptr;
    }
}

int QAccordion::getContentPaneIndex(ContentPane *contentPane)
{
    int index = -1;
    auto result = std::find(this->contentPanes.begin(),
                            this->contentPanes.end(), contentPane);
    if (result != std::end(this->contentPanes)) {
        index = static_cast<int>(result - this->contentPanes.begin());
    } else {
        qDebug() << Q_FUNC_INFO << "Can not find Content Pane";
    }
    return index;
}

int QAccordion::getNumberOfContentPanes()
{
    return this->contentPanes.size();
}



QString QAccordion::getError() { return this->errorString; }

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
}

int QAccordion::internalAddContentPane(QString header, QFrame *cframe,
                                       ContentPane *cpane)
{
    if (this->findContentPaneIndex(header, cframe, cpane) != -1) {
        this->errorString = "Can not add content pane as it already exists";
        return -1;
    }

    if (cpane == nullptr) {
        if (cframe != nullptr) {
            cpane = new ContentPane(std::move(header), cframe);
        } else {
            cpane = new ContentPane(std::move(header));
        }
    }
    dynamic_cast<QVBoxLayout *>(this->layout())
        ->insertWidget(this->layout()->count() - 1, cpane);
    this->contentPanes.push_back(cpane);

    QObject::connect(cpane, &ContentPane::clicked, [this, cpane]() {
        if (cpane->getOpen()) {
            cpane->closeContentPane();
            return;
        }
        if (!cpane->getOpen()) {
            auto result = std::find_if(
                this->contentPanes.begin(), this->contentPanes.end(),
                [](ContentPane *cpane) { return cpane->getOpen(); });
            if (result != std::end(this->contentPanes)) {
                (*result)->closeContentPane();
            }
            cpane->openContentPane();
        }
    });

    // this->determineHighestHeight();
    emit numberOfContentPanesChanged(this->contentPanes.size());

    return this->contentPanes.size() - 1;
}

bool QAccordion::internalInsertContentPane(uint index, QString header,
                                           QFrame *contentFrame,
                                           ContentPane *cpane)
{
    if (this->checkIndexError(index, "Can not insert Content Pane at index " +
                                         QString::number(index) +
                                         ". Index out of range")) {
        return false;
    }

    if (this->findContentPaneIndex(header, contentFrame, cpane) != -1) {
        return false;
    }

    if (cpane == nullptr) {
        if (contentFrame != nullptr) {
            cpane = new ContentPane(std::move(header), contentFrame);
        } else {
            cpane = new ContentPane(std::move(header));
        }
    }

    dynamic_cast<QVBoxLayout *>(this->layout())->insertWidget(index, cpane);

    this->contentPanes.insert(this->contentPanes.begin() + index, cpane);

    emit numberOfContentPanesChanged(this->contentPanes.size());

    return true;
}

bool QAccordion::internalRemoveContentPane(int index, QString name,
                                           QFrame *contentFrame,
                                           ContentPane *cpane)
{
    if (this->checkIndexError(index, "Can not remove content pane at index " +
                                         QString::number(index) +
                                         ". Index out of range")) {
        return false;
    }

    if (index == -1) {
        index = this->findContentPaneIndex(std::move(name), contentFrame, cpane);
        if (index == -1) {
            this->errorString = "Can not remove content pane as it is not part "
                                "of the accordion widget";
            return false;
        }
    }

    dynamic_cast<QVBoxLayout *>(this->layout())
        ->removeWidget(this->contentPanes.at(index));

    delete this->contentPanes.at(index);
    this->contentPanes.at(index) = nullptr;

    this->contentPanes.erase(this->contentPanes.begin() + index);

    emit numberOfContentPanesChanged(this->contentPanes.size());

    return true;
}

bool QAccordion::internalEnableDisableContentPane(bool disable, int index,
                                                  QString header,
                                                  QFrame *contentPane)
{
    if (this->checkIndexError(index, "Can not disable content pane at index " +
                                         QString::number(index) +
                                         ". Index out of range")) {
        return false;
    }

    if (index == -1) {
        index = this->findContentPaneIndex(std::move(header), contentPane);
        if (index == -1) {
            this->errorString = "Can not enable/disable content pane as it is "
                                "not part of the accordion widget";
            return false;
        }
    }

    this->contentPanes.at(index)->setDisabled(disable);

    return true;
}

int QAccordion::findContentPaneIndex(QString name, QFrame *cframe,
                                     ContentPane *cpane)
{
    int index = -1;
    if (name != "") {
        auto result = std::find_if(
            this->contentPanes.begin(), this->contentPanes.end(),
            [&name](ContentPane *pane) { return pane->getHeader() == name; });
        if (result != std::end(this->contentPanes)) {
            // get the index by subtracting begin iterator from result
            // iterator
            // TODO: Is this cast really necessary?
            index = static_cast<int>(result - this->contentPanes.begin());
        }
    }
    if (cframe != nullptr) {
        auto result =
            std::find_if(this->contentPanes.begin(), this->contentPanes.end(),
                         [cframe](ContentPane *cpane) {
                             return cpane->getContentFrame() == cframe;
                         });
        if (result != std::end(this->contentPanes)) {
            index = static_cast<int>(result - this->contentPanes.begin());
        }
    }
    if (cpane != nullptr) {
        auto result = std::find(this->contentPanes.begin(),
                                this->contentPanes.end(), cpane);
        if (result != std::end(this->contentPanes)) {
            index = static_cast<int>(result - this->contentPanes.begin());
        }
    }
    return index;
}

bool QAccordion::checkIndexError(uint index, const QString &errMessage)
{
    if (index >= this->contentPanes.size()) {
        qDebug() << Q_FUNC_INFO << errMessage;
        this->errorString = errMessage;
        return true;
    }
    return false;
}

void QAccordion::paintEvent(__attribute__((unused)) QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
