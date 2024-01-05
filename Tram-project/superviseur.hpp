#ifndef SUPERVISEUR_HPP
#define SUPERVISEUR_HPP

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"



class Superviseur {

    friend class Metro;
    friend class Station;
	
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);
	

private:
       
    Metro& m_metro1;
    Metro& m_metro2;
    sf::RenderWindow& m_window;
    bool enArret = false;
    bool arretUrgence;
    bool activerDistanceSecurite = false;
    std::chrono::steady_clock::time_point demarrageTime;
	
   
	

	
public:
	
    Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window);

    void verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete);
    void toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete);
    void mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut);
    bool getArretUrgence() const;
    int getIdMetro1() const;
    int getIdMetro2() const;
    double getPositionMetro1() const;
    double getPositionMetro2() const;
    void arretDurgence();
    void verifierDistanceSecurite(Metro& metro1, Metro& metro2);

    sf::Clock demarrageClock;
    sf::RectangleShape boutonArret;
    sf::Text texteBouton;
    sf::Time tempsArrete;

    
};

#endif /
