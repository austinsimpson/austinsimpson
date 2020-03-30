#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCheckBox>

MainWindow::MainWindow
(
	QWidget* parent
):
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->_categorySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->_pointsWidget, &PointsWidget::setCurrentCategory);

    connect(ui->_neighborCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->_pointsWidget, &PointsWidget::setNeighborCount);
    connect(ui->_resetButton, &QPushButton::clicked, ui->_pointsWidget, &PointsWidget::reset);
    connect(ui->_trainButton, &QPushButton::clicked, ui->_pointsWidget, &PointsWidget::generateDiagram);
}

MainWindow::~MainWindow()
{
    delete ui;
}

