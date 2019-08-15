/**
* @file CustomDialog.h
* @brief Dialog
* @author nzx
* @date 2018-8-27 15:02:43
*/
#pragma once

#include <QDialog>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_DIALOG_API Q_DECL_EXPORT
#else
#define XW_DIALOG_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QMainWindow;
QT_END_NAMESPACE

namespace zhst
{
	class XW_DIALOG_API  CustomDialog : public QDialog
	{
		Q_OBJECT
	public:
		explicit CustomDialog(QWidget *parent = 0);
		~CustomDialog();
		void setTitle(QString title);
		void setMaximizationButtonIn(bool isin = false);
		void addToolButton(QWidget *btn, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
		void addToolLayout(QLayout *layout, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
		void addContentWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
		void addContentLayout(QLayout *layout, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
		void setType(bool type);//true-with shadow,false-without shadow
		void setModalType(bool type);
		void setRealClose(bool isReal);
		void setContentWidgetMargin(int);
		void setToolWidgetMargin(int,int,int,int);
	private:
		QHBoxLayout *_titleLayout;
		QVBoxLayout *_mainLayout;
		QHBoxLayout *_toolsLayout;

		QWidget *_titleWidget;
		QLabel *_titleLabel;
		QPushButton *_titleCloseButton;
		QPushButton *_titleMaxButton;
		QPushButton *_titleMinButton;

        QMainWindow* _rootWindow{ nullptr };
        QWidget* _shadowWidget;

		bool _windowMaxedFlag = false;
		bool isTitleWidgetPress = false;
		QPoint startPos;


		void init();
		void handlezoomButtonClicked();
		void handleminButtonClicked();
		void handleCloseButtonClicked();

		bool shadow = true;
		bool isRealClose = false;
	signals:
		void closeDialog();

	public slots :

	protected:
		virtual void showEvent(QShowEvent *event);
		virtual void hideEvent(QHideEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent* event);
	};
}