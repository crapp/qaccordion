#ifndef CLICKABLEFRAME_H
#define CLICKABLEFRAME_H

#include <QFrame>
#include <QMouseEvent>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsView>

namespace ClickableFrame_constants
{
const char *const CARRET_ICON_CLOSED = ":/icons/caret-right.png";
const char *const CARRET_ICON_OPENED = ":/icons/caret-bottom.png";
}

class ClickableFrame : public QFrame
{

    Q_OBJECT
public:
    explicit ClickableFrame(const QString &header, QWidget *parent = 0,
                            Qt::WindowFlags f = 0);

    void setClickable(bool status);
    bool getClickable();
    void setHeader(const QString &header);
    QString getHeader();
    void setNormalStylesheet(const QString &stylesheet);
    QString getNormalStylesheet();
    void setHoverStylesheet(const QString &stylesheet);
    QString getHoverStylesheet();

signals:
    void singleClick(const QPoint &pos);

public slots:

    void setCaretPixmap(const QString &pixmapPath);

private:
    QLabel *caretLabel;
    QLabel *nameLabel;

    QString hoverStylesheet;
    QString normalStylesheet;

    QString header;
    QString tooltip;

    bool clickable;

    void initFrame();

    // interface
    void mousePressEvent(QMouseEvent *event);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // CLICKABLEFRAME_H
