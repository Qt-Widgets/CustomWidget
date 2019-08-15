/**
* @file CustomSlider.h
* @brief Slider
* @author hpf
* @date 2018-10-10 14:49
*/
#pragma once

#include <QSlider>

#ifdef XW_CUSTOMWIDGET_EXPORTS
#define XW_CUSTOM_SLIDER_API Q_DECL_EXPORT
#else
#define XW_CUSTOM_SLIDER_API Q_DECL_IMPORT
#endif	//XW_CUSTOMWIDGET_EXPORTS

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace zhst
{
	class XW_CUSTOM_SLIDER_API  CustomSider : public QSlider
	{
		Q_OBJECT
	public:
		explicit CustomSider(Qt::Orientation orientation, QWidget *parent = 0);
		~CustomSider();
	protected:
		void mousePressEvent(QMouseEvent *ev);
		//virtual inline void keyPressEvent(QKeyEvent *e) { emit submitValueChange(this->value()); enterEvent(e); };
		virtual inline void keyReleaseEvent(QKeyEvent *e) {QSlider::keyReleaseEvent(e) ;emit submitValueChange(this->value()); };
		virtual inline void wheelEvent(QWheelEvent *e) { QSlider::wheelEvent(e);emit submitValueChange(this->value()); };

	signals:
		void submitValueChange(int);
	};
}
