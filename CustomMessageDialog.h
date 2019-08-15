#pragma once


#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_NEWDIALOG_API Q_DECL_EXPORT
#else
#define XW_NEWDIALOG_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS
#include <QDialog>
class QLabel;
class QPushButton;
class QMainWindow;
class XW_NEWDIALOG_API CustomMessageDialog:public QDialog
{
	Q_OBJECT
public:
	enum DlgType
	{
		YES_NO,OK_CANCLE,OK_ONLY
	};
public:
	CustomMessageDialog(QWidget* parent = nullptr);
	CustomMessageDialog(const CustomMessageDialog&) = delete;
	static int showDlg(const QString& content, DlgType type);
protected:
	void paintEvent(QPaintEvent* event) override;
	void showEvent(QShowEvent* event) override;
private:
	void setType(DlgType type);
	void setContent(const QString& text);
	QPushButton* yesBtn;
	QPushButton* noBtn;
	QLabel* contentLabel;

    QMainWindow* _rootWindow{ nullptr };

};

