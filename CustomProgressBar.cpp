#include "CustomProgressBar.h"
#include <QProgressBar>
#include <QLabel>
#include <QFile>
#include <QHBoxLayout>
#include <QFont>
#include <QDebug>
namespace zhst
{
	CustomProgressBar::CustomProgressBar(QWidget *parent)
		:QWidget(parent)
	{
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/ProgressBar.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		init();
	}
	CustomProgressBar::~CustomProgressBar()
	{
		qDebug() <<"*********************real delete:"<< this;
	}
	void CustomProgressBar::init()
	{
		this->setObjectName("mainWidget");
		_progressBar=new QProgressBar(this);
		_progressBar->setObjectName("customProgressBar");
		_progressBar->setFixedHeight(8);
		_progressBar->setRange(0, 100);
		_progressBar->setValue(0);
		//_progressBar->setTextVisible(false);

		_progressLabel = new QLabel("0%",this);
		_progressLabel->setObjectName("customProgressLabel");
		setFontColor("white");

		//connect(_progressBar, &QProgressBar::valueChanged, this, &CustomProgressBar::updateValueLabel);

		QHBoxLayout *hLayout = new QHBoxLayout(this);
		hLayout->setMargin(0);
		hLayout->setSpacing(10);
		hLayout->addWidget(_progressBar, 0 , Qt::AlignLeft | Qt::AlignVCenter);
		hLayout->addWidget(_progressLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
		this->setLayout(hLayout);

	}
	void CustomProgressBar::setProgressValue(int value)
	{
		_value = value;
		updateValueLabel(value);
	}
	void CustomProgressBar::setProgressValue(double value)
	{
		_value = value;
		//_progressBar->setValue(value);
		updateValueLabel(value);
	}
	void CustomProgressBar::setProgressWidth(int width)
	{
		_progressBar->setFixedWidth(width);
		this->setFixedWidth(width + 45);
		
	}
	void CustomProgressBar::setFontColor(QString color, int fontsize)
	{
		_progressLabel->setStyleSheet("QLabel#customProgressLabel{ color:"+color+"; font-size: " + QString::number(fontsize) + "px; font-family: \"Microsoft YaHei\";}");
	}
	void CustomProgressBar::setFontColor(int r, int g, int b, int fontsize)
	{
		_progressLabel->setStyleSheet("QLabel#customProgressLabel{ color:rgb(" + QString(u8"%1,%2,%3") + "); font-size: " + QString::number(fontsize) + "px; font-family: \"Microsoft YaHei\";}");
	}
	int CustomProgressBar::getProgressValue()
	{
		return _value;
	}
	double CustomProgressBar::getRealProgressValue()
	{
		return _value;
	}
	void CustomProgressBar::updateValueLabel(int value)
	{
		_progressBar->setValue(value);
		if (value > 100 || value < 0)
			value = 0;
		_progressLabel->setText(QString::number(value) + "%");
	}
	void CustomProgressBar::updateValueLabel(double value)
	{
		_progressBar->setValue(value);
		if (value > 100 || value < 0)
			value = 0;
		if (value == 100)
		{
			_progressLabel->setText(QString::number(((int)value)) + "%");
		}
		else
		{
			_progressLabel->setText(QString::number(value, 'f', 2) + "%");
		}
	}

	void CustomProgressBar::setProgressBarObjectName(QString objectName)
	{
		_progressBar->setObjectName(objectName);
		QFile qss(":/CustomWidget/Resources/ProgressBar.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
	}
}

