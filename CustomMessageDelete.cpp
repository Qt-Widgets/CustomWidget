#include "CustomMessageDelete.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QTimer>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
namespace zhst
{
	CustomMessageDelete::CustomMessageDelete(QWidget *parent)
		: QWidget(parent)
	{
		setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		setWindowFlag(Qt::Dialog/*Popup*/);
		setWindowModality(Qt::ApplicationModal);
		setAttribute(Qt::WA_TranslucentBackground, true);
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/MessageBox.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		
		createWidgets();
		createLayouts();
	}

	CustomMessageDelete::~CustomMessageDelete()
	{
	}
	void CustomMessageDelete::createWidgets()
	{
		this->setFixedWidth(300);
		this->setFixedHeight(170);
		_mainWidget = new QWidget(this);
		_mainWidget->setObjectName("MessageDeletemainWidget");
		_textLabel = new QLabel(this);
		_textLabel->setObjectName("MessageDeletetextLabel");
		_textLabel->setAlignment(Qt::AlignCenter);

		_toolWidget = new QWidget();
		_toolWidget->setObjectName("MessageDeletetoolWidget");
		_toolWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		_closeButton = new QPushButton(QString(u8"取消"));
		_closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		_closeButton->setObjectName("MessageDeletecloseButton");
		_okButton = new QPushButton(QString(u8"确定"));
		_okButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		_okButton->setObjectName("MessageDeletesubmitButton");

		connect(_closeButton, &QPushButton::clicked, this, [this]()
		{
			emit submitCancleBtnClicked();
			this->close();
		});
		connect(_okButton, &QPushButton::clicked, this, &CustomMessageDelete::submitOkButtonClicked);
		connect(_okButton, &QPushButton::clicked, this, &CustomMessageDelete::close);
	}
	void CustomMessageDelete::createLayouts()
	{
		QHBoxLayout *toolHLayout = new QHBoxLayout();
		toolHLayout->setSpacing(0);
		toolHLayout->setContentsMargins(0, 0, 0, 0);
		toolHLayout->addWidget(_closeButton);
		toolHLayout->addWidget(_okButton);
		_toolWidget->setLayout(toolHLayout);

		QVBoxLayout *mainVLayout = new QVBoxLayout();
		mainVLayout->setSpacing(0);
		mainVLayout->setContentsMargins(0, 0, 0, 0);
		mainVLayout->addWidget(_textLabel);
		mainVLayout->addWidget(_toolWidget);
		_mainWidget->setLayout(mainVLayout);

		QHBoxLayout *mainHLayout = new QHBoxLayout();
		mainHLayout->setSpacing(0);
		mainHLayout->setContentsMargins(0, 0, 0, 0);
		mainHLayout->addWidget(_mainWidget);
		this->setLayout(mainHLayout);
	}
	void CustomMessageDelete::showEvent(QShowEvent * event)
	{
	}
	void CustomMessageDelete::setText(QString text)
	{
		_textLabel->setText(text);
	}

	void CustomMessageDelete::setData(int id)
	{
		this->id = id;
	}

	int CustomMessageDelete::getData()
	{
		return id;
	}

	QString CustomMessageDelete::getUuid() const
	{
		return uuid;
	}

	void CustomMessageDelete::setUuid(const QString& uu)
	{
		uuid = uu;
	}

	void CustomMessageDelete::setCancleBtnText(const QString& text)
	{
		_closeButton->setText(text);
	}

	void CustomMessageDelete::setOkBtnText(const QString& text)
	{
		_okButton->setText(text);
	}
	void CustomMessageDelete::setContentFontSize(int size)
	{
		_textLabel->setAlignment(Qt::AlignLeft|Qt::AlignCenter);
		_textLabel->setStyleSheet("#MessageDeletetextLabel{color: rgb(255, 255, 255);font-size: " + QString::number(size) + "px;}");
	}
	void CustomMessageDelete::setCancelBtnHidden(bool status)
	{
		_closeButton->setHidden(status);
	}
}
