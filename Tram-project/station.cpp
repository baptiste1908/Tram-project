#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

//Constructeur de la classe Station
Station::Station(const std::string& nom, double position)
    : nom(nom), position(position), metroPresent(false) {
    //On initialise les attributs de la station avec les valeurs que l'on a 
}

//Cette m�thode est impl�menter pour signaler l'arriv�e d'un m�tro � la station
void Station::signalerArriveeMetro() {
    metroPresent = true; //On met � jour l'indicateur de pr�sence de m�tro � vrai
}

//Cette m�thode est impl�menter pour signaler le d�part d'un m�tro de la station
void Station::signalerDepartMetro() {
    metroPresent = false; //On met � jour l'indicateur de pr�sence de m�tro � faux
}

//Cette m�thode est impl�menter pour v�rifier si un m�tro est pr�sent � la station
bool Station::estMetroPresent() const {
    return metroPresent; //On retourne l'�tat de l'indicateur de pr�sence de m�tro
}

//Cette m�thode est impl�menter pour simuler la gestion des passagers sur le quai
int Station::gererPassagers() const {
    //On g�n�re un nombre al�atoire de passagers
    return rand() % 150; 
}