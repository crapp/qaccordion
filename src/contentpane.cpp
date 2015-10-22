#include "qAccordion/contentpane.h"

namespace clickcon = ClickableFrame_constants;

ContentPane::ContentPane(QString header, QWidget *parent) : QWidget(parent)
{
    this->content = nullptr;

    this->initDefaults(std::move(header));
}

ContentPane::ContentPane(QString header, QFrame *content, QWidget *parent)
    : content(content), QWidget(parent)
{
    this->initDefaults(std::move(header));
}

bool ContentPane::getOpen() { return this->open; }

QFrame *ContentPane::getContentFrame() { return this->content; }

void ContentPane::setContentFrame(QFrame *content)
{
    this->container->layout()->removeWidget(this->content);
    if (this->content != nullptr)
        delete (this->content);
    this->content = content;
    dynamic_cast<QVBoxLayout *>(this->container->layout())
        ->insertWidget(0, this->content);
}

int ContentPane::getMaximumHeight() { return this->container->maximumHeight(); }

void ContentPane::setMaximumHeight(int maxHeight)
{
    this->containerAnimationMaxHeight = maxHeight;

    if (this->open)
        this->container->setMaximumHeight(this->containerAnimationMaxHeight);
    this->openAnimation->setEndValue(this->containerAnimationMaxHeight);
    this->closeAnimation->setStartValue(this->containerAnimationMaxHeight);
}

void ContentPane::setHeader(QString header)
{
    this->header->setHeader(std::move(header));
}

QString ContentPane::getHeader() { return this->header->getHeader(); }

void ContentPane::setHeaderTooltip(QString tooltip)
{
    this->header->setToolTip(tooltip);
}

QString ContentPane::getHeaderTooltip() { return this->header->toolTip(); }

void ContentPane::setHeaderStylesheet(QString stylesheet)
{
    this->header->setNormalStylesheet(std::move(stylesheet));
}

QString ContentPane::getHeaderStylesheet()
{
    return this->header->getNormalStylesheet();
}

void ContentPane::setHeaderHoverStylesheet(QString stylesheet)
{
    this->header->setHoverStylesheet(std::move(stylesheet));
}

QString ContentPane::getHeaderHoverStylesheet()
{
    return this->header->getHoverStylesheet();
}

void ContentPane::setHeaderFrameStyle(int style)
{
    this->header->setFrameStyle(style);
}

int ContentPane::getHeaderFrameStyle() { return this->header->frameStyle(); }

void ContentPane::setContainerFrameStyle(int style)
{
    this->container->setFrameStyle(style);
}

int ContentPane::getContainerFrameStyle()
{
    return this->container->frameStyle();
}

void ContentPane::openContentPane()
{
    if (this->open)
        return;
    this->openAnimation->start();
    this->header->setCaretPixmap(clickcon::CARRET_ICON_OPENED);
    this->open = true;
}

void ContentPane::closeContentPane()
{
    if (!this->open)
        return;
    this->closeAnimation->start();
    this->header->setCaretPixmap(clickcon::CARRET_ICON_CLOSED);
    this->open = false;
}

void ContentPane::initDefaults(QString header)
{
    this->open = false;

    this->setStyleSheet("ContentPane {border: 1px solid red;}");

    this->headerFrameStyle = QFrame::Shape::StyledPanel | QFrame::Shadow::Raised;
    this->contentPaneFrameStyle =
        QFrame::Shape::StyledPanel | QFrame::Shadow::Plain;

    this->setLayout(new QVBoxLayout());

    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);

    this->initHeaderFrame(std::move(header));

    this->containerAnimationMaxHeight = 220;

    this->initContainerContentFrame();

    this->initAnimations();
}

void ContentPane::initHeaderFrame(QString header)
{
    this->header = new ClickableFrame(std::move(header));
    this->header->setFrameStyle(this->headerFrameStyle);
    this->layout()->addWidget(this->header);

    QObject::connect(this->header, &ClickableFrame::singleClick, this,
                     &ContentPane::clickableFrameClicked);
}

void ContentPane::initContainerContentFrame()
{
    this->container = new QFrame();
    this->container->setLayout(new QVBoxLayout());
    this->container->setFrameStyle(this->contentPaneFrameStyle);
    this->container->setMaximumHeight(0);
    this->container->setSizePolicy(QSizePolicy::Policy::Maximum,
                                   QSizePolicy::Policy::Preferred);
    this->layout()->addWidget(this->container);

    if (this->content == nullptr) {
        this->content = new QFrame();
    }

    this->container->layout()->addWidget(this->content);
    //dynamic_cast<QVBoxLayout*>(this->container->layout())->addStretch();
}

void ContentPane::initAnimations()
{
    this->openAnimation =
        std::unique_ptr<QPropertyAnimation>(new QPropertyAnimation());
    this->closeAnimation =
        std::unique_ptr<QPropertyAnimation>(new QPropertyAnimation());
    this->openAnimation->setTargetObject(this->container);
    this->openAnimation->setPropertyName("maximumHeight");
    this->closeAnimation->setTargetObject(this->container);
    this->closeAnimation->setPropertyName("maximumHeight");

    this->openAnimation->setDuration(300);
    this->closeAnimation->setDuration(300);
    this->openAnimation->setStartValue(0);
    this->closeAnimation->setStartValue(this->containerAnimationMaxHeight);
    this->openAnimation->setEndValue(this->containerAnimationMaxHeight);
    this->closeAnimation->setEndValue(0);
    this->openAnimation->setEasingCurve(
        QEasingCurve(QEasingCurve::Type::Linear));
    this->closeAnimation->setEasingCurve(
        QEasingCurve(QEasingCurve::Type::Linear));
}

void ContentPane::clickableFrameClicked(QPoint pos) { emit this->clicked(); }

void ContentPane::paintEvent(__attribute__((unused)) QPaintEvent *event)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
