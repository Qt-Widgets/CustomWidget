#pragma once

#include <QDialog>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_MESSAGEBOX_API Q_DECL_EXPORT
#else
#define XW_MESSAGEBOX_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QLabel;
class QTimer;
QT_END_NAMESPACE

namespace zhst
{
	class XW_MESSAGEBOX_API  CustomMessageInfo : public QDialog
	{
		Q_OBJECT
	public:
		CustomMessageInfo(QWidget *parent = 0);
		~CustomMessageInfo();
		void setText(QString);
		void setType(bool);//false为失败，true为成功
		void setShowTime(int);//提示框显示的时间长度
        static void information(QWidget* parent, const QString& content);
        static void warning(QWidget* parent, const QString& content);
	protected:
		virtual bool event(QEvent *event);
	private slots:
		void handleTimertimeout();
        
	private:
		QWidget *_mainWidget;
		QLabel *_imgLabel;
		QLabel *_textLabel;

		QTimer *_timer;
		int _showTime=1000;

		void createWidgets();
		void createLayouts();

	protected:
		virtual void showEvent(QShowEvent * event);
	};
}
