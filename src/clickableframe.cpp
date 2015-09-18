#include "qAccordion/clickableframe.h"

ClickableFrame::ClickableFrame(const QString &name, QWidget *parent,
                               Qt::WindowFlags f)
    : name(name), QFrame(parent, f)
{
    this->setAttribute(Qt::WA_Hover, true);
    this->clickable = true;
    this->setCursor(Qt::PointingHandCursor);
    this->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    this->initFrame();
}

void ClickableFrame::setClickable(bool status)
{
    this->clickable = status;
    if (status) {
        this->setCursor(Qt::PointingHandCursor);
    } else {
        this->setCursor(Qt::ForbiddenCursor);
    }
}

bool ClickableFrame::getClickable() { return this->clickable; }

void ClickableFrame::setName(const QString &name)
{
    this->name = name;
    this->nameLabel->setText(this->name);

}

QString ClickableFrame::getName()
{
    return this->name;
}

void ClickableFrame::setCaretPixmap(const QString &pixmapPath)
{
    this->caretLabel->setPixmap(QPixmap(pixmapPath));
}

void ClickableFrame::initFrame()
{
    this->setSizePolicy(QSizePolicy::Policy::Preferred,
                        QSizePolicy::Policy::Fixed);
    this->setLayout(new QHBoxLayout());

    this->caretLabel = new QLabel();
    this->caretLabel->setPixmap(QPixmap(":/icons/caret-right.png"));
    this->layout()->addWidget(this->caretLabel);

    this->nameLabel = new QLabel();
    nameLabel->setText(this->name);
    this->layout()->addWidget(nameLabel);

    dynamic_cast<QHBoxLayout *>(this->layout())->addStretch();
}

void ClickableFrame::mousePressEvent(QMouseEvent *event)
{
    if (this->clickable) {
        emit this->singleClick(event->pos());
        event->accept();
    } else {
        event->ignore();
    }

}

void ClickableFrame::enterEvent(QEvent *event)
{
    if (this->clickable) {
        if (this->originalStylesheet == "")
            this->originalStylesheet = this->styleSheet();
        // this->setStyleSheet("background-color: rgb(82, 82, 82);");
    }
}

void ClickableFrame::leaveEvent(QEvent *event)
{
    if (this->clickable) {
        // this->setStyleSheet(this->originalStylesheet);
    }
}
