#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testingfitts.h"

//Ajout des objet Qt necessaie
#include <QSpinBox>
#include <QDesktopServices>
#include <QSettings>

//Des valeur pour les setting
#define GROUP "MainWindow_Group" //Goupe de setting
#define DEFAULT_VALUE "NONE" //valeur par defaut

#include "data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setSettings(); //met a jour les parametres
}

MainWindow::~MainWindow()
{
    delete ui;
}


//clique bouton demarer
void MainWindow::on_pushButton_clicked()
{
    int seed = 0;
    if(ui->Auto_seed->isChecked() != true){
        seed = ui->seed_value->value();
    }

    //sauvegarde les parametres
    saveSettings("Nb_cibles", ui->Nb_Cibles->value(), GROUP); //Sauvegarde le nombre de cible a afficher
    saveSettings("Taille_max", ui->Taille_max->value(), GROUP); //Sauvegarde la taille max
    saveSettings("Taille_min", ui->Taille_min->value(), GROUP); //Sauvegarde la taille min
    saveSettings("seed", ui->seed_value->value(), GROUP); //valeur du seed
    if(ui->Auto_seed->isChecked()){
        saveSettings("Auto_seed", 1, GROUP);
    }else{
        saveSettings("Auto_seed", 0, GROUP);
    }

    //creation de l'objet pour les données
    //creation de l'objet par constructeur avec le nombre de tour en argument
    Data *data_to = new Data(this->findChild<QSpinBox*>("Nb_Cibles")->value());
    data_to->taille_max = this->findChild<QSpinBox*>("Taille_max")->value();//Taille maximum du rond
    data_to->taille_min = this->findChild<QSpinBox*>("Taille_min")->value();//Taille minimum du rond

    //regard de l'interface evoulu par l'utilisateur
    int interface; //variable pour mettre la valeur
    if(ui->Interface_simple->isChecked()){ //Si l'interface simple et cocher
        interface = 1;
    }else if(ui->Interface_complete->isChecked()){//Si l'interface complete et cocher
        interface = 2;
    }else{ //sinon (signifie qu'il ne souhaite aucune interface)
        interface = 0;
    }

    //contruit l'appli de test
    TestingFitts *t = new TestingFitts(nullptr,data_to, interface, seed);// Argument : parent, srrtucture donnee, interface
    t->showFullScreen();// met en plein ecran
    this->hide(); //cache cette appli
}

//Bouton info
void MainWindow::on_pushButton_2_clicked()
{
    //renvoie vers wiki
    QDesktopServices::openUrl(QUrl("https://en.wikipedia.org/wiki/Fitts%27s_law", QUrl::TolerantMode));
}

//retien les parametre dans qsettings
void MainWindow::saveSettings (const QString &key, const QVariant &value, const QString &group){

    QSettings settings;//creer objet setting
    settings.beginGroup(group);//defini le groupe de parametre
    settings.setValue(key, value);//met la valeur et sa cle pour identifier la valeur
    settings.endGroup();//sort du group

}

QString MainWindow::loadSettings(const QString &key, const QVariant &defaultValue, const QString &group){

    QSettings settings;//creer objet setting
    settings.beginGroup(group);//defini le groupe de parametre
    QVariant value = settings.value(key, defaultValue);//recupere la valeur grace a la cle sinon valeur par defaut
    settings.endGroup();//sort du group

    return value.toString();//retourne la valeur

}

//set the setting save before
void MainWindow::setSettings(){

    //nombre de cible
    int val_nb_cible = loadSettings("Nb_cibles",10, GROUP).toInt();
    ui->Nb_Cibles->setValue(val_nb_cible); //set the value

    //taille min
    int val_taille_min = loadSettings("Taille_min",50, GROUP).toInt();
    ui->Taille_min->setValue(val_taille_min); //set the value

    //taille max
    int val_taillle_max = loadSettings("Taille_max",150, GROUP).toInt();
    ui->Taille_max->setValue(val_taillle_max); //set the value

    //seed
    int val_seed = loadSettings("seed", 0, GROUP).toInt();
    ui->seed_value->setValue(val_seed); //set the value

    //auto seed
    int val_auto_seed = loadSettings("Auto_seed", 1, GROUP).toInt();
    ui->seed_value->setEnabled(!val_auto_seed);
    ui->Auto_seed->setChecked(val_auto_seed); //set the value
}

void MainWindow::on_Auto_seed_stateChanged(int arg1)
{
    if(ui->Auto_seed->isChecked()){
        ui->seed_value->setEnabled(false);
    }else{
        ui->seed_value->setEnabled(true);
    }
}

void MainWindow::on_Quitter_clicked()
{
    QApplication::quit(); //ferme la fenetre
}

void MainWindow::on_actionQuitter_triggered()
{
    QApplication::quit(); //ferme la fenetre
}

void MainWindow::on_actionReunitialiser_triggered()
{
    //nombre de cible
    ui->Nb_Cibles->setValue(10); //set the value

    //taille min
    ui->Taille_min->setValue(50); //set the value

    //taille max
    ui->Taille_max->setValue(150); //set the value

    //seed
    ui->seed_value->setValue(0); //set the value

    //auto seed
    ui->Auto_seed->setChecked(true); //set the value
}
