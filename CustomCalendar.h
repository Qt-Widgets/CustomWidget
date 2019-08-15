/**
* @file CustomCalendar.h
* @brief PopupList
* @author nzx
* @date 2018-9-1 11:43:40
*/
#pragma once

#include <QWidget>
#include <QDateTime>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_CALENDAR_API Q_DECL_EXPORT
#else
#define XW_CALENDAR_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QLineEdit;
class QPushButton;
class QLabel;
namespace zhst
{
	class CustomMessageInfo;
}
QT_END_NAMESPACE

namespace zhst
{
	class XW_CALENDAR_API  CustomCalendar : public QWidget
	{
		Q_OBJECT
	public:
		explicit CustomCalendar(QWidget *parent = 0);
		~CustomCalendar();
		void setType(bool);//false开始时间，true结束时间
		void setCurrentDateTime(QDateTime);
		void setLimitTime(QDateTime);
	signals:
		void submitDateTime(QDateTime, bool);
	private slots:
		void handlesubmitButtonClicked();
		void handlecurrentPageChanged(int,int);
	private:
		QWidget *_mainWidget;
		QCalendarWidget *_calendarWidget;
		QWidget *_timeWidget;
		QLabel *_timeNameLabel;
		QLineEdit *_hourLineEdit;
		QLabel *_splithourLabel;
		QLineEdit *_minuteLineEdit;
		QLabel *_splitminuteLabel;
		QLineEdit *_secondLineEdit;


		QPushButton *_submitButton;

		//zhst::CustomMessageInfo *_messageInfo;

		QDateTime _dateTime;
		QDateTime _limitTime;
		bool _typeFlag = false;//false开始时间，true结束时间
		bool _searchFlag = false;//true为高级检索

		void createWidgets();
		void createLayouts();

	};
}
