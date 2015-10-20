#include "qAccordion/clickableframe.h"

ClickableFrame::ClickableFrame(QString header, QWidget *parent,
                               Qt::WindowFlags f)
    : header(header), QFrame(parent, f)
{
    this->setAttribute(Qt::WA_Hover, true);
    this->clickable = true;
    this->setCursor(Qt::PointingHandCursor);
    QColor background = this->palette().color(QPalette::ColorRole::Background);
    QColor lighter = background.lighter(110);
    this->normalStylesheet = "";
    this->hoverStylesheet = "background-color: " + lighter.name() + ";";
    this->initFrame();
    this->setStyleSheet("ClickableFrame {border: 1px solid green}");
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

void ClickableFrame::setHeader(QString header)
{
    this->header = header;
    this->nameLabel->setText(this->header);
}

QString ClickableFrame::getHeader() { return this->header; }

void ClickableFrame::setNormalStylesheet(QString stylesheet)
{
    this->normalStylesheet = stylesheet;
    this->setStyleSheet(this->normalStylesheet);
}

QString ClickableFrame::getNormalStylesheet() { return this->normalStylesheet; }

void ClickableFrame::setHoverStylesheet(QString stylesheet)
{
    this->hoverStylesheet = stylesheet;
}

QString ClickableFrame::getHoverStylesheet() { return this->hoverStylesheet; }

void ClickableFrame::setCaretPixmap(QString pixmapPath)
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
    nameLabel->setText(this->header);
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

void ClickableFrame::enterEvent(__attribute__ ((unused)) QEvent *event)
{
    if (this->clickable) {
        this->setStyleSheet(this->hoverStylesheet);
    }
}

void ClickableFrame::leaveEvent(__attribute__ ((unused)) QEvent *event)
{
    if (this->clickable) {
        this->setStyleSheet(this->normalStylesheet);
    }
}
