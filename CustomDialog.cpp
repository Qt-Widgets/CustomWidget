#include "CustomDialog.h"
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QPushButton>
#include <QIcon>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
namespace zhst
{
	CustomDialog::CustomDialog(QWidget *parent)
		:QDialog(parent)
	{
		Q_INIT_RESOURCE(CustomWidget);
		QFile qss(":/CustomWidget/Resources/Dialog.qss");
		qss.open(QFile::ReadOnly);
		this->setStyleSheet(qss.readAll());
		qss.close();
		init();
		setAttribute(Qt::WA_QuitOnClose, false);
	}
	CustomDialog::~CustomDialog()
	{
		delete _titleLayout;
		delete _mainLayout;
		delete _toolsLayout;
        delete _shadowWidget;
	}
	void CustomDialog::init()
	{
		this->setFixedSize(1000, 550);
		this->setObjectName("CustomDialog");
		this->setModal(true);
		this->setWindowFlags(Qt::FramelessWindowHint);

		_titleWidget = new QWidget(this);
		_titleWidget->setObjectName("titleWidget");
		_titleWidget->setFixedHeight(50);
		
		_titleLabel = new QLabel(_titleWidget);
		_titleLabel->setObjectName("titleLabel");
		_titleLabel->setText("Your Title");
		_titleCloseButton = new QPushButton(this);
		_titleCloseButton->setObjectName("closeButton");
		_titleCloseButton->setToolTip(QString(u8"隐藏"));
		_titleCloseButton->setFocusPolicy(Qt::NoFocus);
		QIcon closeIcon;
		closeIcon.addFile(QString(u8":/CustomWidget/Resources/closeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
		_titleCloseButton->setIcon(closeIcon);
		_titleCloseButton->setIconSize(QSize(22, 22));

		_titleMaxButton = new QPushButton(this);
		_titleMaxButton->setObjectName("maxButton");
		_titleMaxButton->setToolTip(QString(u8"最大化"));
		_titleMaxButton->setFocusPolicy(Qt::NoFocus);
		QIcon maxIcon;
		maxIcon.addFile(QString(u8":/CustomWidget/Resources/zoomInButton.png"), QSize(), QIcon::Normal, QIcon::Off);
		_titleMaxButton->setIcon(maxIcon);
		_titleMaxButton->setIconSize(QSize(22, 22));

		_titleMinButton = new QPushButton(this);
		_titleMinButton->setObjectName("minButton");
		_titleMinButton->setToolTip(QString(u8"最小化"));
		_titleMinButton->setFocusPolicy(Qt::NoFocus);
		QIcon minIcon;
		minIcon.addFile(QString(u8":/CustomWidget/Resources/hiddenButton.png"), QSize(), QIcon::Normal, QIcon::Off);
		_titleMinButton->setIcon(minIcon);
		_titleMinButton->setIconSize(QSize(22, 22));

		QHBoxLayout *titleMainLayout = new QHBoxLayout();
		titleMainLayout->setSpacing(20);
		titleMainLayout->setContentsMargins(20, 0, 20, 0);
		titleMainLayout->addWidget(_titleLabel,20,Qt::AlignLeft);
		titleMainLayout->addWidget(_titleMinButton, 0, Qt::AlignRight);
		titleMainLayout->addWidget(_titleMaxButton, 0, Qt::AlignRight);
		titleMainLayout->addWidget(_titleCloseButton, 0, Qt::AlignRight);
		_titleWidget->setLayout(titleMainLayout);

		_titleLayout = new QHBoxLayout();
		_titleLayout->setAlignment(Qt::AlignTop);
		_titleLayout->setMargin(0);
		_titleLayout->addWidget(_titleWidget);

		_mainLayout = new QVBoxLayout();
		_mainLayout->setMargin(11);

		_toolsLayout = new QHBoxLayout();
		_toolsLayout->setContentsMargins(0, 0, 0, 10);

		QVBoxLayout *vLayout = new QVBoxLayout();
		vLayout->setSpacing(0);
		vLayout->setMargin(0);
		vLayout->addLayout(_titleLayout);
		vLayout->addLayout(_mainLayout);
		vLayout->addLayout(_toolsLayout);
		//vLayout->addSpacing(10);
		this->setLayout(vLayout);

		connect(_titleMinButton, &QPushButton::clicked, this, &CustomDialog::handleminButtonClicked);
		connect(_titleMaxButton, &QPushButton::clicked, this, &CustomDialog::handlezoomButtonClicked);
		connect(_titleCloseButton, &QPushButton::clicked, this, &CustomDialog::handleCloseButtonClicked);
		_titleMaxButton->setVisible(false);
		_titleMinButton->setVisible(false);

        //shadow when model type is true
        _shadowWidget = new QWidget;
        _shadowWidget->setWindowFlags(Qt::FramelessWindowHint);
        _shadowWidget->setObjectName("shadowWidget");
        _shadowWidget->setStyleSheet("#shadowWidget {background-color:rgba(0, 0, 0);}");
        _shadowWidget->setWindowOpacity(0.5);
        _shadowWidget->hide();
	}
	void CustomDialog::setTitle(QString title)
	{
		_titleLabel->setText(title);
		this->setWindowTitle(title);
	}
	void CustomDialog::setMaximizationButtonIn(bool isin)
	{
		if (!isin)
		{
			_titleMaxButton->hide();
		}
		else
		{
			_titleMaxButton->show();
		}
	}
	void CustomDialog::addToolButton(QWidget * btn, int stretch, Qt::Alignment alignment)
	{
		_toolsLayout->addWidget(btn, stretch, alignment);
	}
	void CustomDialog::addToolLayout(QLayout * layout, int stretch, Qt::Alignment alignment)
	{
		_toolsLayout->addLayout(layout, stretch);
		_toolsLayout->setAlignment(alignment);
	}
	void CustomDialog::addContentWidget(QWidget * widget, int stretch, Qt::Alignment alignment)
	{
		_mainLayout->addWidget(widget,stretch,alignment);
	}
	void CustomDialog::addContentLayout(QLayout * layout, int stretch, Qt::Alignment alignment)
	{
		_mainLayout->addLayout(layout, stretch);
		layout->setAlignment(alignment);
	}
	void CustomDialog::setType(bool type)
	{
		shadow = type;
		if (type)
		{
			_titleMaxButton->setVisible(false);
			_titleMinButton->setVisible(false);
		}
		else
		{
			_titleMaxButton->setVisible(true);
			_titleMinButton->setVisible(true);
		}
	}
	void CustomDialog::setModalType(bool type)
	{
		this->setModal(type);
		setType(type);

	}
	void CustomDialog::setRealClose(bool isReal)
	{
		isRealClose = isReal;
	}
	void CustomDialog::setContentWidgetMargin(int margin)
	{
		_mainLayout->setContentsMargins(margin,margin,margin,margin);
	}
	void CustomDialog::setToolWidgetMargin(int left, int top, int right, int bottom)
	{
		_toolsLayout->setContentsMargins(left, top, right, bottom);
	}
	void CustomDialog::showEvent(QShowEvent *event)
	{
        if (!_rootWindow)
        {
            QWidgetList topWidgetList = QApplication::topLevelWidgets();
            for (QWidget* w : topWidgetList)
            {
                if (QMainWindow* window = qobject_cast<QMainWindow*>(w))
                {
                    _rootWindow = qobject_cast<QMainWindow*>(w);
                    break;
                }
            }
        }
		if (!_windowMaxedFlag)
		{
            QSize diffSize = _rootWindow->size() - this->size();
			QPoint aim = _rootWindow->pos() + QPoint(diffSize.width(),diffSize.height()) / 2;
			int curScreenNum = QApplication::desktop()->screenNumber(QCursor::pos());
			QRect curScreenRect = QApplication::desktop()->availableGeometry(curScreenNum);
			int x = aim.x();
			int y = aim.y();
			if (x < curScreenRect.x())
			{
				x = curScreenRect.x();
			}
			if (curScreenRect.width() - x < this->width())
			{
				x = curScreenRect.width() - this->width();
			}
			if (y < curScreenRect.y())
			{
				y = curScreenRect.y();
			}
			if (curScreenRect.height() - y < this->height())
			{
				y = curScreenRect.height() - this->height();
			}
			aim = QPoint(x, y);
			this->move(aim);
		}
        if (shadow)
        {
            _shadowWidget->move(_rootWindow->pos());
            _shadowWidget->resize(_rootWindow->size());
            _shadowWidget->show();
        }
	}
	void CustomDialog::hideEvent(QHideEvent * event)
	{
        if (shadow)
        {
            _shadowWidget->hide();
        }
	}
	void CustomDialog::mousePressEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton &&
			QApplication::widgetAt(QCursor::pos()) == _titleWidget)   //titleBar是自定义的标题栏
		{
			startPos = event->globalPos();
			isTitleWidgetPress = true;
		}
	}
	void CustomDialog::mouseReleaseEvent(QMouseEvent * event)
	{
		isTitleWidgetPress = false;
	}
	void CustomDialog::mouseMoveEvent(QMouseEvent * event)
	{
		if (isTitleWidgetPress)
		{
			QPoint curPos;
			if (_windowMaxedFlag)
			{
				int mouseX = event->globalPos().x();
				int diff = 1000 * (((double)mouseX / (double)this->width()));
				int curX = mouseX - diff;
				curPos.setX(curX);
			}
			else {
				curPos = this->pos();
			}
			if(_windowMaxedFlag)
				handlezoomButtonClicked();
			this->move(curPos + event->globalPos() - startPos);
			startPos = event->globalPos();
		}
	}
    void CustomDialog::keyPressEvent(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_Escape:
            handleCloseButtonClicked();
            break;
        default:
            QDialog::keyPressEvent(event);
        }
    }
	void CustomDialog::handlezoomButtonClicked()
	{
		if (_windowMaxedFlag) {
			this->showNormal();
			_titleMaxButton->setIcon(QIcon(":/CustomWidget/Resources/zoomInButton.png"));
			_titleMaxButton->setToolTip(QString(u8"最大化"));
		}
		else {
			this->showMaximized();
			_titleMaxButton->setIcon(QIcon(":/CustomWidget/Resources/zoomOutButton.png"));
			_titleMaxButton->setToolTip(QString(u8"向下还原"));
		}
		_windowMaxedFlag = !_windowMaxedFlag;
	}
	void CustomDialog::handleminButtonClicked()
	{
		this->showMinimized();
	}
	void CustomDialog::handleCloseButtonClicked()
	{
		if (!isRealClose)
		{
			this->hide();
		}
		else
		{
			emit closeDialog();
			//this->close();
		}
	}
}