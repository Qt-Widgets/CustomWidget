/**
* @file CustomMultiProgressBar.h
* @brief CustomMultiProgressBar
* @author nzx
* @date 2019-1-21 16:49:40
*/
#ifndef CUSTOMMULTIPROGRESSBAR_H
#define CUSTOMMULTIPROGRESSBAR_H
#include <QWidget>
#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_MULTI_PROGRESSBAR_API Q_DECL_EXPORT
#else
#define XW_MULTI_PROGRESSBAR_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS
namespace zhst
{
	class XW_MULTI_PROGRESSBAR_API CustomMultiProgressBar : public QWidget
	{
		Q_OBJECT
			Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
			Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
			Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
			Q_PROPERTY(double extraValue READ extraValue WRITE setExtraValue NOTIFY extraValueChanged)

	public:
		explicit CustomMultiProgressBar(QWidget *parent = 0);
		~CustomMultiProgressBar();

		void setBackGroundColor(int r, int g, int b, int a = 255);
		void setChunkColor(int r, int g, int b, int a = 255);
		void setExtraChunkColor(int r, int g, int b, int a = 255);
		void setGapColor(int r, int g, int b, int a = 255);

		double value() const;
		double extraValue() const;
		int minimum() const;
		int maximum() const;

        QList<QPointF>& getGapList();
		void clearGapList();
		public Q_SLOTS:
		void reset();
		void setRange(int minimum, int maximum);
		void setValue(double value);
		void setExtraValue(double value);
		void setMinimum(int min);
		void setMaximum(int max);

		void setGapList(QList<QPointF> list);
		void addGap(QPointF gap);
	Q_SIGNALS:
		void valueChanged(double value);
		void extraValueChanged(double value);
		void doubleClicked(int);
		void clicked(int);
	private:
		void createWidget();
		void initParameter();

		//self tool funtion
		QPixmap generateColorImage(QSize size, int r, int g, int b, int a = 255);
		QPixmap generateColorImage(QSize size, QColor color);

		//sum para
		double _value;
		double _extraValue;
		int _minimum;
		int _maximum;

		QColor _backgroundColor;
		QColor _chunkColor;
		QColor _extraChunkColor;
		QColor _gapColor;
		QPixmap _backgroundPixmap;
		QPixmap _pixmap;
		QPixmap _extraPixmap;
		QPixmap _gapPixmap;

    QList<QPointF> _gapList;
	bool m_bPressed = false;
	qint64 m_iLastClickDateTime = 0;//上一次点击鼠标的时间
	int _vMargin = 0;
protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void mousePressEvent(QMouseEvent *event) override;
		virtual void mouseReleaseEvent(QMouseEvent *event) override;
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
	};
}
#endif
