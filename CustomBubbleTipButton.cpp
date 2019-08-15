#include "CustomBubbleTipButton.h"
#include <QPainter>
#include <QRect>
#include <QColor>
namespace zhst
{
	CustomBubbleTipButton::CustomBubbleTipButton(QWidget *parent)
		:QPushButton(parent)
	{
	}
	CustomBubbleTipButton::~CustomBubbleTipButton()
	{
		if (_icon_movie)
			delete _icon_movie;
		if (_icon_pixmap)
			delete _icon_pixmap; 
	}
	void CustomBubbleTipButton::setLabelWidth(int w)
	{
		_LABEL_WIDTH = w;
	}
	void CustomBubbleTipButton::setLabelHeight(int w)
	{
		_LABEL_HEIGHT = w;
	}

	void CustomBubbleTipButton::setBubbleText(QString s)
	{
		_bubbleText = s;
	}

	void CustomBubbleTipButton::setPixIcon(QPixmap* ico)
	{
		if (ico != nullptr)
		{
			_icon_pixmap = ico;
		}
		isMovie = false;
        if(connectObject!=nullptr)
            disconnect(connectObject);
	}

	void CustomBubbleTipButton::setMovIcon(QMovie* ico)
	{
		if (ico != nullptr)
		{
			_icon_movie = ico;
		}
		isMovie = true;
        connectObject = connect(_icon_movie, &QMovie::frameChanged, this, &CustomBubbleTipButton::updateMovie);
		_icon_movie->setBackgroundColor(QColor(0, 0, 0));
		_icon_movie->start();
	}
	void CustomBubbleTipButton::setIconSize(int w, int h)
	{
		_icon_height = h;
		_icon_width = w;
	}

	void CustomBubbleTipButton::setFailedTaskNum(int num)
	{
		this->_failedTaskNum = num;
		update();
	}

	void CustomBubbleTipButton::paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		//QRect rt = rect();
		//whl:
		//QRect rt1 = QRect(rt.right() - _LABEL_WIDTH, rt.top(), _LABEL_WIDTH, _LABEL_HEIGHT);
		QRect rt1 = QRect(_icon_width / 2, 0, _LABEL_WIDTH, _LABEL_HEIGHT);
		QColor groudColor;
		QColor sideColor;
		if (_failedTaskNum == 0)
		{
			groudColor.setRgb(33, 177, 50);
			sideColor.setRgb(4, 65, 11);
		}
		else
		{
			groudColor.setRgb(235, 96, 46);
			sideColor.setRgb(107, 38, 13);
		}

		painter.save();
		if (isMovie)
		{
			if (_icon_pixmap != nullptr)
			{
				painter.drawPixmap(QPoint(0, height() / 2 - _icon_height / 2), _icon_movie->currentPixmap().scaled(QSize(_icon_width, _icon_height)));
			}
		}
		else
		{
			if (_icon_pixmap != nullptr)
			{
				painter.drawPixmap(QPoint(0, height() / 2 - _icon_height / 2), _icon_pixmap->scaled(QSize(_icon_width, _icon_height)));
			}
		}
		painter.restore();

		painter.save();
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setPen(QPen(sideColor,1, Qt::SolidLine));
		painter.setBrush(QBrush(groudColor));
		//whl:加条件
		if (_bubbleText != "0")
		{
			painter.drawRoundedRect(rt1, 8, 8);
			painter.restore();
			
			painter.save();
			painter.setPen(QPen(Qt::white, 1, Qt::SolidLine));
			QFont font;
			font.setFamily("Microsoft YaHei");
			font.setPointSize(9);
			//int widthOfTitle = font.fontMetrics().width(title);//字符串显示的像素大小
			//p.drawText(this->width() / 2 - widthOfTitle / 2, 100, title);
			painter.setFont(font);
			
			painter.drawText(rt1, Qt::AlignCenter, _bubbleText);
		}
		
		//painter.drawText(rt, Qt::AlignCenter, "123");
		painter.restore();
	}

    void CustomBubbleTipButton::updateMovie()
    {
        update();
    }
}