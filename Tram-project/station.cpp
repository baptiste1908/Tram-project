

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

// Constructeur de la classe Station
Station::Station(const std::string& nom, double position)
    : nom(nom), position(position), metroPresent(false) {
}

// Méthode pour signaler l'arrivée d'un métro à la station
void Station::signalerArriveeMetro() {
    metroPresent = true;
}

// Méthode pour signaler le départ d'un métro de la station
void Station::signalerDepartMetro() {
    metroPresent = false;
}

// Méthode pour vérifier si un métro est présent à la station
bool Station::estMetroPresent() const {
    return metroPresent;
}

// Méthode pour simuler la gestion des passagers sur le quai
int Station::gererPassagers() const {
    // Génération d'un nombre aléatoire de passagers
    // Cette simulation peut être ajustée en fonction des besoins spécifiques du scénario
    return rand() % 150; // Nombre aléatoire de passagers entre 0 et 149
}
