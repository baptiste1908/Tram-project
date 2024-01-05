#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

const double EPSILON = 1e-5;
const double SEUIL_DECÉLÉRATION = 50.0;

// Constructeur de la classe Metro
Metro::Metro()
    : position(0.0), distanceEntreStations(0.0), vitesse(0.0),
    vitesseCroisiere(2.0), vitesseArret(0.0), dureeArret(5.0),
    capaciteMaximale(200), nombrePassagers(0), enArret(false),
    directionPositive(true), facteurVariabilite(static_cast<double>(rand()) / RAND_MAX),
    positionRamePrecedente(0.0), activerDistanceSecurite(false), id(rand()) {
}

// Méthode pour récupérer l'ID du métro
int Metro::getID() const {
    return id;
}

// Vérifier si le métro est à une station
bool Metro::estALaStation() const {
    return enArret;
}

// Initialiser le métro avec la distance entre les stations
void Metro::initialiser(double distanceEntreStations) {
    this->distanceEntreStations = distanceEntreStations;
}

// Quitter la station
void Metro::quitterStation() {
    accelerer(vitesseCroisiere);
    enArret = false;
}

// Parcourir la ligne de métro
void Metro::parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur) {
    this->positionRamePrecedente = positionPrecedente;
    double distanceSecurite = 50.0; // Distance de sécurité en mètres
    double distanceAvecPrecedente = std::abs(position - positionRamePrecedente);

    if (!enArret) {
        double deplacement = (vitesse * elapsedTime.asSeconds()) / 4.0;
        bool doitSArreter = false;

        double prochaineStationPos = getProchaineStationPosition(*this, stations);
        double distanceProchaineStation = std::abs(prochaineStationPos - position);

        // Gérer la décélération avant d'arriver à la station
        if (distanceProchaineStation <= SEUIL_DECÉLÉRATION && vitesse > EPSILON) {
            vitesse = std::max(vitesseArret, vitesse - 0.1);
        }

        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5); // Ralentir si nécessaire
        }

        // Déplacement du métro
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

        // Arrêt à la station
        if (doitSArreter && !enArret) {
            arreterAStation();
        }
    }
}

// Arrêter le métro à une station
void Metro::arreterAStation() {
    if (!enArret) {
        vitesse = vitesseArret;
        enArret = true;
        stationClock.restart();

        // Simulation de l'arrêt à la station
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dureeArret * 1000)));
        std::cout << "Metro " << id << " s'arrête à la station." << std::endl;
    }
}

// Gérer les passagers montant et descendant
void Metro::gererPassagers(int passagersQuai) {
    // Facteur de variabilité basé sur l'ID du métro et une valeur aléatoire
    int variabilite = (rand() % 20 + 1) * (id % 15);

    // Calcul du nombre de passagers sortants avec variabilité
    passagersSortants = std::min(nombrePassagers, rand() % (nombrePassagers + 1) + variabilite / 20);

    // Calcul du nombre de passagers entrants avec variabilité
    passagersEntrants = std::min(capaciteMaximale - nombrePassagers, passagersQuai) + variabilite / 25;

    // Mise à jour du nombre de passagers à bord
    nombrePassagers += passagersEntrants - passagersSortants;

    // Assurer que le nombre de passagers reste dans les limites
    if (nombrePassagers > capaciteMaximale) nombrePassagers = capaciteMaximale;
    if (nombrePassagers < 0) nombrePassagers = 0;
}

// Arrêt total du métro
void Metro::arretTotal() {
    enArret = true;
    vitesse = vitesseArret;
    arretUrgence = true;
}

// Reprendre après un arrêt d'urgence
void Metro::reprendreAprèsArretUrgence() {
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
    // Formule hypothétique de transition
    return 2 * t * t * t - 3 * t * t + 1;
}

