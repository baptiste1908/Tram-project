// Tram-project.cpp : définit le point d'entrée de l'application.
//

#include "Tram-project.h"

using namespace std;


int main() {
    // Exemple d'utilisation des classes
    Rame rame1(1, 0.0, 30.0, 50);
    Rame rame2(2, 10.0, 25.0, 40);

    Station station1;
    station1.nombrePassagersQuai = 15;

    Superviseur superviseur;
    superviseur.rames.push_back(rame1);
    superviseur.rames.push_back(rame2);

    superviseur.recupererPositionsRames();

    return 0;
}
