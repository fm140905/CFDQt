// drag-zoom functionality added to qcustomplot
// https://www.qcustomplot.com/index.php/support/forum/227

#ifndef CUSTOMPLOTZOOM_H
#define CUSTOMPLOTZOOM_H

#include "qcustomplot.h"
#include <QPoint>
#include <QRubberBand>
#include <QWidget>

class QRubberBand;
class QMouseEvent;
class QWidget;

class CustomPlotZoom : public QCustomPlot
{
    Q_OBJECT
public:
//    explicit CustomPlotZoom(QWidget *parent = nullptr);
    CustomPlotZoom(QWidget * parent = 0);
    virtual ~CustomPlotZoom();

    void setZoomMode(bool mode);
    void setAutoScale(bool mode);
    bool isAutoScaled();

private slots:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool mZoomMode;
    QRubberBand * mRubberBand;
    QPoint mOrigin;
    bool autoScaled;
    bool pressed=false;
signals:

};

#endif // CUSTOMPLOTZOOM_H
