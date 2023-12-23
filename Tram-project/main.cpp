

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <mutex>

class Metro {
public:
    double position;
    double distanceEntreStations;
    double vitesse;
    const double vitesseCroisiere = 50.0;
    const double vitesseArret = 0.0;
    const double dureeArret = 5.0;
    const int capaciteMaximale = 200;
    int nombrePassagers;
    sf::Clock stationClock;
    bool enArret;

    Metro() : position(0.0), vitesse(0.0), nombrePassagers(0), enArret(false), distanceEntreStations(0.0) {}

    void initialiser(double distanceEntreStations) {
        this->distanceEntreStations = distanceEntreStations;
    }

    void quitterStation() {
        accelerer(vitesseCroisiere);
        enArret = false;
    }

    void parcourirLigne(sf::Time elapsedTime) {
        if (!enArret) {
            double distanceRestante = distanceEntreStations - position;
            double tempsCroisiere = distanceRestante / vitesseCroisiere;
            double tempsTotal = std::min<double>(elapsedTime.asSeconds(), tempsCroisiere);
            double coefficient = fonctionTransition(tempsTotal / tempsCroisiere);
            vitesse = coefficient * vitesseCroisiere;

            // Déplacement du métro
            double newPosition = position + vitesse * elapsedTime.asSeconds();

            // Vérification de la fin de la ligne
            if (newPosition > distanceEntreStations) {
                // Passe sur la ligne inférieure et ajuste la position pour le trajet inverse
                newPosition -= distanceEntreStations;
                newPosition = distanceEntreStations - newPosition;
            }

            position = newPosition;
        }
    }


    void arreterAStation() {
        if (vitesse > 0.0 && !enArret) {
            decelerer(vitesseCroisiere);
            vitesse = 0.0;
            stationClock.restart();
            enArret = true;
        }
    }

    void gererPassagers(int passagersQuai) {
        int passagersSortent = rand() % (nombrePassagers + 1);
        if (passagersSortent > nombrePassagers) {
            passagersSortent = nombrePassagers;
        }
        nombrePassagers -= passagersSortent;
        int passagersEntrent = std::min(capaciteMaximale - nombrePassagers, passagersQuai);
        nombrePassagers += passagersEntrent;
    }

    double getPosition() const {
        return position;
    }

    int getCapaciteMaximale() const {
        return capaciteMaximale;
    }

    int getNombrePassagers() const {
        return nombrePassagers;
    }

    double getDureeArret() const {
        return dureeArret;
    }

private:
    void accelerer(double acceleration) {
        vitesse += acceleration;
    }

    void decelerer(double deceleration) {
        vitesse -= deceleration;
        if (vitesse < 0.0) {
            vitesse = 0.0;
        }
    }

    double fonctionTransition(double t) const {
        return 2 * t * t * t - 3 * t * t + 1;
    }
};



class Station {
public:
    std::string nom;
    double position;

    Station(const std::string& nom, double position) : nom(nom), position(position) {}

    int gererPassagers() const {
        return rand() % 150;
    }
};

