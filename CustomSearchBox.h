/**
* @file CustomSearchBox.h
* @brief CustomSearchBox
* @author nzx
* @date 2018-9-1 10:48:53
*/
#pragma once

#include <QWidget>
#include <QJsonArray>
#include <QIcon>
#include <QMutex>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_SEARCHBOX_API Q_DECL_EXPORT
#else
#define XW_SEARCHBOX_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QCompleter;
class QTreeWidget;
class QTimer;
class QLineEdit;
QT_END_NAMESPACE

namespace zhst
{
	class XW_SEARCHBOX_API  CustomSearchBox : public QWidget
	{
		Q_OBJECT
	public:
		explicit CustomSearchBox(QWidget *parent = 0);
		~CustomSearchBox();
		void init();
		void setPlaceholderText(QString& text);
		void addListItems(QStringList list,QIcon ico = QIcon());
		void addListItems(const QJsonArray& array, const QIcon& ico = QIcon(),int type = -1);
		void popupClear();
		void clearText();
		void showPopup();
		QString text();
		QLineEdit *editor;
	private:
		QTreeWidget *popup;
		QMutex viewMutex;
		bool isReload = true;
	signals:
		void textEdited(QString& s);
		void selectItem(int id, int type);
		void editorClicked();
	public slots:
		void doneCompletion();
	protected:
		virtual bool eventFilter(QObject *obj, QEvent *ev) override;
	};
}