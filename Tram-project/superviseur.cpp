

#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


// On d�finit le constructeur par d�feaut de la classe superviseur
Superviseur::Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window)
    : m_metro1(metro1), m_metro2(metro2), m_window(window),
    enArret(false), arretUrgence(false), activerDistanceSecurite(false) {
    demarrageTime = std::chrono::steady_clock::now();
}



// On va ensuite cr�er une fonction pour l'arret d'urgence quand on appuie sur le bouton stop
void Superviseur::verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
	
    // On v�rifie d'abord si un clic de souris a eu lieu.
    if (event.type == sf::Event::MouseButtonPressed) {
        // On v�rifie aussi si le bouton cliqu� est le bouton gauche de la souris.
        if (event.mouseButton.button == sf::Mouse::Left) {
			// dans ce cas on obtient la position actuelle de la souris.
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			
            // On v�rifie aussi si la position de la souris est sur le bouton d'arr�t d'urgence.
            if (boutonArret.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                std::cout << "Bouton Stop cliqu�" << std::endl;

				//On uilise la fonction d'apres pour activer ou d�sactiver l'arr�t d'urgence en fonction de l'�tat actuel
                toggleArretUrgence(clock, tempsTotalArrete);
            }
        }
    }
}

// On d�finit donc la m�thode pour activer ou d�sactiver l'arr�t d'urgence
void Superviseur::toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete) {
    // D�s le d�but on bascule l'�tat de l'arr�t d'urgence.
    arretUrgence = !arretUrgence;

	// Si l'arr�t d'urgence est activ�, on met les 2 m�tros en arr�t et on enregistre le temps �coul� jusqu'� l'arr�t.
    if (arretUrgence) {
        tempsTotalArrete += clock.getElapsedTime();
        m_metro1.arretTotal();
        m_metro2.arretTotal();
    }
    // Et si l'arr�t d'urgence est d�sactiv�,alors on reprend le trajet des m�tros.
    else {
        m_metro1.quitterStation();
        m_metro2.quitterStation();
    }
}


// On fait ensuite la m�thode pour mettre � jour les positions des m�tros
void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
	//IL faaut d'abord obtenir la position des m�tros
    double positionMetro1 = metro1.getPosition();
    double positionMetro2 = metro2.getPosition();

    // On fait avancer le premier m�tro le long de la ligne.
    metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); 
    // On fait avancer le deuxi�me m�tro le long de la ligne.
    metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); 
}




//On va ensuite d�finir plein de petite m�thode pour obtenir diff�rentes chose tel que : l'arret d'urgence, les ID des 2 metros, ainsi que leurs positions.
// VOici les differentes M�thodes :

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




// Ainsi que la m�thode pour d�clencher un arr�t d'urgence
void Superviseur::arretDurgence() {
    // Ou on provoque tout simplment l'arret total des 2 m�tros.
    m_metro1.arretTotal();
    m_metro2.arretTotal();
    arretUrgence = true; 

}




// Enfin on cr�er une m�thode pour v�rifier la distance de s�curit� entre les m�tros
void Superviseur::verifierDistanceSecurite(Metro& metro1, Metro& metro2) {

    // On demarre une clock 
    auto elapsed = demarrageClock.getElapsedTime().asSeconds(); 

    // On choisit d'active la distance de s�curit� apr�s 20 secondes pour eviter des problemes au d�but 
    if (elapsed > 20 && !activerDistanceSecurite) {
        activerDistanceSecurite = true;
        std::cout << "Activation de la distance de s�curit� apr�s 20 secondes." << std::endl;
    }

    // Si la distance de s�curit� n'est pas activ�e, rien de plus � faire
    if (!activerDistanceSecurite) {
        return; 
    }
    
    // On initialise � 50 metres la distance de s�curit�
    double distanceSecurite = 50.0; 
    // Et on calcule la distance actuelle entre les 2 m�tros.
    double distanceActuelle = std::abs(metro1.getPosition() - metro2.getPosition());

    // Evidemment on v�rifie si les deux m�tros se d�placent dans la m�me direction car sinont quand un metro fait l'aller mais que l'autre faist le retour. 
    // il serait trop proche et donc d�clencheraient l'arret d'urgence
    bool memeDirection = metro1.directionPositive == metro2.directionPositive;

    // Enfin on v�rifie la distance et on d�clenche un arr�t d'urgence si n�cessaire
    if (memeDirection && distanceActuelle < distanceSecurite && !arretUrgence) {
        std::cout << "Arr�t d'urgence d�clench� en raison d'une proximit� trop proche." << std::endl;

        arretDurgence();
    }

}