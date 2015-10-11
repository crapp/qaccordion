#ifndef CONTENTPANE_H
#define CONTENTPANE_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QString>

#include <memory>

#include "clickableframe.h"
#include "contentpane.h"

/**
 * @brief The ContentPane class
 */
class ContentPane : public QWidget
{
    Q_OBJECT
public:
    explicit ContentPane(QString header, QWidget *parent = 0);
    explicit ContentPane(QString header, QFrame *content = nullptr,
                         QWidget *parent = 0);

    QFrame *getContentFrame();
    void setContentFrame(QFrame *content);

    int getMaximumHeight();
    void setMaximumHeight(int maxHeight);
    void setHeader(QString header);
    QString getHeader();
    void setHeaderTooltip(QString tooltip);
    QString getHeaderTooltip();
    void setHeaderStylesheet(QString stylesheet);
    QString getHeaderStylesheet();
    void setHeaderHoverStylesheet(QString stylesheet);
    QString getHeaderHoverStylesheet();
    void setHeaderFrameStyle(int style);
    int getHeaderFrameStyle();
    void setContentPaneFrameStyle(int style);
    int getContentPaneFrameStyle();

signals:
    void contentPaneOpen();
    void contentPaneClosed();

public slots:

private:
    ClickableFrame *header;
    QFrame *container;
    QFrame *content;

    std::shared_ptr<AccordionModel> model;

    int headerFrameStyle;
    int contentPaneFrameStyle;

    void initHeaderFrame(QString header);
    void initContainerContentFrame();

    /**
     * @brief paintEvent Reimplement paintEvent to use stylesheets in derived Widgets
     * @param event
     */
    void paintEvent(__attribute__((unused)) QPaintEvent *event);
};

#endif // CONTENTPANE_H
