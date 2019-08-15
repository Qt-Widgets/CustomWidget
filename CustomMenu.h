/**
* @file CustomMenu.h
* @brief Menu
* @author nzx
* @date 2018-8-27 15:02:43
*/
#pragma once

#include <QMenu>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_MENU_API Q_DECL_EXPORT
#else
#define XW_MENU_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QEvent;
QT_END_NAMESPACE

namespace zhst
{
	class XW_MENU_API  CustomMenu : public QMenu
	{
		Q_OBJECT
	public:
		explicit CustomMenu(QWidget *parent = 0);
		~CustomMenu();
		void init();

	protected:
		virtual bool event(QEvent *event);
		virtual bool eventFilter(QObject * watched, QEvent * event);

	signals:

	public slots:


	};
}