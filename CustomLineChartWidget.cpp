#include "CustomLineChartWidget.h"
#include <QHBoxLayout>
#include <QDebug>

using namespace zhst;

QT_CHARTS_USE_NAMESPACE

CustomLineChartWidget::CustomLineChartWidget(QWidget* parent)
    :QWidget(parent)
{
    createWidget();
}
CustomLineChartWidget::~CustomLineChartWidget()
{

}

void CustomLineChartWidget::addSeriesPoint(double x, double y)
{
    if (((QValueAxis*)_chart->axisY())->max() < y || isFirstUpdate)
    {
        _chart->axisY()->setRange(0, y);
        isFirstUpdate = false;
    }
    //_upperSeries->append(QPointF(x, y));
    //_lowerSeries->append(QPointF(x, 0));
    _upperPoints.append(QPointF(x, y));
    _upperSeries->replace(_upperPoints);

    _lowerSeries->append(QPointF(x, 0));
}

void zhst::CustomLineChartWidget::addPoints(QList<QPointF> pList)
{
    _lowerSeries->append(QPointF(pList[0].x(), 0));
    _lowerSeries->append(QPointF(pList[pList.size() - 1].x(), 0));
    _upperPoints.append(pList);
    _upperSeries->replace(_upperPoints);
    double m = 0;
    for (auto point : _upperPoints)
    {
        if (point.y() > m) m = point.y();
    }
    if (((QValueAxis*)_chart->axisY())->max() < m || isFirstUpdate)
    {
        _chart->axisY()->setRange(0, m);
        isFirstUpdate = false;
    }
}

void CustomLineChartWidget::setAreaColor(QColor color)
{
    _areaSeries->setColor(color);
    _areaSeries->setOpacity(color.alphaF());
}

void CustomLineChartWidget::setXRange(double min, double max)
{
    _chart->axisX()->setRange(min, max);
}

void CustomLineChartWidget::setBackgroundColor(QColor color)
{
    QString styleStr = QString("background-color:rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    this->setStyleSheet(styleStr);
}

void zhst::CustomLineChartWidget::clearPoints()
{
    _upperPoints.clear();
    _upperSeries->clear();
    _lowerSeries->clear();
    isFirstUpdate = true;
}

void CustomLineChartWidget::createWidget()
{
    this->setStyleSheet("background-color:transparent");
    _upperSeries = new QSplineSeries();
    _upperSeries->setUseOpenGL(true);
    _lowerSeries = new QSplineSeries();
    _lowerSeries->setUseOpenGL(true);

    _areaSeries = new QAreaSeries(_upperSeries, _lowerSeries);
    _areaSeries->setName("area series");
    _areaSeries->setUseOpenGL(true);
    _areaSeries->setPen(QPen(Qt::white, 1));

    _chart = new QChart();
    _chart->addSeries(_areaSeries);
    //_chart->setAnimationOptions(QChart::SeriesAnimations);
    _chart->legend()->setVisible(false);
    _chart->createDefaultAxes();
    _chart->setBackgroundVisible(false);
    _chart->axisX()->setVisible(false);
    _chart->axisY()->setVisible(false);
    _chart->setMargins(QMargins(-10, -10, -10, -10));

    //创建QChartView
    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);    //抗锯齿
    //_chartView->setRubberBand(QChartView::RectangleRubberBand);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    hLayout->addWidget(_chartView);
    this->setLayout(hLayout);
}
