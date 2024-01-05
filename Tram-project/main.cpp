#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"
#include "fonction-necessaire.hpp"


//On définit aussi des constantes dont on va avoir besoin plus tard 
const double EPSILON = 1e-5;






int main() {
    srand(static_cast<unsigned int>(time(0)));

    // On initialise les 2 metros
    Metro metro;
    metro.initialiser(1700);

    Metro deuxiemeMetro;
    deuxiemeMetro.initialiser(1700);

    //On definit l'arret d'urgence à faux
    bool arretUrgence = false;

    // On creer notre tableau de station grace à vector
    std::vector<Station> stations = {
        Station("Saint-Philibert", 100),
        Station("Bourg", 300),
        Station("Maison des Enfants", 500),
        Station("Mitterie", 700),
        Station("Pont Supérieur", 900),
        Station("Lomme - Lambersart", 1100),
        Station("Canteleu", 1300),
        Station("Bois Blancs", 1500),
        Station("Port de Lille", 1700),
    };



    // On crée l'horloge et le temps total d'arrêt
    sf::Clock clock;
    sf::Time tempsTotalArrete;

    // On crée une fenêtre avec la taille de l'écran de l'ordinateur pour l'interface du simulateur de métro.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Metro Lille"); 
    // On initialise le superviseur avec les deux métros et la fenêtre créée.
    Superviseur superviseur(metro, deuxiemeMetro, window);

    // Redémarrage de l'horloge pour le superviseur.
    superviseur.demarrageClock.restart();

    // On lance les deux threads pour simuler les deux métros en parallèle
    std::thread metroThread1(simulerMetro, std::ref(metro), std::cref(stations), false, std::ref(superviseur), std::ref(tempsTotalArrete));
    std::thread metroThread2(simulerMetro, std::ref(deuxiemeMetro), std::cref(stations), true, std::ref(superviseur), std::ref(tempsTotalArrete));


    // Et on fait démarrer les deux métros. 
    metro.quitterStation();
    deuxiemeMetro.quitterStation();

    // On indique le début de la simulation.
    bool simulationDebut = true;

    // On ouvre notre fenetre SFML
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // On appelle le  superviseur pour qu'il vérifie et réagit aux événements, comme les clics de souris.
            superviseur.verifierEtReagir(event, clock, tempsTotalArrete);


        }
        // On récupère le temps écoulé depuis le dernier appel
        sf::Time elapsedTime = clock.restart(); 


        // On crée alors une boucle pour gérer les interactions avec les stations.
        for (auto& station : stations) {

            // On prend/ obtiens les indices des prochaines stations pour chaque métro.
            size_t stationIndexMetro = getProchaineStationIndex(metro, stations);
            size_t stationIndexDeuxiemeMetro = getProchaineStationIndex(deuxiemeMetro, stations);

            // On vérifie si le métro est à une station
            if (metro.estALaStation()) {
                stations[stationIndexMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexMetro].signalerDepartMetro();
            }

            // On vérifie si le deuxième métro est à une station
            if (deuxiemeMetro.estALaStation()) {
                stations[stationIndexDeuxiemeMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexDeuxiemeMetro].signalerDepartMetro();
            }
        }

        // Le superviseur vérifie la distance de sécurité entre les deux métros.
        superviseur.verifierDistanceSecurite(metro, deuxiemeMetro);
        
        // Et On met à jour les positions des métros.
        superviseur.mettreAJourLesPositions(metro, deuxiemeMetro, elapsedTime, stations, simulationDebut);


        // sinon on affiche l'affichage des métros si l'arrêt d'urgence n'est pas activé.
        if (!superviseur.getArretUrgence()) {
            afficherMetro(window, metro, deuxiemeMetro, stations, true, superviseur);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    metroThread1.join();
    metroThread2.join();

    return 0;
}