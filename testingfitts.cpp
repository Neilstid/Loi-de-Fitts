#include "testingfitts.h"
#include "ui_testingfitts.h"

//pour transmetre et recevoir les données
#include "data.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "result_fitts.h"

//Les include des objet QT
#include <QMouseEvent>
#include <QTime>
#include <QSize>
#include <QLabel>
#include <QSize>
#include <QScreen>
#include <QLCDNumber>
#include <QPoint>
#include <QTimer>

//pour les calculs
#include <cmath>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//Variables globales
int position_figure_x = 0;
int position_figure_y = 0;
int rayon = 100000;
int update_img = 0;
int turn = 0;
int interface_chosen = 0;
int nb_update = 0;
int seed_global;

//Pour le calcul du temps
QTime *timer = new QTime();

//La classe pour les données
Data *data_transfert = new Data(100);

//constructeur (avec recuperation de la classe data creer dans mainwindow)
TestingFitts::TestingFitts(QWidget *parent, Data *d, int interface, int seed) :
    QMainWindow(parent),
    ui(new Ui::TestingFitts)
{
    ui->setupUi(this);

    seed_global = seed;

    //tracke la souris pour determiner plus tard sa position
    setMouseTracking(true);

    // initialize random seed:
    if(seed == 0){
        srand (time(NULL));
    }else{
        srand(seed);
    }

    //transfert les variables recuperer dans le constructeur dans une variable global
    data_transfert->nb_turn = d->nb_turn; //nombre de tour
    data_transfert->taille_max = d->taille_max; //la taille max
    data_transfert->taille_min = d->taille_min; //la taille min
    //libere la memoire
    free(d);

    interface_chosen = interface;

    updateInterface();

}

TestingFitts::~TestingFitts()
{
    delete ui;
}

void TestingFitts::paintEvent(QPaintEvent *event){
    //Aucune update (s'active a chaque clique)

    //Si besoin d'une update
    if(update_img == 1){

        //Recupere la taille de l'ecran
        QSize ecran = qApp->screens()[0]->size(); //Defini l'ecran
        int h = ecran.height(); //Recupere la hauteur de l'ecran
        int w = ecran.width(); //Recupere la largeur de l'ecran

        //Defini les rayon du cercle (Pour le moment diametre)
        int rayon_min = data_transfert->taille_min; //Rayon min donné par l'utilisateur
        //Rayon max auquel on retire le rayon min pour ne pas que ce soit sup lors de la generation aléatoire
        int rayon_max = data_transfert->taille_max - data_transfert->taille_min;

        //genere l'aléatoire
        rayon = rayon_min + rand() % rayon_max; //rayon du cercle
        position_figure_x = rayon/2 + rand() % (w-rayon); //position x du centre du cercle
        position_figure_y = rayon/2 + rand() % (h-rayon); //position y du centre du cercle

        //transmission des données a la classe data
        data_transfert->position_target_x[turn] = position_figure_x; //transmet la position du centre x du cercle i
        data_transfert->position_target_y[turn] = position_figure_y; //transmet la position du centre y du cercle i
        data_transfert->rayon_cercle[turn] = rayon; //transmet le rayon du cercle i

        //creer le cercle
        QPoint centre(position_figure_x,position_figure_y);
        QPainter ElipsePainter(this); //Defini l'objet (ici une elipse que l'on va transformer en cercle)
        ElipsePainter.setBrush(Qt::red); //On fait un enterieur plein rouge
        ElipsePainter.setPen(Qt::NoPen); //Aucun contoure au cercle
        ElipsePainter.drawEllipse(centre,rayon,rayon); //dessine le cercle
        update_img = 0; //On bloque les update de notre cercle (il faudra cliquer sur le cercle pour debloquer)
    }
}

void TestingFitts::mousePressEvent(QMouseEvent *event)
{

    //On recupere les position x et y de la souris
    int x = event->globalX(); //position x actuel de la souris
    int y = event->globalY(); //position y actuel de la souris

    //calcul la distance du clique par rapport au centre du cercle
    int distance = sqrt(pow(event->globalX() - data_transfert->position_target_x[turn], 2) + pow(event->globalY() - data_transfert->position_target_y[turn], 2));

    //test si le clique est dans le cercle
    if(distance <= rayon or turn == 0){ //position x de la souris dans le cercle

        //temps mis par l'utilisateur
        int time_to_target = timer->elapsed(); //recupere le temps
        data_transfert->time_to_target[turn] = time_to_target;
        timer->start(); //relance le timer

        turn++;//On incremente le tour

        //position de depart de la souris
        data_transfert->position_mouse_x[turn] = x; //position x de la souris
        data_transfert->position_mouse_y[turn] = y; //position y de la souris

        //update du cercle
        update_img = 1; //on debloque l'update
        QWidget::update(); //on lance l'update

        updateInterface();

        //test si le nomre de tour est passé
        if(turn > data_transfert->nb_turn){
            this->close();//ferme cette application
            Result_Fitts *r = new Result_Fitts(nullptr, data_transfert);//Creer la prochaine application (resulat)
            r->show();//l'affiche
        }
    }
}


//update l'interface utilisateur
void TestingFitts::updateInterface(){
    if(interface_chosen == 1 and nb_update == 0){//Si c'est la premiere mise a jour et que l'iterface simple a ete choisi

        ui->Upper_Message->setText("Cliquez n'importe ou sur l'écran pour commencer"); //message aficher a l'utilisateur en haut a gauche

    }else if(interface_chosen == 2){//Si c'est la premiere mise a jour et que l'iterface complete a ete choisi

        QString seed_msg = "Seed : " + QString::number(seed_global) + "     Nombre de cercle : " + QString::number(turn) + "/" + QString::number(data_transfert->nb_turn);
        ui->Upper_Message->setText(seed_msg);//message aficher a l'utilisateur en haut a gauche

    }else if(interface_chosen == 1 and nb_update != 0){//Si ce n'est pas la premiere mise a jour et que l'iterface simple a ete choisi

        ui->Upper_Message->setText("Veuillez cliquer sur les cercles <font color=\"red\">rouges</font> qui s'affiche sur l'ecran !");//message aficher a l'utilisateur en haut a gauche

    }else{

    }
    nb_update++;//on augmente le nombre d'update realise
}
