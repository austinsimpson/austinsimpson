#include "PointsWidget.h"

#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>

const int kGridIncrement = 50;

const QVector<QColor> kColors = {
    Qt::red, Qt::blue, QColor(0, 200, 0)
};

PointsWidget::PointsWidget
(
    QWidget* parent
):
    QWidget(parent),
    _hoveredPoint(nullptr),
    _pointRadius(5),
    _mouseLocation(-1, -1),
    _currentCategory(0),
    _internalWidth(200),
    _internalHeight(200)
{
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);

    _matcher.setDistanceFunction([](const QPoint& first, const QPoint& second) -> qreal
    {
        int xDiff = first.x() - second.x();
        int yDiff = first.y() - second.y();
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    });
}


void PointsWidget::paintEvent
(
    QPaintEvent* paintEvent
)
{
    Q_UNUSED(paintEvent);

    QPainter painter;
    painter.begin(this);
    
    QTransform transform;
    transform.translate(width() / 2, height() / 2);
    transform.scale(1.0, -1.0);
    painter.setTransform(transform);


    painter.setBrush(Qt::lightGray);
    painter.fillRect(0, 0, width(), height(), Qt::white);

    drawResults(painter);
    drawGrid(painter);
    drawTrainingData(painter);

    painter.setPen(QColor(0, 0, 0, 126));
    painter.setBrush(QColor(0, 0, 0, 126));
    painter.drawEllipse(_mouseLocation, _pointRadius, _pointRadius);

    painter.scale(1.0, -1.0);
    painter.drawText(QPoint(0,0), QString("X: %1 Y: %2").arg(QString::number(_mouseLocation.x()), QString::number(_mouseLocation.y())));

    painter.end();
}

void PointsWidget::drawGrid(QPainter& painter)
{
    if (!painter.isActive())
    {
        painter.begin(this);
    }
    
    painter.setPen(Qt::gray);

    for (int i = 0; i < width() / 2; i += kGridIncrement)
    {
        painter.drawLine(i, -height() / 2, i, height() / 2);
        painter.drawLine(-i, -height() / 2, -i, height() / 2);
    }
    
    for (int i = 0; i < height() / 2; i += kGridIncrement)
    {
        painter.drawLine(-width() / 2, i, width() / 2, i);
        painter.drawLine(-width() / 2, -i, width() / 2, -i);
    }

    QPen pen = painter.pen();
    pen.setWidth(3);
    pen.setColor(QColor(Qt::black));
    painter.setPen(pen);

    painter.drawLine(-width() / 2, 0, width() / 2, 0);
    painter.drawLine(0, -height() / 2, 0, height() / 2);
}

void PointsWidget::mousePressEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = mouseEvent->pos() - QPoint(width() / 2, height() / 2);
    _mouseLocation.ry() *= -1;

    if (_currentCategory >= 0)
    {
        if (_trainingMode)
        {
            _matcher.addTrainingData(TrainingDatum<QPoint>(_mouseLocation, _currentCategory));
            _points.push_back(qMakePair(_currentCategory, _mouseLocation));
        }
        else
        {
            //_classifications.push_back(qMakePair(_matcher.classifyDataPoint(_mouseLocation), _mouseLocation));
        }
    }
    update();
}

void PointsWidget::mouseMoveEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = mouseEvent->pos() - QPoint(width() / 2, height() / 2);
    _mouseLocation.ry() *= -1;
    update();
}

void PointsWidget::mouseReleaseEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = mouseEvent->pos() - QPoint(width() / 2, height() / 2);
    _mouseLocation.ry() *= -1;
    update();
}

void PointsWidget::drawTrainingData
(
    QPainter& painter
)
{
    for (auto pointPair : _points)
    {
        painter.setPen(kColors[pointPair.first]);
        painter.setBrush(kColors[pointPair.first]);
        painter.drawEllipse(pointPair.second, _pointRadius, _pointRadius);
    }
}

void PointsWidget::drawResults
(
    QPainter& painter
)
{
    painter.drawPixmap(-_imagePixmap.width() / 2, -_imagePixmap.height() / 2, _imagePixmap.width(), _imagePixmap.height(), _imagePixmap);

    //painter.setPen(kColors[classification.first]);
    //QPoint centerPoint = classification.second;
    //painter.drawPoint(centerPoint);
    //painter.drawLine(centerPoint - QPoint(2, 2), centerPoint + QPoint(2, 2));
}

void PointsWidget::setCurrentCategory
(
    int category
)
{
    _currentCategory = category;
}

void PointsWidget::setTrainingMode
(
    bool isTraining
)
{
    _trainingMode = isTraining;
}

void PointsWidget::setNeighborCount
(
    int neighborCount
)
{
    _matcher.setNeighborCount(neighborCount);
}

void PointsWidget::generateDiagram()
{
    QImage temp(QSize(width(), height()), QImage::Format::Format_RGB888);

    if (_points.count() > _matcher.neighborCount())
    {
        _classifications.clear();

        int halfWidth = width() / 2;
        int halfHeight = height() / 2;

        for (int x = -halfWidth; x < halfWidth; x++)
        {
            for (int y = -halfHeight; y < halfHeight; y++)
            {
                _classifications.push_back(qMakePair(_matcher.classifyDataPoint(QPoint(x, y)), QPoint(x, y)));
                temp.setPixel(QPoint(x + halfWidth, y + halfHeight), kColors[_classifications.last().first].lighter().rgb());
            }
        }
    }
    _imagePixmap = QPixmap::fromImage(temp);
    update();
}

void PointsWidget::reset()
{
    _imagePixmap = QPixmap();
    _matcher.clear();
    _points.clear();
}