
#pragma once 
#include <iostream>
#include "main.cpp"



class Superviseur {
    friend class Metro;
    friend class Station;
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);

public:
    Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window)
        : m_metro1(metro1), m_metro2(metro2), m_window(window), arretUrgence(false) {

    }

    void verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
                if (boutonArret.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Stop cliqué" << std::endl;
                    toggleArretUrgence(clock, tempsTotalArrete);
                }
            }
        }
    }

    void toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete) {
        arretUrgence = !arretUrgence;
        if (arretUrgence) {
            tempsTotalArrete += clock.getElapsedTime();
            m_metro1.arretTotal();
            m_metro2.arretTotal();
        }
        else {
            m_metro1.quitterStation();
            m_metro2.quitterStation();
        }
    }

    void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
        double positionMetro1 = metro1.getPosition();
        double positionMetro2 = metro2.getPosition();

        metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); // Passer une référence à ce superviseur
        metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); // Idem ici
    }


    bool getArretUrgence() const { return arretUrgence; }

    // Méthode pour obtenir l'ID du premier métro
    int getIdMetro1() const {
        return m_metro1.getID();
    }

    // Méthode pour obtenir l'ID du deuxième métro
    int getIdMetro2() const {
        return m_metro2.getID();
    }

    // Méthode pour obtenir la position du premier métro
    double getPositionMetro1() const {
        return m_metro1.getPosition();
    }

    // Méthode pour obtenir la position du deuxième métro
    double getPositionMetro2() const {
        return m_metro2.getPosition();
    }
    void arretDurgence() {
        m_metro1.arretTotal();
        m_metro2.arretTotal();
        // Ajoutez ici d'autres actions pour l'arrêt d'urgence
    }

    void verifierDistanceSecurite() {
        if (demarrageClock.getElapsedTime().asSeconds() > 20.0) {
            activerDistanceSecurite = true;
        }
    }

    // Assurez-vous que ces membres sont accessibles là où ils sont nécessaires
    sf::RectangleShape boutonArret;
    sf::Text texteBouton;
    sf::Time tempsArrete;
    sf::Clock demarrageClock;
    bool activerDistanceSecurite = false;

private:
    Metro& m_metro1;
    Metro& m_metro2;
    sf::RenderWindow& m_window;
    bool enArret = false;
    bool arretUrgence;





};