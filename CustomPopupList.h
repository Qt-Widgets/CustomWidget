/**
* @file CustomPopupList.h
* @brief PopupList
* @author nzx
* @date 2018-9-1 11:43:40
*/
#pragma once

#include <QWidget>
#include <QWidgetAction>
#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_POPUPLIST_API Q_DECL_EXPORT
#else
#define XW_POPUPLIST_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QLabel;
class QPushButton;
class QComboBox;
class QHBoxLayout;
namespace zhst
{
	class CustomMenu;
}
QT_END_NAMESPACE

namespace zhst
{
	class XW_POPUPLIST_API ListWidgetAction : public QWidgetAction
	{
		Q_OBJECT
	public:
		ListWidgetAction(QWidget* pParent = NULL);


		void OnSelectChange();//在这里添加事件
		void addStringItem(QString text);//添加string
		void setMaxItemNumber(int num);
		void setItemTextAlignPara(Qt::Alignment a, int num);
		int getClickedIndex();
		void setCurrentIndex(int,bool isEmitSingel = false);
		int getCurrentIndex();
		int getCurrentRow();
		
		void setListWidgetWidth(int width);
		QString getItemText(int);
		void clearLists();
		void setItemTextAlign(QListWidgetItem *i);
		QListWidget* mpListWidget;
	signals:
		void selectChangeSignal(QString strText,int index);
		void itemClicked();
	private:
		Qt::Alignment align = Qt::AlignCenter;
		QString space="";
		int curIndex = -1;
	};

	class XW_POPUPLIST_API  CustomPopupList : public QWidget
	{
		Q_OBJECT
	public:
		explicit CustomPopupList(QWidget *parent = 0);
		~CustomPopupList();

		void setHeadText(QString text);
		void setContentWidth(int width);
		void setContentHeight(int height);
		void setMaxItemNumber(int num);
		void setListUpwards(bool isup);
		void setAllButtonHidden(bool ishidden);
		void setItemTextAlignPara(Qt::Alignment a, int num);
		virtual void addStringItem(QString text);
		void setCurrentIndex(int, bool isEmitSingel = false);
		int getCurrentRow();
		//设置当前的index,从0开始
		void setCurrentTextIndex(int);
		virtual void invokeVirtualFun(int index) {};
		virtual void invokeVirtualFunSetString(QString string) {};

		void clearLists();
		ListWidgetAction* mpActionList;
	private:
		void init();

		QWidget *_mainWidget;
		QLabel *_headLabel;
		QLabel *_contentLabel;
		QLabel *spaceLabel;
		QPushButton *_dropButton;
		QPushButton *_allButton;
		CustomMenu *_contentWidget;

		QHBoxLayout *_mainHLayout;
		
		QFont font;

		bool islistshow = false;
		bool isUp = false;
		bool _isNewFlag=true;//标志是否是新

	signals:
		void allButtonClickedSignal();
		void itemClicked(int index);
	public slots :
		void updateContentLabel(QString text);
		void dropButtonClicked();
		void allButtonClicked();
		void OnSelectChange(QString strText,int index);
	protected:
		virtual void mousePressEvent(QMouseEvent *event);
		virtual bool eventFilter(QObject * watched, QEvent * event);
	};

	class XW_POPUPLIST_API VedioPopupList : public CustomPopupList {
	public:
		VedioPopupList(QWidget* par = 0)
			:CustomPopupList(par)
		{

		}
		void addStringItem(QString text);//添加string
		void invokeVirtualFun(int index);
		void invokeVirtualFunSetString(QString s);
	};
}