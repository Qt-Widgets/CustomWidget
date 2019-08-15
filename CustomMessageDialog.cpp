#include "CustomMessageDialog.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
CustomMessageDialog::CustomMessageDialog(QWidget* parent):
	QDialog(parent),
	yesBtn{new QPushButton{this}},
	noBtn{new QPushButton{this}},
	contentLabel{new QLabel{this}}
{
	//attribute 
	setAutoFillBackground(false);
	//setWindowOpacity(0.7);
	setWindowFlag(Qt::FramelessWindowHint); 
	setAttribute(Qt::WA_TranslucentBackground);
	//widget
	contentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	contentLabel->setAlignment(Qt::AlignCenter);
	resize(460, 276);
	auto btnLayout = new QHBoxLayout{};
	btnLayout->setSpacing(0);
	btnLayout->addWidget(yesBtn);
	btnLayout->addWidget(noBtn);
	btnLayout->setContentsMargins(0, 0, 0, 10);
	auto mainLayout = new QVBoxLayout{ this };
	mainLayout->setMargin(10);
	mainLayout->addWidget(contentLabel);
	mainLayout->addLayout(btnLayout);
	connect(yesBtn, &QPushButton::clicked, this, &CustomMessageDialog::accept);
	connect(noBtn, &QPushButton::clicked, this, &CustomMessageDialog::reject);
	//btn
	yesBtn->setFixedHeight(46);
	noBtn->setFixedHeight(46);
	yesBtn->setWindowOpacity(0);
	noBtn->setWindowOpacity(0);
	//style
	QFile css{ ":/CustomWidget/Resources/dlg.css" };
	css.open(QFile::ReadOnly);
	setStyleSheet(css.readAll());
	css.close();
	yesBtn->setDefault(true);
}

int CustomMessageDialog::showDlg(const QString& content, DlgType type)
{
	CustomMessageDialog dlg{};
	dlg.setType(type);
	dlg.setContent(content);
	return dlg.exec();
}

void CustomMessageDialog::paintEvent(QPaintEvent * event)
{
	QPainter painter{ this };
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QColor{ 0, 0, 0, 200 });
	painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
}

void CustomMessageDialog::showEvent(QShowEvent * event)
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
    QSize diffSize = _rootWindow->size() - this->size();
    QPoint aim = _rootWindow->pos() + QPoint(diffSize.width(), diffSize.height()) / 2;
	int curScreenNum = QApplication::desktop()->screenNumber(QCursor::pos());
	QRect curScreenRect = QApplication::desktop()->screenGeometry(curScreenNum);
	if (!curScreenRect.contains(aim))
	{
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
	}
	this->move(aim);
}

void CustomMessageDialog::setType(DlgType type)
{
	switch (type)
	{
	case DlgType::OK_CANCLE:
		yesBtn->setText(QString(u8"确定"));
		noBtn->setText(QString(u8"取消"));
		break;
	case DlgType::YES_NO:
		yesBtn->setText(QString(u8"是"));
		noBtn->setText(QString(u8"否"));
		break;
	case DlgType::OK_ONLY:
		noBtn->hide();
		yesBtn->setText(QString(u8"确定"));
	}
}

void CustomMessageDialog::setContent(const QString& text)
{
	contentLabel->setText(text);
}
