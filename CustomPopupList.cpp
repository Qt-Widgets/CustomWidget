#include "CustomPopupList.h"
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QFile>
#include <Windows.h>
#include <QHBoxLayout>
#include <QIcon>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QScrollBar>
#include "./CustomMenu.h"
namespace zhst
{
	//ListWidgetAction
	ListWidgetAction::ListWidgetAction(QWidget* pParent)
		: QWidgetAction(pParent)
	{
		mpListWidget = new QListWidget(pParent);
		mpListWidget->setObjectName("ListWidgetAction");
		mpListWidget->setStyleSheet("QListWidget#ListWidgetAction{ outline:0px; font-size: 15px;font-family: \"Microsoft YaHei\";} \
									QListWidget#ListWidgetAction::item{ height:34px;} \
									QListWidget::Item:hover{background:rgb(233,233,233);} \
									QListWidget::item:selected{background:rgb(233,233,233); color:black;} \
									");
		mpListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		connect(mpListWidget, &QListWidget::itemSelectionChanged, this, &ListWidgetAction::OnSelectChange);
		connect(mpListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {emit itemClicked(); });
		setDefaultWidget(mpListWidget);
		mpListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}
	QString ListWidgetAction::getItemText(int index)
	{
		if (index < mpListWidget->count())
			return mpListWidget->item(index)->text();
		else
			return mpListWidget->item(0)->text();
	}
	void ListWidgetAction::OnSelectChange()
	{
		if (mpListWidget->selectedItems().count() > 0)
		{
			for (int i = 0; i < mpListWidget->count(); i++)
			{
				if (mpListWidget->selectedItems().at(0) == mpListWidget->item(i))
					curIndex = i;
			}
			QString strText = mpListWidget->selectedItems().at(0)->text();
			//qDebug() << "OnSelectChange";
			emit selectChangeSignal(strText, curIndex);
			//if (NULL != menu())
			//{
			//	menu()->hide();
			//}
		}
	}
	void ListWidgetAction::addStringItem(QString text)
	{
		QListWidgetItem *item = new QListWidgetItem(space + text);
		item->setToolTip(text);
		this->setItemTextAlign(item);
		mpListWidget->addItem(item);

	}
	void ListWidgetAction::setMaxItemNumber(int num)
	{
		mpListWidget->setFixedHeight(num * 35);
	}
	void ListWidgetAction::setItemTextAlign(QListWidgetItem * i)
	{
		i->setTextAlignment(align);
	}
	void ListWidgetAction::setItemTextAlignPara(Qt::Alignment a, int num)
	{
		align = a;
		for (int i = 0; i < num; i++)
		{
			space += " ";
		}
	}
	int ListWidgetAction::getClickedIndex()
	{
		return curIndex;
	}
	void ListWidgetAction::setListWidgetWidth(int width)
	{
		mpListWidget->setFixedWidth(width);
	}
	void ListWidgetAction::clearLists()
	{
		mpListWidget->clear();
	}
	void ListWidgetAction::setCurrentIndex(int num , bool isEmitSingel)
	{
		//qDebug() << "setCurrentIndexNum"<< mpListWidget->count() << num;
		if (mpListWidget->count()>num)
		{
			mpListWidget->setCurrentRow(num);
			if (isEmitSingel)
				emit selectChangeSignal(mpListWidget->item(num)->text(), curIndex);
		}
	}
	int ListWidgetAction::getCurrentRow()
	{
		return mpListWidget->currentRow();
	}
	int ListWidgetAction::getCurrentIndex()
	{
		int index = -1;
		index = mpListWidget->currentRow();
		return index;
	}
	//CustomPopupList
	CustomPopupList::CustomPopupList(QWidget *parent)
		:QWidget(parent)
	{
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/PopupList.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		init();
	}
	CustomPopupList::~CustomPopupList()
	{
	}
	void CustomPopupList::init()
	{
		_mainWidget = new QWidget(this);
		_mainWidget->setObjectName("customPopupList");
		_mainWidget->setFixedHeight(34);
		_headLabel = new QLabel(this);
		_headLabel->setObjectName("headLabel");

		_contentLabel = new QLabel(this);
		_contentLabel->setObjectName("contentLabel");
		_contentLabel->setFixedWidth(100);
		QFont font("Microsoft YaHei");
		font.setPixelSize(15);
		_contentLabel->setFont(font);

		_dropButton = new QPushButton(this);
		_dropButton->setFocusPolicy(Qt::NoFocus);
		_dropButton->setObjectName("dropButton");
		_dropButton->setIcon(QIcon(":/CustomWidget/Resources/dropdown.png"));
		_dropButton->setIconSize(QSize(34, 34));

		spaceLabel = new QLabel;
		spaceLabel->setPixmap(QPixmap(":/CustomWidget/Resources/space.png"));

		_allButton = new QPushButton(this);
		_allButton->setObjectName("allButton");
		_allButton->setText(QString(u8"全部"));

		_mainHLayout = new QHBoxLayout(this);
		_mainHLayout->setContentsMargins(10,0,10,0);
		_mainHLayout->addWidget(_headLabel);
		_mainHLayout->addWidget(_contentLabel);
		_mainHLayout->addWidget(_dropButton);
		_mainHLayout->addWidget(spaceLabel);
		_mainHLayout->addWidget(_allButton);
		_mainWidget->setLayout(_mainHLayout);
		
		QHBoxLayout *hLayout = new QHBoxLayout(this);
		hLayout->setMargin(0);
		hLayout->addWidget(_mainWidget);
		this->setLayout(hLayout);
		
		_contentWidget = new CustomMenu;
		mpActionList = new ListWidgetAction();
		connect(mpActionList, &ListWidgetAction::selectChangeSignal, this, &CustomPopupList::OnSelectChange);
		connect(mpActionList, &ListWidgetAction::itemClicked, this, [this]() {_contentWidget->hide(); });
		
		_contentWidget->addAction(mpActionList);

		_contentWidget->installEventFilter(this);
		_contentWidget->hide();

		connect(_dropButton, &QPushButton::clicked, this, &CustomPopupList::dropButtonClicked);
		connect(_allButton, &QPushButton::clicked, this, &CustomPopupList::allButtonClicked);

		setMaxItemNumber(1);
	}
	void CustomPopupList::dropButtonClicked()
	{
		if (!islistshow)
		{
			_contentWidget->setFixedWidth(this->width()+2);
			mpActionList->setListWidgetWidth(this->width());
			if (!isUp)
			{
				//qDebug() << _headLabel->pos() << mapToGlobal(_headLabel->pos()) << QCursor::pos();
				_contentWidget->move(mapToGlobal(_headLabel->pos()) + QPoint(-11, _headLabel->height()+10));
			}
			else 
			{
				//qDebug() << _headLabel->pos()<< mapToGlobal(_headLabel->pos()) << QCursor::pos();
				_contentWidget->move(mapToGlobal(_headLabel->pos()) + QPoint(-11, -_contentWidget->height()-10));
			}
			_contentWidget->show();
			islistshow = true;
		}
		else
		{
			_contentWidget->hide();
			islistshow = false;
		}
	}
	void CustomPopupList::allButtonClicked()
	{
		updateContentLabel(QString(u8"显示全部"));
		emit allButtonClickedSignal();
	}
	void CustomPopupList::OnSelectChange(QString strText,int index)
	{
		//qInfo() << strText << index;
		updateContentLabel(strText);
		_contentWidget->hide();
		emit itemClicked(index);
		invokeVirtualFun(index);
	}
	void CustomPopupList::mousePressEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton &&
			(QApplication::widgetAt(QCursor::pos()) == _headLabel||
				QApplication::widgetAt(QCursor::pos()) == _contentLabel))
		{
			//show listview
			dropButtonClicked();
		}
	}
	bool CustomPopupList::eventFilter(QObject *watched, QEvent *event)
	{
		if (watched == _contentWidget && event->type() == QEvent::Hide && QApplication::widgetAt(QCursor::pos()) != _dropButton && QApplication::widgetAt(QCursor::pos()) != _headLabel && QApplication::widgetAt(QCursor::pos()) != _contentLabel)
		{
			islistshow = false;
		}
		else if (watched == _contentWidget && event->type() == QEvent::Show && QApplication::widgetAt(QCursor::pos()) != _dropButton && QApplication::widgetAt(QCursor::pos()) != _headLabel && QApplication::widgetAt(QCursor::pos()) != _contentLabel)
		{
			islistshow = true;
		}
		else {
		}
		return QWidget::eventFilter(watched, event);     //将事件传递给父类
	}
	void CustomPopupList::setHeadText(QString text)
	{
		_headLabel->setText(text);
	}
	void CustomPopupList::setContentWidth(int width)
	{
		_contentLabel->setFixedWidth(width);
	}
	void CustomPopupList::setContentHeight(int height)
	{
		_contentLabel->setFixedHeight(height);
	}
	void CustomPopupList::setMaxItemNumber(int num)
	{
		mpActionList->setMaxItemNumber(num);
		_contentWidget->setFixedHeight(num * 35);
	}
	void CustomPopupList::setListUpwards(bool isup)
	{
		isUp = isup;
		if(isup)
			_dropButton->setIcon(QIcon(":/CustomWidget/Resources/raiseup.png"));
		else
			_dropButton->setIcon(QIcon(":/CustomWidget/Resources/dropdown.png"));
	}
	void CustomPopupList::setAllButtonHidden(bool ishidden)
	{
		if (ishidden)
		{
			spaceLabel->hide();
			_allButton->hide();
			_mainHLayout->setContentsMargins(10, 0, 0, 0);
		}
		else 
		{
			spaceLabel->show();
			_allButton->show();
			_mainHLayout->setContentsMargins(10, 0, 10, 0);
		}
	}
	void CustomPopupList::setItemTextAlignPara(Qt::Alignment a, int num)
	{
		mpActionList->setItemTextAlignPara(a, num);
	}
	void CustomPopupList::addStringItem(QString text)
	{
		mpActionList->addStringItem(text);
		if (_isNewFlag)
		{
			//setCurrentIndex(0);
			_isNewFlag = false;
		}

		if (mpActionList->mpListWidget->count() == 0) {
			setMaxItemNumber(1);
		}
		else if (mpActionList->mpListWidget->count() >= 5) {
			setMaxItemNumber(5);
		}
		else {
			setMaxItemNumber(mpActionList->mpListWidget->count());
		}
	}
	void CustomPopupList::updateContentLabel(QString text)
	{
		QFontMetrics fontMetrics(_contentLabel->font());
		int fontSize = fontMetrics.width(text);//获取之前设置的字符串的像素大小
		QString str = text;
		if (fontSize > _contentLabel->width())
		{
			str = fontMetrics.elidedText(text, Qt::ElideRight, _contentLabel->width());//返回一个带有省略号的字符串
		}
		_contentLabel->setText(str);
		_contentLabel->setToolTip(text);
	}
	void CustomPopupList::clearLists()
	{
		_contentLabel->setText("");
		_contentLabel->setToolTip("");
		mpActionList->clearLists();
		_isNewFlag = true;
	}
	void CustomPopupList::setCurrentIndex(int num ,bool isEmitSingel)
	{
		if (num < 0)
		{
			return;
		}
		mpActionList->setCurrentIndex(num, isEmitSingel);
	}
	int CustomPopupList::getCurrentRow()
	{
		return mpActionList->getCurrentRow();
	}
	void CustomPopupList::setCurrentTextIndex(int num)
	{
		updateContentLabel(mpActionList->getItemText(num));
	}
	class TestItems :public QWidget {
	public:
		TestItems(QWidget* par = nullptr) {
			createWidget();
			createLayOut();
		};
		QString getShowText() { return text->text(); }
		void setShowText(QString s) { 
			QFontMetrics fontWidth(text->font());//得到每个字符的宽度
			QString elideNote = fontWidth.elidedText(s, Qt::ElideRight, 200);//最大宽度150像素
			text->setText(elideNote);
		};
		void setReadHidden(bool b) { img->setHidden(b); };
		void createWidget() {
			img = new QLabel();
			text = new QLabel();
			img->setText(u8"已读");
			text->setText(u8"");
			text->setFixedWidth(200);
			//text->setAlignment(Qt::AlignLeft);
			text->setObjectName("MyText");
			text->setStyleSheet("#MyText{width:160px;text-overflow:ellipsis;}");
			img->setFixedSize(QSize(35, 18));
			img->setObjectName("frame");
			img->setStyleSheet("#frame{border:2px solid #FC7E5D;border-radius: 5px;color:#FC7E5D;}");
			this->setToolTip(text->text());
			//img->setAlignment(Qt::AlignCenter);
		};
		void createLayOut() {

			QHBoxLayout* v = new QHBoxLayout();
			v->addWidget(text);
			v->addWidget(img);
			v->setSpacing(30);
			v->addStretch();
			this->setLayout(v);
		}
	private:
		QLabel* text;
		QLabel* img;
	};
	void VedioPopupList::addStringItem(QString text)
	{
		if (text == "")
			return;
		QListWidgetItem *item = new QListWidgetItem();
		item->setToolTip(text);
		mpActionList->mpListWidget->addItem(item);
		TestItems* t = new TestItems();
		t->setShowText(text);
		t->setReadHidden(true);
		mpActionList->mpListWidget->setItemWidget(item, t);

		if (mpActionList->mpListWidget->count() == 0) {
			setMaxItemNumber(1);

		}
		else if (mpActionList->mpListWidget->count() >= 5) {
			setMaxItemNumber(5);
		}
		else {
			setMaxItemNumber(mpActionList->mpListWidget->count());
		}
		//mpActionList->setItemTextAlign(new QListWidgetItem(text));
	}
	void VedioPopupList::invokeVirtualFun(int index)
	{
		auto item = mpActionList->mpListWidget->item(index);
		TestItems* text = (TestItems*)mpActionList->mpListWidget->itemWidget(item);
		QString qText = text->getShowText();
		text->setReadHidden(false);
		updateContentLabel(text->getShowText());
	}
	void VedioPopupList::invokeVirtualFunSetString(QString s)
	{

	}
}
