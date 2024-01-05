

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

// M�thode pour v�rifier et r�agir aux �v�nements
void Superviseur::verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
            if (boutonArret.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                std::cout << "Bouton Stop cliqu�" << std::endl;

                toggleArretUrgence(clock, tempsTotalArrete);
            }
        }
    }
}

// M�thode pour activer ou d�sactiver l'arr�t d'urgence
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

// M�thode pour mettre � jour les positions des m�tros
void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
    double positionMetro1 = metro1.getPosition();
    double positionMetro2 = metro2.getPosition();

    metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); // Passer une r�f�rence � ce superviseur
    metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); // Idem ici
}


// M�thode pour obtenir l'�tat de l'arr�t d'urgence
bool Superviseur::getArretUrgence() const {
    return arretUrgence;
}

// M�thode pour obtenir l'ID du premier m�tro
int Superviseur::getIdMetro1() const {
    return m_metro1.getID();
}

// M�thode pour obtenir l'ID du deuxi�me m�tro
int Superviseur::getIdMetro2() const {
    return m_metro2.getID();
}

// M�thode pour obtenir la position du premier m�tro
double Superviseur::getPositionMetro1() const {
    return m_metro1.getPosition();
}

// M�thode pour obtenir la position du deuxi�me m�tro
double Superviseur::getPositionMetro2() const {
    return m_metro2.getPosition();
}


// M�thode pour d�clencher un arr�t d'urgence
void Superviseur::arretDurgence() {
    m_metro1.arretTotal();
    m_metro2.arretTotal();
    arretUrgence = true;
    // Ajoutez ici d'autres actions pour l'arr�t d'urgence
}

// M�thode pour v�rifier la distance de s�curit� entre les m�tros
void Superviseur::verifierDistanceSecurite(Metro& metro1, Metro& metro2) {
    auto elapsed = demarrageClock.getElapsedTime().asSeconds();

    // Activer la distance de s�curit� apr�s 20 secondes
    if (elapsed > 20 && !activerDistanceSecurite) {
        activerDistanceSecurite = true;
        std::cout << "Activation de la distance de s�curit� apr�s 20 secondes." << std::endl;
    }

    if (!activerDistanceSecurite) {
        return; // Si la distance de s�curit� n'est pas activ�e, rien de plus � faire
    }

    double distanceSecurite = 50.0; // Distance de s�curit� en m�tres
    double distanceActuelle = std::abs(metro1.getPosition() - metro2.getPosition());

    // V�rifier si les deux m�tros se d�placent dans la m�me direction
    bool memeDirection = metro1.directionPositive == metro2.directionPositive;

    // V�rifier la distance et d�clencher un arr�t d'urgence si n�cessaire
    if (memeDirection && distanceActuelle < distanceSecurite && !arretUrgence) {
        std::cout << "Arr�t d'urgence d�clench� en raison d'une proximit� trop proche." << std::endl;

        arretDurgence();
    }

}