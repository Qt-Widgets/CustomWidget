#include "SearchLists.h"
#include <QLabel>
#include <QListWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include "../CustomWidget/CustomMessageBox.h"
#include "QApplication"
#include "QDesktopWidget"
#include "GlobalTooltip.h"

SearchLists::SearchLists(QWidget* parent)
	: QDialog(parent)
{
	createWidgets();
	createLayouts();
}

SearchLists::~SearchLists()
{
}
void SearchLists::createWidgets()
{
    QFile qss(":/VideoAnalysis/VideoAnalysis.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());

	setWindowFlags(Qt::FramelessWindowHint);
	setWindowModality(Qt::ApplicationModal);
	this->setFixedWidth(800);
	this->setFixedHeight(450);
	_mainWidget = new QWidget();
	_mainWidget->setObjectName("SearchListsWidget");
	_titleWidget = new QWidget();
	_titleWidget->setObjectName("SearchListstitleWidget");
	_titleLabel = new QLabel(this);
	_titleLabel->setText(QString(u8"请选择视频"));
	_titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	_titleLabel->setObjectName("SearchListstitleLabel");
	_titleLabel->installEventFilter(this);
	_closeButton = new QPushButton();
	_closeButton->setObjectName("SearchListsCloseButton");
	connect(_closeButton, &QPushButton::clicked, this, &SearchLists::handleClose);
	_selectAllButton = new QPushButton();
	_selectAllButton->setText(QString(u8"全部选择"));
	_selectAllButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	_selectAllButton->setObjectName("SearchListsSubmitButton");
	_unselectAllButton = new QPushButton();
	_unselectAllButton->setText(QString(u8"全部取消"));
	_unselectAllButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	_unselectAllButton->setObjectName("SearchListsSubmitButton");

	_hidenLabel = new QLabel();
	_hidenLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	connect(_selectAllButton, &QCheckBox::clicked, this, &SearchLists::handleselectAllCheckBoxClicked);
	connect(_unselectAllButton, &QCheckBox::clicked, this, &SearchLists::handlecancelAllCheckBoxClicked);

	_submitButton = new QPushButton();
	_submitButton->setText(QString(u8"确定"));
	_submitButton->setObjectName("SearchListsSubmitButton");
	connect(_submitButton, &QPushButton::clicked, this, &SearchLists::handlesubmitButtonClicked);

	_videoListWidget = new QListWidget();
	_videoListWidget->setObjectName("SearchListsListWidget");
	_videoListWidget->setFocusPolicy(Qt::NoFocus);
	_videoListWidget->setAlternatingRowColors(true);

	connect(_videoListWidget, &QListWidget::itemClicked, this, &SearchLists::handlevideoListWidgetitemClicked);
	_toolWidget = new QWidget();
	_toolWidget->setObjectName("SearchListsToolWidget");
}
void SearchLists::createLayouts()
{
	QHBoxLayout* titleHLayout = new QHBoxLayout();
	titleHLayout->setSpacing(5);
	titleHLayout->setContentsMargins(10, 15, 10, 15);
	titleHLayout->addWidget(_titleLabel);
	titleHLayout->addWidget(_closeButton);
	_titleWidget->setLayout(titleHLayout);

	QHBoxLayout* toolHLayout = new QHBoxLayout();
	toolHLayout->setSpacing(20);
	toolHLayout->setContentsMargins(0, 10, 0, 0);
	toolHLayout->addWidget(_selectAllButton);
	toolHLayout->addWidget(_unselectAllButton);
	toolHLayout->addWidget(_hidenLabel);
	toolHLayout->addWidget(_submitButton);
	_toolWidget->setLayout(toolHLayout);

	QVBoxLayout* contentVLayout = new QVBoxLayout();
	contentVLayout->setSpacing(0);
	contentVLayout->setContentsMargins(10, 10, 10, 10);
	contentVLayout->addWidget(_videoListWidget);
	contentVLayout->addWidget(_toolWidget);

	QVBoxLayout* mainVLayout = new QVBoxLayout();
	mainVLayout->setSpacing(0);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->addWidget(_titleWidget);
	mainVLayout->addLayout(contentVLayout);
	_mainWidget->setLayout(mainVLayout);

	QHBoxLayout* mainHLayout = new QHBoxLayout();
	mainHLayout->setSpacing(0);
	mainHLayout->setContentsMargins(0, 0, 0, 0);
	mainHLayout->addWidget(_mainWidget);
	this->setLayout(mainHLayout);
}
//事件过滤器
bool SearchLists::eventFilter(QObject* obj, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (event->type() == QEvent::MouseButtonPress && obj == _titleLabel && e->buttons().testFlag(Qt::LeftButton) && _mousePressedFlag == false) {

		_mousePressedFlag = true;
		_mousePoint = e->globalPos() - this->pos();
		e->accept();
		return true;
	}
	if (event->type() == QEvent::MouseMove && obj == _titleLabel && e->buttons().testFlag(Qt::LeftButton) && _mousePressedFlag == true) {

		this->move(e->globalPos() - _mousePoint);
		e->accept();
		return true;
	}
	if (event->type() == QEvent::MouseButtonRelease) {
		_mousePressedFlag = false;
		return true;
	}
	return QObject::eventFilter(obj, event);
}
void SearchLists::showEvent(QShowEvent * event)
{
	move(
		(QApplication::desktop()->width() - this->width()) / 2,
		(QApplication::desktop()->height() - this->height()) / 2);

}
void SearchLists::handlesubmitButtonClicked()
{
	QList<int> taskidlist;
	int listnum = _videoListWidget->count();
	for (int i = 0; i < listnum; i++)
	{
		auto itemcheck = qobject_cast<QCheckBox*>(_videoListWidget->itemWidget(_videoListWidget->item(i)));
		if (itemcheck->checkState() == Qt::Checked)
		{
			taskidlist << i;
		}
	}
	if (taskidlist.size() == 0)
	{
		GlobalToolTip::popup(QString(u8"请选择需要检索的视频！"), false);
	}
	/*else if (taskidlist.size() > 20&& _SearchNumFlag)
	{
		zhst::CustomMessageInfo::warning(this, QString(u8"暂不支持过多的视频进行检索！"));
	}*/
	else
	{
		emit submitTaskLists(taskidlist);
        _selectIndexList = taskidlist.toVector().toStdVector();
		this->close();
	}
}

void SearchLists::closeEvent(QCloseEvent *)
{
    
}
void SearchLists::submitTaskList(const QList<int> & taskidlist)
{
	emit submitTaskLists(taskidlist);
}

void SearchLists::handlevideoListWidgetitemClicked(QListWidgetItem * item)
{
	auto itemcheck = qobject_cast<QCheckBox*>(_videoListWidget->itemWidget(item));
	if (itemcheck->checkState() == Qt::Checked)
	{
		itemcheck->setCheckState(Qt::Unchecked);
	}
	else
	{
		itemcheck->setCheckState(Qt::Checked);
	}
}
void SearchLists::setSearchNumFlag(const bool& type)
{
	_SearchNumFlag = type;
}
void SearchLists::createTaskListWidget(const std::vector<QString>& namelists)
{
	//int oldVideoListWidgetSize = _videoListWidget->count();
    _videoListWidget->clear();
	for (int i = 0; i < namelists.size(); i++)
	{
		//create new item if the new list items is more than the old ones.
		/*if (i >= oldVideoListWidgetSize)*/
		//{
			QListWidgetItem* item = new QListWidgetItem();
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			_videoListWidget->addItem(item);
			QCheckBox * checkbox = new QCheckBox();
			checkbox->setObjectName("SearchListsCheckBox");
			checkbox->setCheckState(Qt::Unchecked);
            checkbox->setText(namelists[i]);
			_videoListWidget->setItemWidget(item, checkbox);
			connect(checkbox, &QCheckBox::stateChanged, this, &SearchLists::handleStateChanged);
		//}
		//set item name, set whether checked
		//QWidget* pWgt = _videoListWidget->itemWidget(_videoListWidget->item(i));
		//QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(pWgt);
		//pCheckBox->setText(namelists[i]);
		//pCheckBox->setCheckState(Qt::Unchecked);

        for (auto ite : defaultCheckedList)
        {
            if (i == ite) checkbox->setCheckState(Qt::Checked);
        }
		//QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(pWgt);
		//if (pCheckBox)
		//{
		//	if (idlists[i] == taskID)
		//	{
		//		pCheckBox->setCheckState(Qt::Checked);
		//		checkedNum = i;
		//	}
		//	else
		//		pCheckBox->setCheckState(Qt::Unchecked);
		//	
		//}
		//else
		//{
		//	qCritical() << "createTaskListWidget(): the pCheckBox is null. i=" << i << "; namelists[i]:" << namelists[i];
		//}

	}

	this->show();

    if (defaultCheckedList.size() > 0)
    {
        _videoListWidget->scrollToItem(_videoListWidget->item(defaultCheckedList[0]));
    }
}
void SearchLists::handleStateChanged(int checkState)
{
	if (checkState > 0)
	{
		_submitButton->setEnabled(true);
		_submitButton->setStyleSheet("background-color:#0694CE;");
	}
	else
	{
		bool check = false;
		int listnum = _videoListWidget->count();
		for (int i = 0; i < listnum; i++)
		{
			auto itemcheck = qobject_cast<QCheckBox*>(_videoListWidget->itemWidget(_videoListWidget->item(i)));
			if (itemcheck->checkState() == Qt::Checked)
			{
				check = true;
			}
		}
		if (!check)
		{
			_submitButton->setEnabled(false);
			_submitButton->setStyleSheet("background-color:#C6D7DE;");
		}
	}
}
void SearchLists::handleClose()
{
    _selectIndexList.clear();
    this->close();
}
void SearchLists::handleselectAllCheckBoxClicked()
{
	int listnum = _videoListWidget->count();
	for (int i = 0; i < listnum; i++)
	{
		auto itemcheck = qobject_cast<QCheckBox*>(_videoListWidget->itemWidget(_videoListWidget->item(i))); ;
		itemcheck->setCheckState(Qt::Checked);
	}
}
void SearchLists::handlecancelAllCheckBoxClicked()
{
	int listnum = _videoListWidget->count();
	for (int i = 0; i < listnum; i++)
	{
		auto itemcheck = qobject_cast<QCheckBox*>(_videoListWidget->itemWidget(_videoListWidget->item(i))); ;
		itemcheck->setCheckState(Qt::Unchecked);
	}
}
void SearchLists::clearList()
{
	_videoListWidget->clear();
}

std::vector<int> SearchLists::getTaskList(std::vector<QString> strLists, std::vector<int> selectIndexList) {
	static auto ptr = new SearchLists;
    // ptr->setChecked({ 0 });
    ptr->setChecked(selectIndexList);
    ptr->createTaskListWidget(strLists);
	ptr->exec();
	return ptr->_selectIndexList;
}

void SearchLists::setChecked(std::vector<int> checkedList)
{
    this->defaultCheckedList = checkedList;
}
