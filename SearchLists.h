#pragma once

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QListWidget;
class QCheckBox;
class QPushButton;
class QLabel;
class QListWidgetItem;
QT_END_NAMESPACE

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_SEARCHLISTS_API Q_DECL_EXPORT
#else
#define XW_SEARCHLISTS_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

class XW_SEARCHLISTS_API SearchLists : public QDialog
{
	Q_OBJECT
public:
	SearchLists(QWidget* parent = 0);
	~SearchLists();
	void setSearchNumFlag(const bool&);
	void createTaskListWidget(const std::vector<QString>& string);
	void submitTaskList(const QList<int>&);
    static std::vector<int> getTaskList(std::vector<QString> strLists, std::vector<int> selectIndexList = {});
    void setChecked(std::vector<int> checkedList);
public slots:
	void clearList();
private slots:
	void handlesubmitButtonClicked();
	void handlevideoListWidgetitemClicked(QListWidgetItem* item);
	void handleselectAllCheckBoxClicked();
	void handlecancelAllCheckBoxClicked();
	void handleStateChanged(int checkState);
    void handleClose();
signals:
	void submitTaskLists(const QList<int>&);
	void submitEmptyTaskLists();
protected:
    void closeEvent(QCloseEvent *) override; 
private:
	QWidget* _mainWidget;
	QWidget* _titleWidget;
	QLabel* _titleLabel;
	QPushButton* _closeButton;
	QWidget* _toolWidget;
	QPushButton* _selectAllButton;
	QPushButton* _unselectAllButton;
	QLabel* _hidenLabel;
	QPushButton* _submitButton;
	QListWidget* _videoListWidget;
    std::vector<int> _selectIndexList;
	QList<int> _idLists;
	QList<QString> _nameLists;
	bool _mousePressedFlag = false;//按钮按下
	QPoint _mousePoint;
	bool _SearchNumFlag = true;

    std::vector<int> defaultCheckedList;

	void createWidgets();
	void createLayouts();
	bool eventFilter(QObject* obj, QEvent* event);

	std::vector<int> _selectIndex;

protected:
	void showEvent(QShowEvent* event);
};
