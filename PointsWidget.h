#ifndef POINTSWIDGET_H
#define POINTSWIDGET_H

#include <QWidget>

#include <QVector>
#include <QPoint>

#include "NearestNeighborsMatcher.h"

class PointsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PointsWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* paintEvent);

    void mousePressEvent(QMouseEvent* mouseEvent);
    void mouseMoveEvent(QMouseEvent* mouseEvent);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

public slots:
    void setCurrentCategory(int category);
    void setTrainingMode(bool isTraining);
    void setNeighborCount(int neighborCount);
    void generateDiagram();

    void reset();
signals:


private:
    void drawGrid(QPainter& painter);
    void drawTrainingData(QPainter& painter);
    void drawResults(QPainter& painter);


    NearestNeighborsMatcher<QPoint> _matcher;

    int _internalWidth;
    int _internalHeight;

    int _currentCategory;
    bool _trainingMode;

    int _pointRadius;
    QPoint _mouseLocation;
    QPair<int, QPoint>* _hoveredPoint;
    QVector<QPair<int, QPoint>> _points;
    QVector<QPair<int, QPoint>> _classifications;

    QPixmap _imagePixmap;
};

#endif // POINTSWIDGET_H
