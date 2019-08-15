#include "CustomCalendar.h"
#include <QLabel>
#include <QListWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCalendarWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QFile> 
#include <QDebug>
#include "./GlobalToolTip.h"

namespace zhst
{
	CustomCalendar::CustomCalendar(QWidget *parent)
		:QWidget(parent)
	{
		setWindowFlag(Qt::Popup);
		createWidgets();
		createLayouts();
		QFile qss(":/CustomWidget/Resources/Calendar.qss");
		qss.open(QIODevice::ReadOnly);
		setStyleSheet(qss.readAll());
		qss.close();
	}
	CustomCalendar::~CustomCalendar()
	{
	}
	void CustomCalendar::createWidgets()
	{
		this->setFixedHeight(400);
		this->setFixedWidth(300);
		_mainWidget = new QWidget();
		_mainWidget->setObjectName("DateTimeDialog");
		_calendarWidget = new QCalendarWidget();
		_calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
		_calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
		_calendarWidget->setMaximumDate(QDate::currentDate());
		_calendarWidget->setMinimumDate(QDate(1971,1,1));
		connect(_calendarWidget, &QCalendarWidget::currentPageChanged, this, &CustomCalendar::handlecurrentPageChanged);

		QLineEdit  *yearLineEdit = _calendarWidget->findChild<QLineEdit *>();
		yearLineEdit->setMaxLength(4);

		_timeWidget = new QWidget();
		_timeWidget->setObjectName("timeWidget");
		_timeNameLabel = new QLabel();
		_timeNameLabel->setText(QString(u8"请输入时间"));
		QRegularExpression hourReg{"^(0?[0-9]|1[0-9]|2[0-3])$"};
		auto *hourValidator = new QRegularExpressionValidator(hourReg, this);
		_hourLineEdit = new QLineEdit(this);
		_hourLineEdit->setPlaceholderText(QString(u8"00"));
		_hourLineEdit->setValidator(hourValidator);
		_hourLineEdit->setMaxLength(2);
        _hourLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
		_splithourLabel = new QLabel();
		_splithourLabel->setText(QString(u8":"));
		QRegularExpression minReg{ "^(0?[0-9]|[1-5][0-9])$" };
		QValidator *minu = new QRegularExpressionValidator(minReg, this);
		_minuteLineEdit = new QLineEdit(this);
		_minuteLineEdit->setPlaceholderText(QString(u8"00"));
		_minuteLineEdit->setValidator(minu);
		_minuteLineEdit->setMaxLength(2);
        _minuteLineEdit->setContextMenuPolicy(Qt::NoContextMenu);
		_splitminuteLabel = new QLabel();
		_splitminuteLabel->setText(QString(u8":"));
		_secondLineEdit = new QLineEdit(this);
		_secondLineEdit->setPlaceholderText(QString(u8"00"));
		_secondLineEdit->setValidator(minu);
		_secondLineEdit->setMaxLength(2);
        _secondLineEdit->setContextMenuPolicy(Qt::NoContextMenu);

		_submitButton = new QPushButton();
		_submitButton->setText(QString(u8"确定"));
		_submitButton->setObjectName("DateTimeDialogSubmitButton");
		_submitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
		connect(_submitButton, &QPushButton::clicked, this, &CustomCalendar::handlesubmitButtonClicked);
		//_messageInfo = new zhst::CustomMessageInfo(this);
	}
	void CustomCalendar::createLayouts()
	{
		QHBoxLayout *timeHLayout = new QHBoxLayout();
		timeHLayout->setContentsMargins(0, 20, 0, 20);
		timeHLayout->addWidget(_timeNameLabel);
		timeHLayout->addWidget(_hourLineEdit);
		timeHLayout->addWidget(_splithourLabel);
		timeHLayout->addWidget(_minuteLineEdit);
		timeHLayout->addWidget(_splitminuteLabel);
		timeHLayout->addWidget(_secondLineEdit);
		_timeWidget->setLayout(timeHLayout);

		QHBoxLayout *submitHLayout = new QHBoxLayout();
		submitHLayout->addWidget(_submitButton);

		QVBoxLayout *mainVLayout = new QVBoxLayout();
		mainVLayout->addWidget(_calendarWidget);
		mainVLayout->addWidget(_timeWidget);
		mainVLayout->addLayout(submitHLayout);
		_mainWidget->setLayout(mainVLayout);

		QHBoxLayout *mainHLayout = new QHBoxLayout();
		mainHLayout->setSpacing(0);
		mainHLayout->setContentsMargins(0, 0, 0, 0);
		mainHLayout->addWidget(_mainWidget);
		this->setLayout(mainHLayout);
	}
	void CustomCalendar::handlecurrentPageChanged(int year, int month)
	{
		_calendarWidget->setSelectedDate(QDate(year, month, 1));
	}
	void CustomCalendar::handlesubmitButtonClicked()
	{
		_dateTime.setDate(_calendarWidget->selectedDate());
		QTime time;
		time.setHMS(_hourLineEdit->text().toInt(), _minuteLineEdit->text().toInt(), _secondLineEdit->text().toInt(), 0);
		_dateTime.setTime(time);
		if (_dateTime > QDateTime::currentDateTime())
		{
			GlobalToolTip::popup(QString(u8"选择的时间不能超过当前时间"), 2);
			return;
		}
		emit submitDateTime(_dateTime, _typeFlag);
		this->close();
	}
	void CustomCalendar::setCurrentDateTime(QDateTime currenttime)
	{
		_dateTime = currenttime;
		_calendarWidget->setSelectedDate(currenttime.date());
		QTime Time;
		Time = currenttime.time();
		_hourLineEdit->setText(QString::number(Time.hour()));
		_minuteLineEdit->setText(QString::number(Time.minute()));
		_secondLineEdit->setText(QString::number(Time.second()));
	}
	void CustomCalendar::setType(bool type)
	{
		_typeFlag = type;
		_searchFlag = true;
	}
	void CustomCalendar::setLimitTime(QDateTime time)
	{
		//qDebug() << _limitTime;
		_limitTime = time;
	}
}