void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro,
    const std::vector<Station>& stations, bool afficherDeuxiemeMetro) {
    window.clear(sf::Color(28, 48, 68));  // Nouvelle couleur d'arrière-plan bleu foncé

    sf::RectangleShape outerTrackRect;
    outerTrackRect.setSize(sf::Vector2f(stations.back().position + 200, 40)); // Ajustez la largeur selon vos besoins
    outerTrackRect.setFillColor(sf::Color::Transparent);
    outerTrackRect.setOutlineColor(sf::Color::White);
    outerTrackRect.setOutlineThickness(20); // Ajustez la largeur du bord selon vos besoins
    outerTrackRect.setPosition(0, window.getSize().y / 2 - 20); // Ajustez la position verticale selon vos besoins

    window.draw(outerTrackRect);
   

    // Dessiner les stations
    for (const auto& station : stations) {
        sf::CircleShape stationShape(20);
        stationShape.setFillColor(sf::Color(50, 205, 50));  // Nouvelle couleur verte pour les stations
        stationShape.setPosition(static_cast<float>(station.position - 20), window.getSize().y / 2 - 20);
        window.draw(stationShape);

        sf::Text stationText;
        stationText.setString(station.nom);
        stationText.setCharacterSize(16);
        stationText.setFillColor(sf::Color::White);
        stationText.setPosition(static_cast<float>(station.position - 10), window.getSize().y / 2 - 25);
        window.draw(stationText);
    }

    // Dessiner le métro
    sf::RectangleShape metroShape(sf::Vector2f(40, 20));
    metroShape.setFillColor(sf::Color::Blue);
    metroShape.setPosition(static_cast<float>(metro.getPosition() - 20), window.getSize().y / 2 - 40); // Ajustez la position verticale
    window.draw(metroShape);

    if (afficherDeuxiemeMetro) {
        // Dessiner le deuxième métro
        sf::RectangleShape deuxiemeMetroShape(sf::Vector2f(40, 20));
        deuxiemeMetroShape.setFillColor(sf::Color::Red);
        deuxiemeMetroShape.setPosition(static_cast<float>(deuxiemeMetro.getPosition() - 20), window.getSize().y / 2 - 40); // Ajustez la position verticale
        window.draw(deuxiemeMetroShape);
    }

	
    // Afficher les informations    
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10);

    infoText.setString(
        "Capacite maximale du métro : " + std::to_string(metro.getCapaciteMaximale()) +
        " | Nombre de passagers à bord : " + std::to_string(metro.getNombrePassagers()) +
        (afficherDeuxiemeMetro ? ("\nCapacite maximale du 2eme métro : " + std::to_string(deuxiemeMetro.getCapaciteMaximale()) +
            " | Nombre de passagers à bord du 2eme métro : " + std::to_string(deuxiemeMetro.getNombrePassagers())) : "")
    );

    window.draw(infoText);

    window.display();
}   

bool deuxiemeMetroPret = false;
std::mutex mutex;

// Modifier la fonction simulerMetro pour passer la direction au métro
void simulerMetro(Metro& metro, const std::vector<Station>& stations, double distanceEntreMetros, bool premierMetro) {
    size_t stationIndex = 0;

    if (!premierMetro) {
        // Attendre 10 secondes avant que le deuxième métro ne commence à bouger
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    while (true) {
        if (metro.getPosition() >= stations[stationIndex].position || (distanceEntreMetros > 0 && metro.getPosition() > distanceEntreMetros && !deuxiemeMetroPret)) {
            metro.parcourirLigne(sf::seconds(0));
            metro.arreterAStation();

            sf::Clock stationClock;
            while (stationClock.getElapsedTime().asSeconds() < metro.getDureeArret()) {
                // Attendre à la station sans bloquer la boucle principale
            }

            metro.gererPassagers(stations[stationIndex].gererPassagers());

            stationIndex = (stationIndex + 1) % stations.size();
        }

        sf::Time elapsedTime = sf::milliseconds(16);
        metro.quitterStation();
        metro.parcourirLigne(elapsedTime);

        if (distanceEntreMetros > 0 && metro.getPosition() > distanceEntreMetros && !deuxiemeMetroPret) {
            std::lock_guard<std::mutex> lock(mutex);
            deuxiemeMetroPret = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
int main() {
    srand(static_cast<unsigned int>(time(0)));

    Metro metro;
    metro.initialiser(1500);

    Metro deuxiemeMetro;
    deuxiemeMetro.initialiser(1500);

    std::vector<Station> stations = {
        Station("Saint-Philibert", 100),
        Station("Bourg", 300),
        Station("Maison des Enfants", 500),
        Station("Mitterie", 700),
        Station("Pont Supérieur", 900),
        Station("Lomme - Lambersart - Arthur-Notebart", 1100),
        Station("Canteleu", 1300),
        Station("Bois Blancs", 1500),
    };

    double distanceEntreMetros = 200;

    deuxiemeMetro.initialiser(1500 + distanceEntreMetros);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Metro Simulator");

    std::thread metroThread(simulerMetro, std::ref(metro), std::cref(stations), 0.0, true);
    std::thread deuxiemeMetroThread(simulerMetro, std::ref(deuxiemeMetro), std::cref(stations), distanceEntreMetros, false);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        afficherMetro(window, metro, deuxiemeMetro, stations, true);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    metroThread.join();
    deuxiemeMetroThread.join();

    return 0;
}