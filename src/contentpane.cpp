#include "qAccordion/contentpane.h"

ContentPane::ContentPane(QString header, QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QHBoxLayout());

    this->content = nullptr;

    this->initHeaderFrame(std::move(header));
    this->initContainerContentFrame();

    this->headerFrameStyle = QFrame::Shape::StyledPanel | QFrame::Shadow::Raised;
    this->contentPaneFrameStyle =
        QFrame::Shape::StyledPanel | QFrame::Shadow::Plain;
}

ContentPane::ContentPane(QString header, QFrame *content, QWidget *parent)
    : content(content), QWidget(parent)
{
    this->setLayout(new QHBoxLayout());
    this->initHeaderFrame(std::move(header));

    this->initContainerContentFrame();

    this->headerFrameStyle = QFrame::Shape::StyledPanel | QFrame::Shadow::Raised;
    this->contentPaneFrameStyle =
            QFrame::Shape::StyledPanel | QFrame::Shadow::Plain;
}

QFrame *ContentPane::getContentFrame()
{
    return this->content;
}

void ContentPane::setContentFrame(QFrame *content)
{
    this->content = content;
}

void ContentPane::initHeaderFrame(QString header)
{
    this->header = new ClickableFrame(std::move(header));
    this->layout()->addWidget(this->header);
}

void ContentPane::initContainerContentFrame()
{
    this->container = new QFrame();
    this->container->setLayout(new QVBoxLayout());
    this->container->setFrameStyle(this->contentPaneFrameStyle);
    this->container->setMaximumHeight(0);
    this->container->setSizePolicy(QSizePolicy::Policy::Preferred,
                                   QSizePolicy::Policy::Expanding);
    this->layout()->addWidget(this->container);

    if (this->content == nullptr) {
        this->content = new QFrame();
    }

    this->container->layout()->addWidget(this->content);
}

void ContentPane::paintEvent(__attribute__((unused)) QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
