#include "data.h"

Data::Data()
{
    nb_turn = 0;
    taille_max = 0;
    taille_min = 0;

    position_mouse_x = NULL;
    position_mouse_y  = NULL;
    position_target_x  = NULL;
    position_target_y  = NULL;
    time_to_target  = NULL;
    rayon_cercle  = NULL;
}

Data::Data(const Data& d){
    nb_turn = d.nb_turn;
    taille_max = d.taille_max;
    taille_min = d.taille_min;

    delete[] position_mouse_x;
    delete[] position_mouse_y;
    delete[] position_target_x;
    delete[] position_target_y;
    delete[] time_to_target;
    delete[] rayon_cercle;

    position_mouse_x = new int[d.nb_turn];
    position_mouse_y  = new int[d.nb_turn];
    position_target_x  = new int[d.nb_turn];
    position_target_y  = new int[d.nb_turn];
    time_to_target  = new int[d.nb_turn];
    rayon_cercle  = new int[d.nb_turn];

    for(int i = 0; i <= nb_turn; i++){
        position_mouse_x[i] = d.position_mouse_x[i];
        position_mouse_y[i]  = d.position_mouse_y[i];
        position_target_x[i]  = d.position_target_x[i];
        position_target_y[i]  = d.position_target_y[i];
        time_to_target[i]  = d.time_to_target[i];
        rayon_cercle[i]  = d.rayon_cercle[i];
    }
}

Data::Data(int n)
{
    nb_turn = n;
    taille_max = 0;
    taille_min = 0;

    position_mouse_x = new int[nb_turn];
    position_mouse_y  = new int[nb_turn];
    position_target_x  = new int[nb_turn];
    position_target_y  = new int[nb_turn];
    time_to_target  = new int[nb_turn];
    rayon_cercle  = new int[nb_turn];
}

Data::~Data(){

}

Data& Data::operator=(const Data& d){
    if(this != &d){
        nb_turn = d.nb_turn;
        taille_max = d.taille_max;
        taille_min = d.taille_min;

        delete[] position_mouse_x;
        delete[] position_mouse_y;
        delete[] position_target_x;
        delete[] position_target_y;
        delete[] time_to_target;
        delete[] rayon_cercle;

        int *position_mouse_x = new int[d.nb_turn];
        int *position_mouse_y  = new int[d.nb_turn];
        int *position_target_x  = new int[d.nb_turn];
        int *position_target_y  = new int[d.nb_turn];
        int *time_to_target  = new int[d.nb_turn];
        int *rayon_cercle  = d.rayon_cercle;
/*
        for(int i = 0; i <= nb_turn; i++){
            position_mouse_x[i] = d.position_mouse_x[i];
            position_mouse_y[i]  = d.position_mouse_y[i];
            position_target_x[i]  = d.position_target_x[i];
            position_target_y[i]  = d.position_target_y[i];
            time_to_target[i]  = d.time_to_target[i];
            rayon_cercle[i]  = d.rayon_cercle[i];
        }*/
    }
    return *this;
}
