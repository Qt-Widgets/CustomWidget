/**
* @file CustomProgressBar.h
* @brief ProgressBar
* @author nzx
* @date 2018-9-1 10:48:53
*/
#pragma once

#include <QWidget>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_PROGRESSBAR_API Q_DECL_EXPORT
#else
#define XW_PROGRESSBAR_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QProgressBar;
class QLabel;
QT_END_NAMESPACE

namespace zhst
{
	class XW_PROGRESSBAR_API  CustomProgressBar : public QWidget
	{
		Q_OBJECT
	public:
		explicit CustomProgressBar(QWidget *parent = 0);
		~CustomProgressBar();
		void init();
		void setProgressValue(int value);
		void setProgressValue(double value);
		void setProgressWidth(int width);
		void setFontColor(QString color, int fontsize = 14);
		void setFontColor(int r, int g, int b, int fontsize = 14);
		int getProgressValue();
		double getRealProgressValue();
		void setProgressBarObjectName(QString objectName);
	private:
		QProgressBar *_progressBar;
		QLabel *_progressLabel;

		QFont font;
		double _value = 0;
	signals:

	public slots :
		void updateValueLabel(int value);
		void updateValueLabel(double value);

	};
}