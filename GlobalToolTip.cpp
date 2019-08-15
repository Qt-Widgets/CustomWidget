#include "GlobalToolTip.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QMainWindow>
#include <QPixmap>
#include <qpropertyanimation.h>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPainter>
#include <QTimer>

GlobalToolTip::GlobalToolTip(QWidget* parent):QWidget{parent}, 
			moveOutAnimation{ new QPropertyAnimation{ this ,"pos",this }},
			opacityAnimation{new QPropertyAnimation{this,"windowOpacity",this}},
			animationGroup{new QParallelAnimationGroup{ this }}
{
	setWindowFlags(Qt::FramelessWindowHint|Qt::Tool| Qt::WindowStaysOnTopHint);
	//setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_TranslucentBackground, true);
	opacityAnimation->setStartValue(0);
	opacityAnimation->setEndValue(1);
	opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);
	moveOutAnimation->setEasingCurve(QEasingCurve::OutQuad);
	animationGroup->addAnimation(moveOutAnimation);
	animationGroup->addAnimation(opacityAnimation);
}

void GlobalToolTip::popup(const QString & text, int type)
{
	//attain mainWindow
	QMainWindow* rootWindow{ nullptr };
	QWidgetList topWidgetList = QApplication::topLevelWidgets();
	for (QWidget* w: topWidgetList)
	{
		if (QMainWindow* window = qobject_cast<QMainWindow*>(w))
		{
			rootWindow = qobject_cast<QMainWindow*>(w);
		}
	}
	auto toolTip = instance(rootWindow);
	if (toolTip->isVisible())
	{
		toolTip->hide();
	}
	QFont font{ "Microsoft YaHei" };
	font.setPointSize(9);
	QFontMetrics fontMetrics{ font };
	int textWidth = fontMetrics.width(text);
	toolTip->resize(textWidth + 86, 56);
	toolTip->setText(text);
	toolTip->setType(type);
	QRect rootWindowRect = rootWindow->geometry();
	toolTip->updatePos(rootWindow);
	toolTip->show();

}

void GlobalToolTip::paintEvent(QPaintEvent * event)
{
	QPixmap contentImage;
	QPainter tipPainter{ this };
	tipPainter.save();
	tipPainter.setRenderHint(QPainter::Antialiasing, true);
	tipPainter.setBrush(QColor{245,245,247});
	tipPainter.drawRoundedRect(rect().adjusted(2, 2, -2, -2),26,26);
	tipPainter.restore();
	switch (contentType)
	{
	case 1:
		contentImage.load(":/img/Resources/image/success.png");
		tipPainter.setPen(QColor{ 51,51,51 });
		break;
	case 2:
		contentImage.load(":/img/Resources/image/fail.png");
		tipPainter.setPen(QColor{ 51,51,51 });
		break;
	case 0:
		contentImage.load(":/img/Resources/image/normal.png");
		tipPainter.setPen(QColor{ Qt::black });
		break;
	default:
		break;
	}
	tipPainter.drawPixmap(QPoint{ 26,16 }, contentImage);
	//the width between pixmap and text is 10
	const QRect textRect = { 60,16,width()-86,24 };
	QFont font;
	font.setPointSize(9);
	font.setFamily("Microsoft YaHei");
	tipPainter.setFont(font);
	tipPainter.drawText(textRect,Qt::AlignVCenter, contentText);

}

void GlobalToolTip::setText(const QString & text)
{
	contentText = text;
	if (text.length()<5)
	{
		QTimer::singleShot(2000, [this]() {
			if (isVisible())
			{
				close();
			}
		});
	}
	else
	{
		QTimer::singleShot(4000, [this]() {
			if (isVisible())
			{
				close();
			}
		});
	}
	
}

void GlobalToolTip::setType(int type)
{
	contentType = type;
}

void GlobalToolTip::updatePos(QWidget* parent)
{
	QPoint firstP{ parent->mapToGlobal(QPoint{ parent->width() / 2,0 }) };
	QPoint secondP{ parent->mapToGlobal(QPoint{ parent->width() / 2,74 }) };
	moveOutAnimation->setStartValue(firstP);
	moveOutAnimation->setEndValue(secondP);
	animationGroup->start();
}

GlobalToolTip* GlobalToolTip::instance(QWidget* parent)
{
	static auto instance = new GlobalToolTip{};
	//instance.setParent(parent);
	//instance.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//instance.setAttribute(Qt::WA_DeleteOnClose);
	//instance.setAttribute(Qt::WA_TranslucentBackground, true);
	return instance;
}
