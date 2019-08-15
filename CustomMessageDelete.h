#pragma once

#include <QWidget>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_MESSAGEDELETE_API Q_DECL_EXPORT
#else
#define XW_MESSAGEDELETE_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

namespace zhst
{
	class XW_MESSAGEDELETE_API  CustomMessageDelete : public QWidget
	{
		Q_OBJECT
	public:
		CustomMessageDelete(QWidget *parent = 0);
		~CustomMessageDelete();
		void setText(QString);
		void setData(int id);
		int getData();
		QString getUuid()const;
		void setUuid(const QString& uu);
		void setCancleBtnText(const QString& text);
		void setOkBtnText(const QString& text);
		void setContentFontSize(int size);
		void setCancelBtnHidden(bool status = false);
	signals:
		void submitOkButtonClicked();
		void submitCancleBtnClicked();
	private:
		QWidget *_mainWidget;
		QLabel *_textLabel;
		QWidget *_toolWidget;
		QPushButton *_closeButton;
		QPushButton *_okButton;
		int id{-1};
		QString uuid{""};

		void createWidgets();
		void createLayouts();
	protected:
		virtual void showEvent(QShowEvent *event);
	};

}
