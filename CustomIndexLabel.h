/**
* @file CustomIndexLabel.h
* @brief IndexLabel
* @author nzx
* @date 2018-8-31 20:40:57
*/
#pragma once

#include <QLabel>
#include <QMap>
#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_INDEXLABEL_API Q_DECL_EXPORT
#else
#define XW_INDEXLABEL_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QEvent;
QT_END_NAMESPACE

namespace zhst
{
	class XW_INDEXLABEL_API  CustomIndexLabel : public QLabel
	{
		//Q_OBJECT
	public:
		explicit CustomIndexLabel(QWidget *parent = 0);
		~CustomIndexLabel();
		void init();
		void setIndex(int index);
		void setBackgroundColor(int r, int g, int b, int a = 255);
		void setFontColor(int r, int g, int b);
		void setFontColor(QString color);//example:black;white;green
	protected:
		void paintEvent(QPaintEvent *e);
	private:
		void reloadStyleSheet();

		QMap<QString, QString> styleSheetMap;
		int fontSize;
		//signals:

		//public slots :


		};
}