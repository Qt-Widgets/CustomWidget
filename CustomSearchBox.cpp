#include "CustomSearchBox.h"
#include <QFile>
#include <QEvent>
#include <QKeyEvent>
#include <QAction>
#include <QPainter>
#include <QDebug>
#include <QAbstractItemView>
#include <QTreeWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QScrollBar>
#include <QApplication>
#define ITEMHEIGHT 40
#define MAXSHOWITEMNUM 10
namespace zhst
{
	class CustomTreeWidgetItem :public QTreeWidgetItem
	{
	public:
		explicit CustomTreeWidgetItem(QTreeWidget *view, const QStringList &strings):QTreeWidgetItem(view,strings) {}
		~CustomTreeWidgetItem() = default;
		int type = -1;//0-video,1-camera,2-preprocess
		int id = -1;
	};
	CustomSearchBox::CustomSearchBox(QWidget *parent)
		:QWidget(parent)
	{
		this->setAttribute(Qt::WA_StyleSheet);
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/SearchBox.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		init();
	}
	CustomSearchBox::~CustomSearchBox()
	{
		delete popup;
		delete editor;
	}
	void CustomSearchBox::init()
	{
		this->setObjectName("mainWidget");
		
		popup = new QTreeWidget;
		popup->setObjectName("popup");
		popup->setStyleSheet("QTreeWidget#popup{ background:rgb(255,255,255);outline:0px; font-size: 15px;font-family: \"Microsoft YaHei\";} \
							QTreeWidget::item:hover{background:rgb(233,233,233);} \
							QTreeWidget::item:selected{background:rgb(233,233,233); color:black;} \
							");
		popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
		popup->setFocusPolicy(Qt::NoFocus);
		popup->setFocusProxy(this);
		popup->setMouseTracking(true);

		popup->setColumnCount(1);
		popup->setUniformRowHeights(true);
		popup->setRootIsDecorated(false);
		popup->setEditTriggers(QTreeWidget::NoEditTriggers);
		popup->setSelectionBehavior(QTreeWidget::SelectRows);
		popup->setFrameStyle(QFrame::Box | QFrame::Plain);
		popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		popup->setHeaderHidden(true);
        popup->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

		popup->installEventFilter(this);
		connect(popup, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
			SLOT(doneCompletion()));

		editor = new QLineEdit();
		editor->setObjectName("editor");
		editor->setContextMenuPolicy(Qt::NoContextMenu);
		editor->installEventFilter(this);

		QIcon ico(":/CustomWidget/Resources/startSearch.png");
		QAction* searchAction = editor->addAction(ico, QLineEdit::TrailingPosition);
		connect(editor, &QLineEdit::returnPressed, this, [this]()
		{
			emit textEdited(editor->text());
		});
		connect(searchAction, &QAction::triggered, this, [this]()
		{
			emit textEdited(editor->text());
		});
		connect(editor, &QLineEdit::textEdited, [this](QString s)
		{
			if (!editor->isClearButtonEnabled())
			{
				editor->setClearButtonEnabled(true);
				editor->findChild<QAction*>("_q_qlineeditclearaction")->setIcon(QPixmap(":/CustomWidget/Resources/clear.png"));
			}
			emit textEdited(s);
		});
		
		QHBoxLayout *hLayout = new QHBoxLayout;
		hLayout->setMargin(0);
		hLayout->addWidget(editor);
		this->setLayout(hLayout);
	}
	void CustomSearchBox::setPlaceholderText(QString & text)
	{
		editor->setPlaceholderText(text);
	}
	void CustomSearchBox::addListItems(QStringList list, QIcon ico)
	{
		//test方法
		viewMutex.lock();
		if (popup->topLevelItemCount() > 0)
		{
			popup->clear();
		}
		for (auto& s : list)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(popup, QStringList(s));
			item->setSizeHint(0, QSize(0, 60));
			if (!ico.isNull())
			{
				item->setIcon(0, ico);
			}
		}
		popup->setCurrentItem(popup->topLevelItem(0));
		popup->resizeColumnToContents(0);
		popup->setUpdatesEnabled(true);
		popup->move(this->mapToGlobal(QPoint(0, this->height())));
		editor->setFocus();
		popup->setFixedWidth(this->width());
		popup->show();
		viewMutex.unlock();
	}
	void CustomSearchBox::addListItems(const QJsonArray& array, const QIcon& ico,int type)
	{
		isReload = false;
		int i = 0;
		//viewMutex.lock();
		while (!isReload && i<array.size())
		{
            if (i > 100)return;
			QString name = array.at(i).toObject().value("name").toString();
			int id = array.at(i).toObject().value("id").toInt();
			CustomTreeWidgetItem *item = new CustomTreeWidgetItem(popup, QStringList(name));
			item->setSizeHint(0, QSize(0, ITEMHEIGHT));
			item->id = id;
			item->type = type;
			if (!ico.isNull())
			{
				item->setIcon(0, ico);
			}
			i++;
		}
		if (!isReload)
		{
			int current = popup->topLevelItemCount();
			if (current > MAXSHOWITEMNUM)
			{
				popup->setFixedHeight(MAXSHOWITEMNUM * ITEMHEIGHT + 15);
			}
			else
			{
				popup->setFixedHeight(current * ITEMHEIGHT + 15);
			}
			popup->setCurrentItem(popup->topLevelItem(0));
			popup->resizeColumnToContents(0);
			popup->setUpdatesEnabled(true);
			popup->move(this->mapToGlobal(QPoint(0, this->height())));
			editor->setFocus();
			popup->setFixedWidth(this->width());
			popup->show();
		}
		//viewMutex.unlock();
	}
	void CustomSearchBox::popupClear()
	{
		isReload = true;
		//viewMutex.lock();
		popup->clear();
		popup->setFixedHeight(3 * ITEMHEIGHT);
		//viewMutex.unlock();
	}
	void CustomSearchBox::clearText()
	{
		editor->clear();
	}
	void CustomSearchBox::showPopup()
	{
		popup->setCurrentItem(popup->topLevelItem(0));
		popup->resizeColumnToContents(0);
		popup->setUpdatesEnabled(true);
		popup->move(this->mapToGlobal(QPoint(0, this->height())));
		editor->setFocus();
		popup->setFixedWidth(this->width());
		popup->show();
	}
	QString CustomSearchBox::text()
	{
		return editor->text();
	}
	bool CustomSearchBox::eventFilter(QObject * obj, QEvent * ev)
	{
        //if (obj != popup && obj != editor)
        //    return false;

        if (obj == editor && ev->type() == QEvent::KeyPress)
        {
            return false;
        }
        if (obj == editor && ev->type() == QEvent::MouseButtonRelease) {
            emit editorClicked();
            return true;
        }
        if (ev->type() == QEvent::MouseButtonPress) {

            popup->hide();
            editor->setFocus();
            editor->event(ev);
            return true;
        }
        if (ev->type() == QEvent::KeyPress) {

            bool consumed = false;
            int key = static_cast<QKeyEvent*>(ev)->key();
            switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                doneCompletion();
                consumed = true;
                break;

            case Qt::Key_Escape:
                editor->setFocus();
                popup->hide();
                consumed = true;
                break;

            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Home:
            case Qt::Key_End:
            case Qt::Key_PageUp:
            case Qt::Key_PageDown:
                break;

            default:
                /*if(obj!=editor)*/
                editor->setFocus();
                editor->event(ev);
                break;
            }

            return consumed;
        }

        return false;

	}
	void CustomSearchBox::doneCompletion()
	{
		popup->hide();
		editor->setFocus();
		if (popup->selectedItems().size() > 0)
		{
			CustomTreeWidgetItem* item = static_cast<CustomTreeWidgetItem*>(popup->selectedItems()[0]);
			emit selectItem(item->id, item->type);
		}
	}
}

