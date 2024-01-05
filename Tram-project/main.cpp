#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"
#include "fonction-necessaire.hpp"


//On d�finit aussi des constantes dont on va avoir besoin plus tard 
const double EPSILON = 1e-5;






int main() {
    srand(static_cast<unsigned int>(time(0)));

    // On initialise les 2 metros
    Metro metro;
    metro.initialiser(1700);

    Metro deuxiemeMetro;
    deuxiemeMetro.initialiser(1700);

    //On definit l'arret d'urgence � faux
    bool arretUrgence = false;

    // On creer notre tableau de station grace � vector
    std::vector<Station> stations = {
        Station("Saint-Philibert", 100),
        Station("Bourg", 300),
        Station("Maison des Enfants", 500),
        Station("Mitterie", 700),
        Station("Pont Sup�rieur", 900),
        Station("Lomme - Lambersart", 1100),
        Station("Canteleu", 1300),
        Station("Bois Blancs", 1500),
        Station("Port de Lille", 1700),
    };



    // On cr�e l'horloge et le temps total d'arr�t
    sf::Clock clock;
    sf::Time tempsTotalArrete;

    // On cr�e une fen�tre avec la taille de l'�cran de l'ordinateur pour l'interface du simulateur de m�tro.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Metro Lille"); 
    // On initialise le superviseur avec les deux m�tros et la fen�tre cr��e.
    Superviseur superviseur(metro, deuxiemeMetro, window);

    // Red�marrage de l'horloge pour le superviseur.
    superviseur.demarrageClock.restart();

    // On lance les deux threads pour simuler les deux m�tros en parall�le
    std::thread metroThread1(simulerMetro, std::ref(metro), std::cref(stations), false, std::ref(superviseur), std::ref(tempsTotalArrete));
    std::thread metroThread2(simulerMetro, std::ref(deuxiemeMetro), std::cref(stations), true, std::ref(superviseur), std::ref(tempsTotalArrete));


    // Et on fait d�marrer les deux m�tros. 
    metro.quitterStation();
    deuxiemeMetro.quitterStation();

    // On indique le d�but de la simulation.
    bool simulationDebut = true;

    // On ouvre notre fenetre SFML
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // On appelle le  superviseur pour qu'il v�rifie et r�agit aux �v�nements, comme les clics de souris.
            superviseur.verifierEtReagir(event, clock, tempsTotalArrete);


        }
        // On r�cup�re le temps �coul� depuis le dernier appel
        sf::Time elapsedTime = clock.restart(); 


        // On cr�e alors une boucle pour g�rer les interactions avec les stations.
        for (auto& station : stations) {

            // On prend/ obtiens les indices des prochaines stations pour chaque m�tro.
            size_t stationIndexMetro = getProchaineStationIndex(metro, stations);
            size_t stationIndexDeuxiemeMetro = getProchaineStationIndex(deuxiemeMetro, stations);

            // On v�rifie si le m�tro est � une station
            if (metro.estALaStation()) {
                stations[stationIndexMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexMetro].signalerDepartMetro();
            }

            // On v�rifie si le deuxi�me m�tro est � une station
            if (deuxiemeMetro.estALaStation()) {
                stations[stationIndexDeuxiemeMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexDeuxiemeMetro].signalerDepartMetro();
            }
        }

        // Le superviseur v�rifie la distance de s�curit� entre les deux m�tros.
        superviseur.verifierDistanceSecurite(metro, deuxiemeMetro);
        
        // Et On met � jour les positions des m�tros.
        superviseur.mettreAJourLesPositions(metro, deuxiemeMetro, elapsedTime, stations, simulationDebut);


        // sinon on affiche l'affichage des m�tros si l'arr�t d'urgence n'est pas activ�.
        if (!superviseur.getArretUrgence()) {
            afficherMetro(window, metro, deuxiemeMetro, stations, true, superviseur);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    metroThread1.join();
    metroThread2.join();

    return 0;
}