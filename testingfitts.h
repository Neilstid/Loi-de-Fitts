#ifndef TESTINGFITTS_H
#define TESTINGFITTS_H


#include "data.h"

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class TestingFitts;
}

class TestingFitts : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestingFitts(QWidget *parent = nullptr, Data *d = nullptr, int interface = 0, int seed = 0);
    ~TestingFitts();
    virtual void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void updateInterface();

private:
    Ui::TestingFitts *ui;
};

#endif // TESTINGFITTS_H
