/**
* @file CustomMenu.cpp
* @brief 使用类集合
* @author nzx
* @date 2018-8-27 15:02:43
*/
#include "CustomMenu.h"
#include <QEvent>
#include <QMouseEvent>
#include <QFile>
#include <Windows.h>
#include <QDebug>
//#include "dwmapi.h"
#include "WinUser.h"
#pragma comment (lib,"user32.lib")
namespace zhst
{
	CustomMenu::CustomMenu(QWidget *parent)
		:QMenu(parent)
	{
		this->installEventFilter(this);
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/Menu.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		init();
	}

	CustomMenu::~CustomMenu()
	{

	}

	void CustomMenu::init()
	{
		this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
		this->setAttribute(Qt::WA_TranslucentBackground);
		this->setObjectName("CustomMenu");

	}

	bool CustomMenu::event(QEvent * event)
	{
		static bool class_amended = false;
		if (event->type() == QEvent::WinIdChange)
		{
			HWND hwnd = reinterpret_cast<HWND>(winId());
			if (class_amended == false)
			{
				class_amended = true;
				DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
				class_style &= ~CS_DROPSHADOW;
				::SetClassLong(hwnd, GCL_STYLE, class_style);
			}

		}
		return QWidget::event(event);
	}

	bool CustomMenu::eventFilter(QObject * watched, QEvent * event)
	{
		if (event->type() == QEvent::MouseButtonPress) {
			if (static_cast<QMouseEvent*>(event)->button() == Qt::RightButton)
			{
				return true;
			}
		}
		if (event->type() == QEvent::MouseButtonRelease) {
			if (static_cast<QMouseEvent*>(event)->button() == Qt::RightButton)
			{
				return true;
			}
		}
		if (event->type() == QEvent::MouseButtonDblClick) {
			if (static_cast<QMouseEvent*>(event)->button() == Qt::RightButton)
			{
				return true;
			}
		}
		return false;
	}

}
