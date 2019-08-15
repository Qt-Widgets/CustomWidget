#pragma once

#include <QPushButton>
#include <QPixmap>
#include <QMovie>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_BUBBLETIPBUTTON_API Q_DECL_EXPORT
#else
#define XW_BUBBLETIPBUTTON_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS
namespace zhst
{
	class XW_BUBBLETIPBUTTON_API  CustomBubbleTipButton : public QPushButton
	{
		Q_OBJECT
	public:
		CustomBubbleTipButton(QWidget *parent = 0);
		~CustomBubbleTipButton();
		void setLabelWidth(int w);
		void setLabelHeight(int h);
		void setBubbleText(QString s);
		void setPixIcon(QPixmap* ico = nullptr);
		void setMovIcon(QMovie* ico = nullptr);
		void setIconSize(int w, int h);
		void setFailedTaskNum(int num);
		//int _LABEL_WIDTH = 18;
		//int _LABEL_HEIGHT = 12;
		//int _icon_width = 22;
		//int _icon_height = 22;
		int _LABEL_WIDTH = 24;
		int _LABEL_HEIGHT = 16;
		int _icon_width = 22;
		int _icon_height = 20;
	private:	
		QString _bubbleText = "0";

		int _failedTaskNum = 0;
		bool isMovie = false;

		QPixmap* _icon_pixmap = nullptr;
		QMovie* _icon_movie = nullptr;
        QMetaObject::Connection connectObject;
	protected:
		virtual void paintEvent(QPaintEvent * event);
    private slots:
        void updateMovie();
	};
}