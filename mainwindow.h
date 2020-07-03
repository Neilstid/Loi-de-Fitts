#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveSettings (const QString &key, const QVariant &value, const QString &group);
    QString loadSettings(const QString &key, const QVariant &defaultValue, const QString &group);
    void setSettings();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Auto_seed_stateChanged(int arg1);


    void on_Quitter_clicked();

    void on_actionQuitter_triggered();

    void on_actionReunitialiser_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
