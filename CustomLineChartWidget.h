#pragma once
#include <QWidget>
#include <QList>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QAreaSeries>
#include <QList>
#include <QValueAxis>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_LINECHART_API Q_DECL_EXPORT
#else
#define XW_LINECHART_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS
class QGraphicsOpacityEffect;


QT_CHARTS_USE_NAMESPACE
namespace zhst
{
    class XW_LINECHART_API CustomLineChartWidget : public QWidget
    {
        Q_OBJECT

    public:
        CustomLineChartWidget(QWidget* parent = Q_NULLPTR);
        ~CustomLineChartWidget();

        void addSeriesPoint(double x, double y);
        void addPoints(QList<QPointF> pList);
        void setAreaColor(QColor color);
        void setXRange(double min, double max);
        void setBackgroundColor(QColor color);
        void clearPoints();
    private:
        void createWidget();

        QChartView* _chartView;
        QChart* _chart;
        QSplineSeries* _upperSeries;
        QSplineSeries* _lowerSeries;
        QAreaSeries* _areaSeries;

        bool isFirstUpdate = true;

        QList<QPointF> _upperPoints;
    };
}