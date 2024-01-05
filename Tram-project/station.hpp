
#ifndef STATION_HPP
#define STATION_HPP

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

class Station {
    friend class Metro;
    friend class Superviseur;
	
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);
    friend double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations);
	
private:
    std::string nom;
    double position;
    bool metroPresent;

public:
    Station(const std::string& nom, double position);

	
    void signalerArriveeMetro();
    void signalerDepartMetro();
    bool estMetroPresent() const;
    int gererPassagers() const;
};

#endif // STATION_HPP
