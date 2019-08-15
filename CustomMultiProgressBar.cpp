#include "CustomMultiProgressBar.h"
#include <QPainter>
#include <QPalette>
#include <QColor>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QDateTime>
#include <QStyleOption>
#include <QStylePainter>

namespace zhst
{
	CustomMultiProgressBar::CustomMultiProgressBar(QWidget *parent)
		:QWidget(parent)
	{
		createWidget();
		setCursor(Qt::PointingHandCursor);
        setMouseTracking(true);
	}
	CustomMultiProgressBar::~CustomMultiProgressBar()
	{

	}
	void CustomMultiProgressBar::createWidget()
	{
		initParameter();
	}
	void CustomMultiProgressBar::initParameter()
	{
		_value = 0;
		_extraValue = 0;
		_minimum = 0;
		_maximum = 100;
		_backgroundColor.setRgb(105, 105, 105);
		_backgroundPixmap = generateColorImage(size(), _backgroundColor);
		_chunkColor.setRgb(0, 0, 0);
		_pixmap = generateColorImage(size(), _chunkColor);
		_extraChunkColor.setRgb(100, 100, 100);
		_extraPixmap = generateColorImage(size(), _extraChunkColor);
		_gapColor.setRgb(0, 100, 0);
		_gapPixmap = generateColorImage(size(), _gapColor);
		_gapList.clear();
		update();
	}
	void CustomMultiProgressBar::paintEvent(QPaintEvent *event)
	{
		QWidget::paintEvent(event);
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);
		QRect rect = QRect(0, 0, width(), height());
		int rectRaidus = height() / 2;
		int yStart = height() / 4;
		int yEnd = height() / 4 * 3;

		//painter  background
		painter.save();
		QBrush backgroundBrush;
		backgroundBrush.setTexture(_backgroundPixmap);
		painter.setBrush(backgroundBrush);
		painter.setPen(_backgroundColor);
		painter.drawRoundedRect(rect, rectRaidus, rectRaidus);
		painter.restore();

		//painter  extra progressBar
		if (extraValue() != 0)
		{
			const double extraRatio = (double)(extraValue() - minimum()) / (maximum() - minimum());
			double extraLength = (rect.width() * extraRatio);
			QRectF extraFillRect = rect.adjusted(0, 0, extraLength - rect.width(), 0);
			painter.save();
			QBrush extraBrush;
			extraBrush.setTexture(_extraPixmap);
			painter.setBrush(extraBrush);
			painter.setPen(_extraChunkColor);
			painter.drawRoundedRect(extraFillRect, rectRaidus, rectRaidus);
			painter.restore();
		}

		//painter normal progressBar
		if (value() != 0)
		{
			const double ratio = (double)(value() - minimum()) / (maximum() - minimum());
			double length = (rect.width() * ratio);
			QRectF fillRect = rect.adjusted(0, 0, length - rect.width(), 0);
			painter.save();
			QBrush brush;
			brush.setTexture(_pixmap);
			painter.setBrush(brush);
			painter.setPen(_chunkColor);
			painter.drawRoundedRect(fillRect, rectRaidus, rectRaidus);
			painter.restore();
		}

