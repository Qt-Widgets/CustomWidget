#include <qdialog.h>
#include <qlabel.h>
#include <qwidget.h>
//#include <qstring.h>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_GLOBALTOOLTIP_API Q_DECL_EXPORT
#else
#define XW_GLOBALTOOLTIP_API Q_DECL_IMPORT
#endif	//XW_GLOBALTOOLTIP_API

class QPixmap;
class QPropertyAnimation;
class QParallelAnimationGroup;
// size 300 56
class XW_GLOBALTOOLTIP_API GlobalToolTip:public QWidget
{
private:
	//five shit
	GlobalToolTip(QWidget* parent=nullptr);
	GlobalToolTip(const GlobalToolTip&) = delete;
	GlobalToolTip& operator = (const GlobalToolTip&) = delete;
	GlobalToolTip (GlobalToolTip&&) = delete;
	GlobalToolTip& operator =(GlobalToolTip&&) = delete;
	~GlobalToolTip()=default;
	//api 
public:
	//0 normal 1 success 2 fail
	static void popup(const QString& text,int type=0);
protected:
	void paintEvent(QPaintEvent *event)override;

private:
	void setText(const QString& text);
	void setType(int type);
	void updatePos(QWidget* parent);
	static GlobalToolTip* instance(QWidget* parent);
private:
	QString contentText;
	QColor contentColor;
	int contentType{ 0 };
	QPropertyAnimation* moveOutAnimation;
	QPropertyAnimation* opacityAnimation;
	QParallelAnimationGroup* animationGroup;
};
