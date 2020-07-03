#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <array>

class Data
{
public:
    Data();
    Data(const Data& d);
    Data(int n);
    ~Data();

    Data& operator=(const Data& d);

    int nb_turn;
    int taille_max;
    int taille_min;

    int *position_mouse_x;
    int *position_mouse_y;
    int *position_target_x;
    int *position_target_y;
    int *time_to_target;
    int *rayon_cercle;
};

#endif // DATA_H
