#ifndef METRO_HPP
#define METRO_HPP

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"





class Metro {
	
    friend class Superviseur; // Permet à Superviseur d'accéder aux membres privés de Metro
	friend class Station;
	
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations);

private:
    double position;
    double distanceEntreStations;
    double vitesse = 0;
    const double vitesseCroisiere = 2.0;
    const double vitesseArret = 0.0;
    const double dureeArret = 5.0;
    const int capaciteMaximale = 200;
    int nombrePassagers;
    sf::Clock stationClock;
    bool enArret;
    int passagersEntrants = 0;
    int passagersSortants = 0;

    bool directionPositive;
    double facteurVariabilite;
    bool arretUrgence = false;
    bool estEnProcessusDeDeceleration = false;
    double positionRamePrecedente;

    bool activerDistanceSecurite = false;
    int id = rand(); 

public:
    Metro();
    int getID() const;
    bool estALaStation() const;
    void initialiser(double distanceEntreStations);
    void quitterStation();
    void parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur);
    void arreterAStation();
    void gererPassagers(int passagersQuai);
    void arretTotal();
    void reprendreAprèsArretUrgence();
    int getPassagersEntrants() const;
    int getPassagersSortants() const;
    double getVitesse() const;
    double getPosition() const;
    int getCapaciteMaximale() const;
    int getNombrePassagers() const;
    double getDureeArret() const;
    void accelerer(double acceleration);
    double fonctionTransition(double t) const;
};

#endif // METRO_HPP
