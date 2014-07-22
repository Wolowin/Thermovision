#ifndef DRAWABLEQLABEL_H
#define DRAWABLEQLABEL_H

#include <QLabel>
#include <QImage>
class DrawableQLabel : public QLabel
{
	Q_OBJECT
public:
	explicit DrawableQLabel(QImage image_to_show, QWidget *parent = 0);

	QRect get_selection_rectangle();
	QImage get_calibration_picture();
	void put_picture_in_label(QImage image_to_show);
	void set_starting_selection_rectangle(QRect rectangle_to_set);
public slots:

signals:

protected:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);



private:
	bool selectionStarted;
	QRect selectionRect;
	QImage calibration_image;
};

#endif // DRAWABLEQLABEL_H
