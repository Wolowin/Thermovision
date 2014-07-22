#include "drawableqlabel.h"

#include <QPainter>
#include <QMouseEvent>


#include <iostream>
using namespace std;

DrawableQLabel::DrawableQLabel(QImage image_to_show, QWidget *parent) :
	QLabel(parent)
{
	put_picture_in_label(image_to_show);
}

void DrawableQLabel::put_picture_in_label(QImage image_to_show)
{
	calibration_image = image_to_show.copy();
	setPixmap(QPixmap::fromImage(calibration_image));
}

void DrawableQLabel::set_starting_selection_rectangle(QRect rectangle_to_set)
{
	selectionRect = rectangle_to_set;
}


QImage DrawableQLabel::get_calibration_picture()
{
	return calibration_image;
}

QRect DrawableQLabel::get_selection_rectangle()
{
	return selectionRect;
}

void DrawableQLabel::paintEvent(QPaintEvent *e)
{
	cout << "painEvent"<< endl;
	QLabel::paintEvent(e);
	QPainter painter(this);
	painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
	painter.setBrush(QBrush(QColor(255,255,255,120)));

	painter.drawRect(selectionRect);
}

void DrawableQLabel::mousePressEvent(QMouseEvent *e)
{
	if (e->button()==Qt::RightButton)
	{
		cout << "Jest prawy"<< endl;
		QImage mniejszy_image = calibration_image.copy(selectionRect);
		setPixmap(QPixmap::fromImage(mniejszy_image));

	}
	else
	{
		cout << "Jest lewy"<< endl;
		selectionStarted=true;
		selectionRect.setTopLeft(e->pos());
		selectionRect.setBottomRight(e->pos());
	}
}

void DrawableQLabel::mouseMoveEvent(QMouseEvent *e)
{
	cout << "Move"<< endl;
	if (selectionStarted)
	{
		selectionRect.setBottomRight(e->pos());
		update();
	}
}

void DrawableQLabel::mouseReleaseEvent(QMouseEvent *e)
{
	cout << "Release"<< endl;
	selectionStarted=false;
}
