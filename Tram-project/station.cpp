

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

// Constructeur de la classe Station
Station::Station(const std::string& nom, double position)
    : nom(nom), position(position), metroPresent(false) {
}

// M�thode pour signaler l'arriv�e d'un m�tro � la station
void Station::signalerArriveeMetro() {
    metroPresent = true;
}

// M�thode pour signaler le d�part d'un m�tro de la station
void Station::signalerDepartMetro() {
    metroPresent = false;
}

// M�thode pour v�rifier si un m�tro est pr�sent � la station
bool Station::estMetroPresent() const {
    return metroPresent;
}

// M�thode pour simuler la gestion des passagers sur le quai
int Station::gererPassagers() const {
    // G�n�ration d'un nombre al�atoire de passagers
    // Cette simulation peut �tre ajust�e en fonction des besoins sp�cifiques du sc�nario
    return rand() % 150; // Nombre al�atoire de passagers entre 0 et 149
}
