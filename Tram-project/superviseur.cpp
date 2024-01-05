

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


// Constructeur de la classe Superviseur
Superviseur::Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window)
    : m_metro1(metro1), m_metro2(metro2), m_window(window),
    enArret(false), arretUrgence(false), activerDistanceSecurite(false) {
    demarrageTime = std::chrono::steady_clock::now();
}

// Méthode pour vérifier et réagir aux événements
void Superviseur::verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
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

// Méthode pour activer ou désactiver l'arrêt d'urgence
void Superviseur::toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete) {
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

// Méthode pour mettre à jour les positions des métros
void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
    double positionMetro1 = metro1.getPosition();
    double positionMetro2 = metro2.getPosition();

    metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); // Passer une référence à ce superviseur
    metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); // Idem ici
}


// Méthode pour obtenir l'état de l'arrêt d'urgence
bool Superviseur::getArretUrgence() const {
    return arretUrgence;
}

// Méthode pour obtenir l'ID du premier métro
int Superviseur::getIdMetro1() const {
    return m_metro1.getID();
}

// Méthode pour obtenir l'ID du deuxième métro
int Superviseur::getIdMetro2() const {
    return m_metro2.getID();
}

// Méthode pour obtenir la position du premier métro
double Superviseur::getPositionMetro1() const {
    return m_metro1.getPosition();
}

// Méthode pour obtenir la position du deuxième métro
double Superviseur::getPositionMetro2() const {
    return m_metro2.getPosition();
}


// Méthode pour déclencher un arrêt d'urgence
void Superviseur::arretDurgence() {
    m_metro1.arretTotal();
    m_metro2.arretTotal();
    arretUrgence = true;
    // Ajoutez ici d'autres actions pour l'arrêt d'urgence
}

// Méthode pour vérifier la distance de sécurité entre les métros
void Superviseur::verifierDistanceSecurite(Metro& metro1, Metro& metro2) {
    auto elapsed = demarrageClock.getElapsedTime().asSeconds();

    // Activer la distance de sécurité après 20 secondes
    if (elapsed > 20 && !activerDistanceSecurite) {
        activerDistanceSecurite = true;
        std::cout << "Activation de la distance de sécurité après 20 secondes." << std::endl;
    }

    if (!activerDistanceSecurite) {
        return; // Si la distance de sécurité n'est pas activée, rien de plus à faire
    }

    double distanceSecurite = 50.0; // Distance de sécurité en mètres
    double distanceActuelle = std::abs(metro1.getPosition() - metro2.getPosition());

    // Vérifier si les deux métros se déplacent dans la même direction
    bool memeDirection = metro1.directionPositive == metro2.directionPositive;

    // Vérifier la distance et déclencher un arrêt d'urgence si nécessaire
    if (memeDirection && distanceActuelle < distanceSecurite && !arretUrgence) {
        std::cout << "Arrêt d'urgence déclenché en raison d'une proximité trop proche." << std::endl;

        arretDurgence();
    }

}