

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


// On définit le constructeur par défeaut de la classe superviseur
Superviseur::Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window)
    : m_metro1(metro1), m_metro2(metro2), m_window(window),
    enArret(false), arretUrgence(false), activerDistanceSecurite(false) {
    demarrageTime = std::chrono::steady_clock::now();
}



// On va ensuite créer une fonction pour l'arret d'urgence quand on appuie sur le bouton stop
void Superviseur::verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
	
    // On vérifie d'abord si un clic de souris a eu lieu.
    if (event.type == sf::Event::MouseButtonPressed) {
        // On vérifie aussi si le bouton cliqué est le bouton gauche de la souris.
        if (event.mouseButton.button == sf::Mouse::Left) {
			// dans ce cas on obtient la position actuelle de la souris.
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			
            // On vérifie aussi si la position de la souris est sur le bouton d'arrêt d'urgence.
            if (boutonArret.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                std::cout << "Bouton Stop cliqué" << std::endl;

				//On uilise la fonction d'apres pour activer ou désactiver l'arrêt d'urgence en fonction de l'état actuel
                toggleArretUrgence(clock, tempsTotalArrete);
            }
        }
    }
}

// On définit donc la méthode pour activer ou désactiver l'arrêt d'urgence
void Superviseur::toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete) {
    // Dés le début on bascule l'état de l'arrêt d'urgence.
    arretUrgence = !arretUrgence;

	// Si l'arrêt d'urgence est activé, on met les 2 métros en arrêt et on enregistre le temps écoulé jusqu'à l'arrêt.
    if (arretUrgence) {
        tempsTotalArrete += clock.getElapsedTime();
        m_metro1.arretTotal();
        m_metro2.arretTotal();
    }
    // Et si l'arrêt d'urgence est désactivé,alors on reprend le trajet des métros.
    else {
        m_metro1.quitterStation();
        m_metro2.quitterStation();
    }
}


// On fait ensuite la méthode pour mettre à jour les positions des métros
void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
	//IL faaut d'abord obtenir la position des métros
    double positionMetro1 = metro1.getPosition();
    double positionMetro2 = metro2.getPosition();

    // On fait avancer le premier métro le long de la ligne.
    metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); 
    // On fait avancer le deuxième métro le long de la ligne.
    metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); 
}




//On va ensuite définir plein de petite méthode pour obtenir différentes chose tel que : l'arret d'urgence, les ID des 2 metros, ainsi que leurs positions.
// VOici les differentes Méthodes :

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




// Ainsi que la méthode pour déclencher un arrêt d'urgence
void Superviseur::arretDurgence() {
    // Ou on provoque tout simplment l'arret total des 2 métros.
    m_metro1.arretTotal();
    m_metro2.arretTotal();
    arretUrgence = true; 

}




// Enfin on créer une méthode pour vérifier la distance de sécurité entre les métros
void Superviseur::verifierDistanceSecurite(Metro& metro1, Metro& metro2) {

    // On demarre une clock 
    auto elapsed = demarrageClock.getElapsedTime().asSeconds(); 

    // On choisit d'active la distance de sécurité après 20 secondes pour eviter des problemes au début 
    if (elapsed > 20 && !activerDistanceSecurite) {
        activerDistanceSecurite = true;
        std::cout << "Activation de la distance de sécurité après 20 secondes." << std::endl;
    }

    // Si la distance de sécurité n'est pas activée, rien de plus à faire
    if (!activerDistanceSecurite) {
        return; 
    }
    
    // On initialise à 50 metres la distance de sécurité
    double distanceSecurite = 50.0; 
    // Et on calcule la distance actuelle entre les 2 métros.
    double distanceActuelle = std::abs(metro1.getPosition() - metro2.getPosition());

    // Evidemment on vérifie si les deux métros se déplacent dans la même direction car sinont quand un metro fait l'aller mais que l'autre faist le retour. 
    // il serait trop proche et donc déclencheraient l'arret d'urgence
    bool memeDirection = metro1.directionPositive == metro2.directionPositive;

    // Enfin on vérifie la distance et on déclenche un arrêt d'urgence si nécessaire
    if (memeDirection && distanceActuelle < distanceSecurite && !arretUrgence) {
        std::cout << "Arrêt d'urgence déclenché en raison d'une proximité trop proche." << std::endl;

        arretDurgence();
    }

}