#include "result_fitts.h"
#include "ui_result_fitts.h"
#include "mainwindow.h"

//include des objet QT
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QVector>
#include <QTableWidget>
#include <QApplication>
#include <QProcess>

#include <data.h>

//include pour calcul (et ecriture dans fichier)
#include <math.h>
#include <iostream>
#include <algorithm>
#include <fstream>

//La classe pour les données
Data data_final(100);//creer la structure de donné pour recuperer celle transmis par son pere

Result_Fitts::Result_Fitts(QWidget *parent, Data *d) :
    QWidget(parent),
    ui(new Ui::Result_Fitts)
{

    //copie la structure dans la structure globale
    data_final.nb_turn = d->nb_turn; //nombre de tour
    data_final.rayon_cercle = d->rayon_cercle; //rayon du cercle
    data_final.position_mouse_x = d->position_mouse_x; //transmission pointeur position souris x
    data_final.position_mouse_y = d->position_mouse_y; //transmission pointeur position souris y
    data_final.position_target_x = d->position_target_x; //transmission pointeur position cible x
    data_final.position_target_y = d->position_target_y; //transmission pointeur position cible y
    data_final.time_to_target = d->time_to_target; //transmission pointeur temps utilisateur

    ui->setupUi(this);
    makeplot();//creer le graphique
    tableData();//creer la table

    free(d);
}

Result_Fitts::~Result_Fitts()
{
    delete ui;
}

//creer le graphique
void Result_Fitts::makeplot(){

    // show legend with slightly transparent background brush:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));

    // create graph and assign data to it:
    ui->customPlot->addGraph();

    // create graph and assign data to it:
    ui->customPlot->addGraph();

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Essaies");
    ui->customPlot->yAxis->setLabel("Temps (milliseconde)");

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //set the plot
    ui->customPlot->replot();
    ui->customPlot->update();

    updatePLot();//met a jours le graphique avec les données

}

