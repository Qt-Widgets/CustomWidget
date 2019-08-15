#include "CustomMessageBox.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <Windows.h>
#include <QEvent>
#include <QMouseEvent>
namespace zhst
{
	CustomMessageInfo::CustomMessageInfo(QWidget *parent)
		: QDialog(parent)
	{
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/MessageBox.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
		setAttribute(Qt::WA_TranslucentBackground, true);
		createWidgets();
		createLayouts();
	}

	CustomMessageInfo::~CustomMessageInfo()
	{
	}
	void CustomMessageInfo::createWidgets()
	{
		this->setFixedWidth(300);
		this->setFixedHeight(170);
		_mainWidget = new QWidget(this);
		_mainWidget->setObjectName("MessageInfomainWidget");
		_imgLabel = new QLabel(this);
		_imgLabel->setObjectName("MessageInfoimgLabel");
		_textLabel = new QLabel(this);
		_textLabel->setObjectName("MessageInfotextLabel");
		_textLabel->setAlignment(Qt::AlignCenter);
		_timer = new QTimer();
		connect(_timer, &QTimer::timeout, this, &CustomMessageInfo::handleTimertimeout);
	}
	void CustomMessageInfo::createLayouts()
	{
		QVBoxLayout *mainVLayout = new QVBoxLayout();
		mainVLayout->setSpacing(0);
		mainVLayout->addWidget(_imgLabel);
		mainVLayout->addWidget(_textLabel);
		_mainWidget->setLayout(mainVLayout);

		QHBoxLayout *mainHLayout = new QHBoxLayout();
		mainHLayout->setSpacing(0);
		mainHLayout->setContentsMargins(0, 0, 0, 0);
		mainHLayout->addWidget(_mainWidget);
		this->setLayout(mainHLayout);
	}
	bool CustomMessageInfo::event(QEvent * event)
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
		else if (event->type() == QEvent::MouseButtonPress)
		{
			this->hide();
		}
		return QWidget::event(event);
	}
	void CustomMessageInfo::setShowTime(int time)
	{
		_showTime = time;
	}
	void CustomMessageInfo::setText(QString text)
	{
		this->close();
		if (_timer->isActive())
		{
			_timer->stop();
		}
		_timer->start(_showTime);
		_textLabel->setText(text);
	}
	void CustomMessageInfo::setType(bool type)
	{
		if (type)
		{
			_imgLabel->setStyleSheet("background: url(:/CustomWidget/Resources/success.png) no-repeat;background-position: bottom center;");
		}
		else
		{
			_imgLabel->setStyleSheet("background: url(:/CustomWidget/Resources/warning.png) no-repeat;background-position: bottom center;");
		}
	}
	void CustomMessageInfo::handleTimertimeout()
	{
		_timer->stop();
		this->close();
	}

    void CustomMessageInfo::information(QWidget* parent, const QString& content)
    {
        CustomMessageInfo dlg(parent);
        dlg.setType(true);
        dlg.setText(content);
        dlg.exec();
    }

    void CustomMessageInfo::warning(QWidget* parent, const QString& content)
    {
        CustomMessageInfo dlg(parent);
        dlg.setType(false);
        dlg.setText(content);
        dlg.exec();
    }
	void CustomMessageInfo::showEvent(QShowEvent * event)
	{
	}
}
