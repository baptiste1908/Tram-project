#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

//Constructeur de la classe Station
Station::Station(const std::string& nom, double position)
    : nom(nom), position(position), metroPresent(false) {
    //On initialise les attributs de la station avec les valeurs que l'on a 
}

//Cette méthode est implémenter pour signaler l'arrivée d'un métro à la station
void Station::signalerArriveeMetro() {
    metroPresent = true; //On met à jour l'indicateur de présence de métro à vrai
}

//Cette méthode est implémenter pour signaler le départ d'un métro de la station
void Station::signalerDepartMetro() {
    metroPresent = false; //On met à jour l'indicateur de présence de métro à faux
}

//Cette méthode est implémenter pour vérifier si un métro est présent à la station
bool Station::estMetroPresent() const {
    return metroPresent; //On retourne l'état de l'indicateur de présence de métro
}

//Cette méthode est implémenter pour simuler la gestion des passagers sur le quai
int Station::gererPassagers() const {
    //On génère un nombre aléatoire de passagers
    return rand() % 150; 
}