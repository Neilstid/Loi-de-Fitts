#ifndef RESULT_FITTS_H
#define RESULT_FITTS_H

#include <QWidget>
#include "data.h"

namespace Ui {
class Result_Fitts;
}

class Result_Fitts : public QWidget
{
    Q_OBJECT

public:
    explicit Result_Fitts(QWidget *parent = nullptr, Data *d = nullptr);
    ~Result_Fitts();
    void makeplot();
    void tableData();
    void updatePLot();
    void updateTable();

private slots:
    void on_Value_a_valueChanged(double arg1);

    void on_Value_b_valueChanged(double arg1);

    void on_Extract_clicked();

    void on_pushButton_2_clicked();

    void on_Retry_clicked();


private:
    Ui::Result_Fitts *ui;
};

#endif // RESULT_FITTS_H
