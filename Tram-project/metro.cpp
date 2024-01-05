#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

const double EPSILON = 1e-5;
const double SEUIL_DEC�L�RATION = 50.0;

// Constructeur de la classe Metro
Metro::Metro()
    : position(0.0), distanceEntreStations(0.0), vitesse(0.0),
    vitesseCroisiere(2.0), vitesseArret(0.0), dureeArret(5.0),
    capaciteMaximale(200), nombrePassagers(0), enArret(false),
    directionPositive(true), facteurVariabilite(static_cast<double>(rand()) / RAND_MAX),
    positionRamePrecedente(0.0), activerDistanceSecurite(false), id(rand()) {
}

// M�thode pour r�cup�rer l'ID du m�tro
int Metro::getID() const {
    return id;
}

// V�rifier si le m�tro est � une station
bool Metro::estALaStation() const {
    return enArret;
}

// Initialiser le m�tro avec la distance entre les stations
void Metro::initialiser(double distanceEntreStations) {
    this->distanceEntreStations = distanceEntreStations;
}

// Quitter la station
void Metro::quitterStation() {
    accelerer(vitesseCroisiere);
    enArret = false;
}

// Parcourir la ligne de m�tro
void Metro::parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur) {
    this->positionRamePrecedente = positionPrecedente;
    double distanceSecurite = 50.0; // Distance de s�curit� en m�tres
    double distanceAvecPrecedente = std::abs(position - positionRamePrecedente);

    if (!enArret) {
        double deplacement = (vitesse * elapsedTime.asSeconds()) / 4.0;
        bool doitSArreter = false;

        double prochaineStationPos = getProchaineStationPosition(*this, stations);
        double distanceProchaineStation = std::abs(prochaineStationPos - position);

        // G�rer la d�c�l�ration avant d'arriver � la station
        if (distanceProchaineStation <= SEUIL_DEC�L�RATION && vitesse > EPSILON) {
            vitesse = std::max(vitesseArret, vitesse - 0.1);
        }

        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5); // Ralentir si n�cessaire
        }

        // D�placement du m�tro
        if (directionPositive) {
            position += deplacement;
            if (position >= distanceEntreStations) {
                position = distanceEntreStations;
                doitSArreter = true;
            }
        }
        else {
            position -= deplacement;
            if (position <= 0.0) {
                position = 0.0;
                doitSArreter = true;
            }
        }

        // Arr�t � la station
        if (doitSArreter && !enArret) {
            arreterAStation();
        }
    }
}

// Arr�ter le m�tro � une station
void Metro::arreterAStation() {
    if (!enArret) {
        vitesse = vitesseArret;
        enArret = true;
        stationClock.restart();

        // Simulation de l'arr�t � la station
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dureeArret * 1000)));
        std::cout << "Metro " << id << " s'arr�te � la station." << std::endl;
    }
}

// G�rer les passagers montant et descendant
void Metro::gererPassagers(int passagersQuai) {
    // Facteur de variabilit� bas� sur l'ID du m�tro et une valeur al�atoire
    int variabilite = (rand() % 20 + 1) * (id % 15);

    // Calcul du nombre de passagers sortants avec variabilit�
    passagersSortants = std::min(nombrePassagers, rand() % (nombrePassagers + 1) + variabilite / 20);

    // Calcul du nombre de passagers entrants avec variabilit�
    passagersEntrants = std::min(capaciteMaximale - nombrePassagers, passagersQuai) + variabilite / 25;

    // Mise � jour du nombre de passagers � bord
    nombrePassagers += passagersEntrants - passagersSortants;

    // Assurer que le nombre de passagers reste dans les limites
    if (nombrePassagers > capaciteMaximale) nombrePassagers = capaciteMaximale;
    if (nombrePassagers < 0) nombrePassagers = 0;
}

// Arr�t total du m�tro
void Metro::arretTotal() {
    enArret = true;
    vitesse = vitesseArret;
    arretUrgence = true;
}

// Reprendre apr�s un arr�t d'urgence
void Metro::reprendreApr�sArretUrgence() {
    if (arretUrgence) {
        enArret = false;
        accelerer(vitesseCroisiere);
        arretUrgence = false;
    }
}

int Metro::getPassagersEntrants() const {
    return passagersEntrants;
}

int Metro::getPassagersSortants() const {
    return passagersSortants;
}

double Metro::getVitesse() const {
    return vitesse;
}

double Metro::getPosition() const {
    return position;
}

int Metro::getCapaciteMaximale() const {
    return capaciteMaximale;
}

int Metro::getNombrePassagers() const {
    return nombrePassagers;
}

double Metro::getDureeArret() const {
    return dureeArret;
}

void Metro::accelerer(double acceleration) {
    vitesse += acceleration;
    if (vitesse > vitesseCroisiere) {
        vitesse = vitesseCroisiere;
    }
}

double Metro::fonctionTransition(double t) const {
    // Formule hypoth�tique de transition
    return 2 * t * t * t - 3 * t * t + 1;
}