		//painter gap progressBar
		if (_gapList.size() > 0)
		{
			for (auto& point : _gapList)
			{
				const double startRatio = (double)(point.x() - minimum()) / (maximum() - minimum());
				double startLength = (rect.width() * startRatio);
				const double endRatio = (double)(point.y() - minimum()) / (maximum() - minimum());
				double endLength = (rect.width() * endRatio);
				QRectF fillRect = rect.adjusted(startLength, 0, endLength - rect.width(), 0);
				painter.save();
				QBrush brush;
				brush.setTexture(_gapPixmap);
				painter.setBrush(brush);
				painter.setPen(_gapColor);
				painter.drawRect(fillRect);
				painter.restore();
			}
		}
	}
	void CustomMultiProgressBar::mousePressEvent(QMouseEvent *event)
	{
		Q_UNUSED(event);
		m_bPressed = true;
	}
	void CustomMultiProgressBar::mouseReleaseEvent(QMouseEvent *event)
	{
		m_bPressed = false;

		//避免双击触发2次
		qint64 iCurrentDateTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
		if (iCurrentDateTime - m_iLastClickDateTime > 400)
		{
			emit clicked(maximum()*event->x() / width());
			m_iLastClickDateTime = iCurrentDateTime;
		}
	}
	void CustomMultiProgressBar::mouseMoveEvent(QMouseEvent *event)
	{
        event->ignore();
	}
	void CustomMultiProgressBar::mouseDoubleClickEvent(QMouseEvent *event)
	{
		emit doubleClicked(maximum()*event->x() / width());
	}
	void CustomMultiProgressBar::setBackGroundColor(int r, int g, int b, int a)
	{
		_backgroundColor = QColor(r, g, b, a);
		_backgroundPixmap = generateColorImage(size(), _backgroundColor);
		update();
	}
	double CustomMultiProgressBar::value() const
	{
		return _value;
	}
	double CustomMultiProgressBar::extraValue() const
	{
		return _extraValue;
	}
	int CustomMultiProgressBar::minimum() const
	{
		return _minimum;
	}
	int CustomMultiProgressBar::maximum() const
	{
		return _maximum;
	}

    QList<QPointF>& CustomMultiProgressBar::getGapList()
    {
        // TODO: 在此处插入 return 语句
        return _gapList;
    }

    void CustomMultiProgressBar::clearGapList()
	{
		_gapList.clear();
		update();
	}

	void CustomMultiProgressBar::reset()
	{
		initParameter();
	}
	void CustomMultiProgressBar::setRange(int minimum, int maximum)
	{
		_minimum = minimum;
		_maximum = maximum;
	}
	void CustomMultiProgressBar::setValue(double value)
	{
		if (_value != value)
		{
			_value = value;
			emit valueChanged(value);
			update();
		}
	}
	void CustomMultiProgressBar::setExtraValue(double value)
	{
		if (_extraValue != value)
		{
			_extraValue = value;
			emit extraValueChanged(value);
			update();
		}
	}
	void CustomMultiProgressBar::setMinimum(int min)
	{
		_minimum = min;
	}
	void CustomMultiProgressBar::setMaximum(int max)
	{
		_maximum = max;
	}
	void CustomMultiProgressBar::setChunkColor(int r, int g, int b, int a)
	{
		_chunkColor = QColor(r, g, b, a);
		_pixmap = generateColorImage(size(), _chunkColor);
		update();
	}
	void CustomMultiProgressBar::setExtraChunkColor(int r, int g, int b, int a)
	{
		_extraChunkColor = QColor(r, g, b, a);
		_extraPixmap = generateColorImage(size(), _extraChunkColor);
		update();
	}
	void CustomMultiProgressBar::setGapList(QList<QPointF> list)
	{
		_gapList.clear();
		_gapList = list;
		update();
	}
	void CustomMultiProgressBar::addGap(QPointF gap)
	{
		_gapList.append(gap);
		update();
	}
	void CustomMultiProgressBar::setGapColor(int r, int g, int b, int a)
	{
		_gapColor = QColor(r, g, b, a);
		_gapPixmap = generateColorImage(size(), _gapColor);
		update();
	}
	QPixmap CustomMultiProgressBar::generateColorImage(QSize size, int r, int g, int b, int a)
	{
		QImage img(size, QImage::Format_RGB888);
		img.fill(QColor(r, g, b, a).rgba());
		QPixmap pixmap = QPixmap::fromImage(img);
		return pixmap;
	}
	QPixmap CustomMultiProgressBar::generateColorImage(QSize size, QColor color)
	{
		QImage img(size, QImage::Format_RGB888);
		img.fill(color.rgba());
		QPixmap pixmap = QPixmap::fromImage(img);
		return pixmap;
	}
}