void Result_Fitts::updatePLot(){
    // generate some data:
    int taille = int(data_final.nb_turn);//taille des données stocke

    int valeur[taille];//tableau pour stocker les temps de l'utilisateur
    for(int i = 1; i <= taille; i++){//pour chaque donné (les données commencent a 1)
        valeur[i] = int(data_final.time_to_target[i]); //transmission des données
    }

    int Fitts[taille];//tableau pour loi de fitts
    int distance[taille];//ditance entre le point de depart  et la cible
    int diametre[taille];//diametre du cercle
    //recupere les valeur a et b voulu par l'utilisateur
    double a = ui->Value_a->value();
    double b = ui->Value_b->value();
    for(int i = 1; i <= taille; i++){
        distance[i] = sqrt(pow(data_final.position_mouse_x[i] - data_final.position_target_x[i], 2) + pow(data_final.position_mouse_y[i] - data_final.position_target_y[i], 2));//calcul de la distance
        diametre[i] = data_final.rayon_cercle[i]; //recupere le diametre du cercle
        Fitts[i] = (a + b * log2(1 + (distance[i]/(diametre[i]))))*1000; //loi de fitts
    }

    //creer de vecteur (un pour le temps empirique et l'autre pour loi de fitts)
    QVector<double> x0(taille + 1), y0(taille + 1);
    QVector<double> x1(taille + 1), y1(taille + 1);
    for (int i=1; i<=taille; ++i)
    {
        x0[i] = i;
        y0[i] = valeur[i]; //temps empirique

        x1[i] = i;
        y1[i] = Fitts[i]; //loi de fitts
    }

    //update les données du graphique
    ui->customPlot->graph(0)->setData(x0, y0); //mise a jour des données
    ui->customPlot->graph(0)->setPen(QPen(QColor(255, 0, 0))); //coulleur du trait
    ui->customPlot->graph(0)->setName("Loi de Fitts empirique"); //defini nom

    //update les données du graphique
    ui->customPlot->graph(1)->setData(x1, y1); //mise a jour des données
    ui->customPlot->graph(1)->setPen(QPen(QColor(0, 255, 0))); //coulleur du trait
    ui->customPlot->graph(1)->setName("Loi de Fitts theorique"); //defini nom

    ui->customPlot->xAxis->setRange(1, taille + 1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->customPlot->graph(1)->rescaleAxes(true);

    //set the plot
    ui->customPlot->replot();
    ui->customPlot->update();

}

//creer le tableau
void Result_Fitts::tableData(){
    // generate some data:
    int taille = int(data_final.nb_turn);  //nombre de données

    ui->donnee->setRowCount(taille);//nombre de ligne
    ui->donnee->setColumnCount(4);  //nombre de colonne

    //titre des colonnes
    ui->donnee->setHorizontalHeaderLabels(QStringList() << "Distance au depart" << "Diametre du cercle" << "Temps empirique (Temps du sujet)" << "Temps theorique (Loi de Fitts)");

    updateTable();//met a jour les donnees
}

//met a jour le tableaux
void Result_Fitts::updateTable(){
    int taille = int(data_final.nb_turn);//nombre de données

    int valeur[taille];
    //int valeur[taille]; //temps empirique
    for(int i = 0; i < taille; i++){
        valeur[i] = int(data_final.time_to_target[i+1]); //recupeartion des données
    }

    int Fitts[taille];//loi de fitts
    int distance[taille];//distance souris cible
    int diametre[taille];//diametre du cercle
    int temps[taille];//temps mis par l'utilisateur
    //recupere les valeur a et b voulu par l'utilisateur
    double a = ui->Value_a->value();
    double b = ui->Value_b->value();
    for(int i = 0; i < taille; i++){
        distance[i] = sqrt(pow(data_final.position_mouse_x[i+1] - data_final.position_target_x[i+1], 2) + pow(data_final.position_mouse_y[i+1] - data_final.position_target_y[i+1], 2));//calcul de la distance
        diametre[i] = data_final.rayon_cercle[i+1];//diametre du cercle
        Fitts[i] = (a + b * log2(1 + (distance[i]/(diametre[i]))))*1000;//loi de fitts en milliseconde
        temps[i] = data_final.time_to_target[i+1]; //temps mis
    }

    for(int i = 0; i<taille; i++){

        //conversion des ont en qstrng pour le tableau
        QString s_distance = QString::number(distance[i]);
        QString s_rayon = QString::number(diametre[i]);
        QString s_temps = QString::number(temps[i]);
        QString s_fitts = QString::number(Fitts[i]);

        //affichage des valeur dans le tableau
        ui->donnee->setItem(i, 0, new QTableWidgetItem(s_distance));
        ui->donnee->setItem(i, 1, new QTableWidgetItem(s_rayon));
        ui->donnee->setItem(i, 2, new QTableWidgetItem(s_temps));
        ui->donnee->setItem(i, 3, new QTableWidgetItem(s_fitts));
    }

    //calcul de l'ecart moyen
    auto mean = abs((std::accumulate(&Fitts[0], &Fitts[taille], 0)/taille+1)-(std::accumulate(&valeur[0], &valeur[taille], 0)/taille));
    ui->Ecart_moyen->setNum(mean);//affiche

    //calcul de la variance (pour l'ecart type ensuite)
    int variance = 0;
    for(int i = 0; i < taille; i++){
        variance += pow((Fitts[i] - valeur[i]), 2);
    }
    ui->Ecart_type->setNum(sqrt(variance/(taille+1)));//affiche ecart type

    //calcul ecart max et min
    int mini = 999999;
    int maxi = 0;
    int val;
    for(int i = 0; i < taille; i++){
        val = abs(int(Fitts[i])-int(temps[i]));
        if(val > maxi){
            maxi = val;
        }
        if(val < mini){
            mini = val;
        }
    }
    ui->Maximum_ecart->setNum(maxi);
    ui->Minimum_ecart->setNum(mini);
}

//valeur de a changer
void Result_Fitts::on_Value_a_valueChanged(double arg1)
{
    updatePLot(); //met a jour le graphique
    updateTable(); //met a jour le tableau
}

//valeur de b changer
void Result_Fitts::on_Value_b_valueChanged(double arg1)
{
    updatePLot(); //met a jour le graphique
    updateTable(); //met a jour le tableau
}

//met les données dans un fichier csv
void Result_Fitts::on_Extract_clicked()
{
    ui->Extract->setEnabled(false);

    FILE *file_extract;//creation du pointeur vers le fichier
    file_extract = fopen("data.csv","a"); //ouverture (ou creation) en ajout

    //test si le fichier est bien ouvert
    if(file_extract == NULL) //le fichier n'est pas ouvert
    {
       ui->Donnee_extraite->setText("Error");
        printf("Error!");
    }

    int taille = int(data_final.nb_turn); // taille des données

    int valeur[taille]; //temps empirique
    for(int i = 0; i < taille; i++){
        valeur[i] = int(data_final.time_to_target[i+1]);//transmission des donné
    }

    int Fitts[taille];
    int distance[taille];
    int diametre[taille];
    int temps[taille];
    double a = ui->Value_a->value();
    double b = ui->Value_b->value();
    for(int i = 0; i < taille; i++){
        distance[i] = sqrt(pow(data_final.position_mouse_x[i+1] - data_final.position_target_x[i+1], 2) + pow(data_final.position_mouse_y[i+1] - data_final.position_target_y[i+1], 2));
        diametre[i] = data_final.rayon_cercle[i+1];
        Fitts[i] = (a + b * log2(1 + (distance[i]/(diametre[i]))))*1000;
        temps[i] = data_final.time_to_target[i+1];

        fprintf(file_extract, "%d;%d;%d;%d;\n", distance[i], int(diametre[i]/2), temps[i], Fitts[i]); //ecriture des donnnées
    }

    ui->Donnee_extraite->setText("Donnée extraite");
}

//bouton quitter
void Result_Fitts::on_pushButton_2_clicked()
{
    QApplication::quit(); //ferme la fenetre
}

void Result_Fitts::on_Retry_clicked()
{
    // restart:
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
