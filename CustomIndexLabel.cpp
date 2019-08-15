#include "CustomIndexLabel.h"
#include "CustomIndexLabel.h"
#include <QDebug>
#include "qstyleoption"
#include <QPainter>

namespace zhst
{
	CustomIndexLabel::CustomIndexLabel(QWidget *parent)
		:QLabel(parent)
	{
		init();
	}
	CustomIndexLabel::~CustomIndexLabel()
	{
	}
	void CustomIndexLabel::init()
	{
		this->setText("0");
		this->setFixedSize(25, 25);
		this->setAlignment(Qt::AlignCenter);

		styleSheetMap["background-color"] = "rgb(255,255,255)";
		styleSheetMap["border-radius"] = "3px";
		styleSheetMap["color"]= "rgb(94, 94, 94)";
		reloadStyleSheet();
	}
	void CustomIndexLabel::setIndex(int index)
	{
		QString str = QString::number(index);
		int len = str.length();
		if (len <= 3)
		{
			fontSize = 12;
		}
		else if (len == 4)
		{
			fontSize = 10;
		}
		else if (len == 5)
		{
			fontSize = 9;
		}
		styleSheetMap["font-size"] = QString::number(fontSize)+"px";
		reloadStyleSheet();
		this->setText(str);
	}
	void CustomIndexLabel::setBackgroundColor(int r, int g, int b, int a)
	{
		styleSheetMap["background-color"] = "rgba("+QString(u8"%1,%2,%3,%4").arg(r).arg(g).arg(b).arg(a) + ")";
		reloadStyleSheet();
	}
	void CustomIndexLabel::setFontColor(int r, int g, int b)
	{
		styleSheetMap["color"] = "rgb("+QString(u8"%1,%2,%3").arg(r).arg(g).arg(b)+")";
		reloadStyleSheet();
	}
	void CustomIndexLabel::setFontColor(QString color)
	{
		styleSheetMap["color"] = color;
		reloadStyleSheet();
	}
	void CustomIndexLabel::reloadStyleSheet()
	{
		QString styleStr;
		QMap<QString, QString>::iterator it;
		for (it = styleSheetMap.begin(); it != styleSheetMap.end(); it++)
		{
			styleStr += it.key() + ":" + it.value() + ";";
		}
		this->setStyleSheet(styleStr);
	}


	void CustomIndexLabel::paintEvent(QPaintEvent *e) {
		QStyleOption opt;
		opt.init(this);
		QPainter p(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
		QLabel::paintEvent(e);
	}
}