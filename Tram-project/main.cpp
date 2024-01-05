#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"

constexpr const char* _PATH_IMG_ = "C:/Program Files/SFML/img/";

const double EPSILON = 1e-5;
const double SEUIL_DECÉLÉRATION = 50.0;





size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations) {
    size_t stationIndex = 0;

    if (metro.directionPositive) {
        // Lorsque le métro se déplace vers la droite, trouvez la prochaine station devant lui.
        // Commencez par la première station et déplacez-vous vers la droite.
        for (size_t i = 0; i < stations.size() - 1; ++i) {
            if (metro.getPosition() >= stations[i].position && metro.getPosition() < stations[i + 1].position) {
                stationIndex = i;
                break;
            }
        }
    }
    else {
        // Lorsque le métro se déplace vers la gauche, trouvez la prochaine station derrière lui.
        // Commencez par la dernière station et déplacez-vous vers la gauche.
        for (size_t i = stations.size() - 1; i > 0; --i) {
            if (metro.getPosition() <= stations[i].position && metro.getPosition() > stations[i - 1].position) {
                stationIndex = i;
                break;
            }
        }
    }

    // Si le métro est à la dernière station et se déplace vers la droite, il est en train de tourner.
    if (metro.directionPositive && metro.getPosition() >= stations.back().position) {
        stationIndex = stations.size() - 1;
    }

    // Si le métro est à la première station et se déplace vers la gauche, il est en train de tourner.
    if (!metro.directionPositive && metro.getPosition() <= stations.front().position) {
        stationIndex = 0;
    }

    return stationIndex;
}


void drawTextWithBackground(sf::RenderWindow& window, sf::Text& text, sf::FloatRect& bounds, sf::Color topColor, sf::Color bottomColor) {
    sf::RectangleShape backgroundRect;
    backgroundRect.setSize(sf::Vector2f(bounds.width, bounds.height + 10)); // +10 pour un peu d'espace supplémentaire
    backgroundRect.setPosition(text.getPosition().x, text.getPosition().y);

    // Créez le dégradé en utilisant sf::VertexArray
    sf::VertexArray gradient(sf::Quads, 4);

    // Configurez le dégradé pour qu'il corresponde à la taille et à la position de backgroundRect
    gradient[0].position = backgroundRect.getPosition();
    gradient[1].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y);
    gradient[2].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);
    gradient[3].position = sf::Vector2f(backgroundRect.getPosition().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);



    // Définissez les couleurs des coins pour le dégradé
    gradient[0].color = topColor;
    gradient[1].color = topColor;
    gradient[2].color = bottomColor;
    gradient[3].color = bottomColor;

    // Dessinez le dégradé
    window.draw(gradient);

    // Dessinez le texte
    window.draw(text);
}

void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur) {
    window.clear(sf::Color(28, 48, 68));

    sf::RectangleShape outerTrackRect;
    outerTrackRect.setSize(sf::Vector2f(static_cast<float>(stations.back().position + 20), 40));
    outerTrackRect.setFillColor(sf::Color::Transparent);
    outerTrackRect.setOutlineColor(sf::Color(170, 140, 100));
    outerTrackRect.setOutlineThickness(20);
    outerTrackRect.setPosition(0.0f, static_cast<float>(window.getSize().y) / 2.0f - 20.0f);
    window.draw(outerTrackRect);


    sf::Font fontStation;
    if (!fontStation.loadFromFile("C:/Windows/Fonts/BAUHS93.TTF")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

    sf::Texture stationTexture;
    if (!stationTexture.loadFromFile(std::string(_PATH_IMG_) + "Logo_Metro.png")) {
        std::cerr << "Erreur lors du chargement de l'image de la station" << std::endl;
        return;
    }

    // Pour chaque station, dessinez la station et son nom
    for (const auto& station : stations) {
        sf::Sprite stationSprite;
        stationSprite.setTexture(stationTexture);

        // Ajustez la taille si nécessaire
        stationSprite.setScale(0.06f, 0.06f); // Ajustez cette échelle selon la taille de votre image et la taille souhaitée sur l'écran

        // Centrez le sprite sur l'emplacement de la station
        sf::Vector2u textureSize = stationTexture.getSize();
        stationSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
        stationSprite.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f);

        window.draw(stationSprite);

        // Créer un objet Text pour le nom de la station
        sf::Text stationText;
        stationText.setFont(fontStation);
        stationText.setString(station.nom);
        stationText.setCharacterSize(16);
        stationText.setFillColor(sf::Color::White);

        // Positionner le texte au-dessus de la station
        sf::FloatRect textRect = stationText.getLocalBounds();
        stationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        stationText.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f - 60.0f);

        window.draw(stationText);
    }

    // Position verticale pour la ligne du haut et du bas
    const float ligneHaut = static_cast<float>(window.getSize().y) / 2.0f - 40.0f;
    const float ligneBas = static_cast<float>(window.getSize().y) / 2 + 20;






    sf::Texture metroTexture;
    if (!metroTexture.loadFromFile(std::string(_PATH_IMG_) + "metro-test.png")) {
        std::cerr << "Erreur lors du chargement de l'image du métro" << std::endl;
        // Gérez l'erreur comme il se doit - peut-être fermer le programme ou utiliser une image par défaut
    }

    // Utilisation de la texture pour créer un sprite
    sf::Sprite metroSprite;
    metroSprite.setTexture(metroTexture);

    // Ajustez la taille si nécessaire
    // Par exemple, si l'image est trop grande, vous pouvez la réduire
    metroSprite.setScale(0.1f, 0.1f); // Ajustez cette échelle selon la taille de votre image et la taille souhaitée sur l'écran

    // Positionnez le sprite du métro selon la position du métro dans le jeu
    float metroPosY = metro.directionPositive ? ligneHaut : ligneBas; // Assurez-vous que ligneHaut et ligneBas sont définis
    metroSprite.setPosition(static_cast<float>(metro.getPosition() - 20), metroPosY);

    // Dessinez le sprite du métro
    window.draw(metroSprite);



    sf::Texture metroTexture2;
    if (!metroTexture2.loadFromFile(std::string(_PATH_IMG_) + "metro1.png")) {
        std::cerr << "Erreur lors du chargement de l'image du deuxième métro" << std::endl;
        // Gérez l'erreur comme il se doit - peut-être fermer le programme ou utiliser une image par défaut
    }


    if (afficherDeuxiemeMetro) {
        sf::Sprite deuxiemeMetroSprite;
        deuxiemeMetroSprite.setTexture(metroTexture2);
        // Ajustez la taille si nécessaire
        deuxiemeMetroSprite.setScale(0.2f, 0.3f); // Ajustez cette échelle selon la taille de votre image et la taille souhaitée sur l'écran

        // Positionnez le sprite du deuxième métro selon la position du métro dans le jeu
        float deuxiemeMetroPosY = deuxiemeMetro.directionPositive ? ligneHaut : ligneBas;
        deuxiemeMetroSprite.setPosition(static_cast<float>(deuxiemeMetro.getPosition() - 20), deuxiemeMetroPosY);

        // Dessinez le sprite du deuxième métro
        window.draw(deuxiemeMetroSprite);
    }

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }


    // Configurez le premier bloc de texte
    sf::Text textMetro1;
    textMetro1.setFont(font);
    textMetro1.setCharacterSize(20);
    textMetro1.setFillColor(sf::Color::White);
    textMetro1.setString(std::string("               Métro 1 (ID: ") + std::to_string(metro.getID()) + ")" +
        "\n\n --> Capacité maximale : " + std::to_string(metro.getCapaciteMaximale()) +
        "\n\n | Entrants : " + std::to_string(metro.getPassagersEntrants()) +
        "\n | Sortants : " + std::to_string(metro.getPassagersSortants()) +
        "\n | Passagers à bord : " + std::to_string(metro.getNombrePassagers()) +
        "\n | Places disponibles : " + std::to_string(metro.getCapaciteMaximale() - metro.getNombrePassagers()) +
        "\n | Vitesse : " + std::to_string(metro.getVitesse()) + " m/s" +
        "\n\n | Station actuelle : " + (metro.estALaStation() ? stations[getProchaineStationIndex(metro, stations)].nom : "En déplacement"));

    // Ajoutez le nombre de passagers sur le quai si le nombre de passagers à bord est supérieur à 200
    if (metro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(metro, stations)].gererPassagers()));
    }

    textMetro1.setPosition(350, 50); // Position de départ du premier bloc de texte


    // Configuration du deuxième bloc de texte pour le Métro 2
    sf::Text textMetro2;
    textMetro2.setFont(font);
    textMetro2.setCharacterSize(20);
    textMetro2.setFillColor(sf::Color::White);
    textMetro2.setString(std::string("               Métro 2 (ID: ") + std::to_string(deuxiemeMetro.getID()) + ")" +
        "\n\n --> Capacité maximale : " + std::to_string(deuxiemeMetro.getCapaciteMaximale()) +
        "\n\n | Entrants : " + std::to_string(deuxiemeMetro.getPassagersEntrants()) +
        "\n | Sortants : " + std::to_string(deuxiemeMetro.getPassagersSortants()) +
        "\n | Passagers à bord : " + std::to_string(deuxiemeMetro.getNombrePassagers()) +
        "\n | Places disponibles : " + std::to_string(deuxiemeMetro.getCapaciteMaximale() - deuxiemeMetro.getNombrePassagers()) +
        "\n | Vitesse : " + std::to_string(deuxiemeMetro.getVitesse()) + " m/s" +
        "\n\n | Station actuelle : " + (deuxiemeMetro.estALaStation() ? stations[getProchaineStationIndex(deuxiemeMetro, stations)].nom : "En déplacement"));

    // Ajoutez le nombre de passagers sur le quai si le nombre de passagers à bord est supérieur à 200
    if (deuxiemeMetro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(deuxiemeMetro, stations)].gererPassagers()));
    }

    textMetro2.setPosition(1050, 50);

    // Dessinez les dégradés et les textes
    sf::FloatRect boundsMetro1 = textMetro1.getLocalBounds();
    sf::FloatRect boundsMetro2 = textMetro2.getLocalBounds();

    drawTextWithBackground(window, textMetro1, boundsMetro1, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));
    drawTextWithBackground(window, textMetro2, boundsMetro2, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));

    superviseur.boutonArret.setSize(sf::Vector2f(100, 50));
    superviseur.boutonArret.setPosition(850, 800); // Position du bouton
    superviseur.boutonArret.setFillColor(sf::Color::Red);

    sf::Font font_stop;
    if (!font_stop.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

    superviseur.texteBouton.setFont(font_stop); // Assurez-vous que la police est chargée
    superviseur.texteBouton.setString("STOP");
    superviseur.texteBouton.setCharacterSize(24);
    superviseur.texteBouton.setFillColor(sf::Color::White);

    // Calculer la position pour centrer le texte sur le bouton
    sf::FloatRect textRect = superviseur.texteBouton.getLocalBounds();
    superviseur.texteBouton.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    superviseur.texteBouton.setPosition(850 + 50, 800 + 25); // Ajustez en fonction de la taille du bouton


    window.draw(superviseur.boutonArret);
    window.draw(superviseur.texteBouton);

    window.display();
}



void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete) {
    if (avecDelai) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
    }

    sf::Clock clock;
    sf::Time tempsArrete;
    bool estArrete = false;
    size_t stationIndex = 0;
    bool aller = true;
    bool simulationDebut = true;

    while (true) {
 
        if (!superviseur.getArretUrgence()) {
            sf::Time tempsTotalEcoulé = clock.getElapsedTime() - tempsTotalArrete;

            if (estArrete) {
                tempsArrete += clock.restart();
                estArrete = false;
            }

            if (!metro.enArret) {
                sf::Time tempsEcoulé = clock.getElapsedTime();


                metro.parcourirLigne(tempsEcoulé, stations, static_cast<double>(stationIndex), superviseur);

                double nextStationPos = stations[stationIndex].position;
                if ((aller && metro.getPosition() >= nextStationPos) ||
                    (!aller && metro.getPosition() <= nextStationPos)) {
				
                    metro.arreterAStation();
                    if ((stationIndex == stations.size() - 1 && aller) ||
                        (stationIndex == 0 && !aller)) {
                        metro.passagersSortants = metro.nombrePassagers;
                        metro.nombrePassagers = 0;
                        metro.passagersEntrants = 0;
                        std::cout << "Terminus, tout le monde descend !\n";

                        aller = !aller;
                        stationIndex = aller ? 0 : stations.size() - 1;
                        clock.restart();
                        tempsArrete = sf::Time::Zero;
                    }
                    else {
                        metro.gererPassagers(stations[stationIndex].gererPassagers());
                        stationIndex += (aller) ? 1 : -1;

                    }
                    metro.directionPositive = aller;
                }
            }

            if (metro.enArret && tempsTotalEcoulé.asSeconds() - tempsArrete.asSeconds() >= metro.getDureeArret()) {
                metro.quitterStation();
                clock.restart();
                tempsArrete = sf::Time::Zero;
            }
        }
        else {
            if (!estArrete) {
                tempsArrete = clock.getElapsedTime();
                estArrete = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}



double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations) {
    size_t stationIndex = getProchaineStationIndex(metro, stations);
    return stations[stationIndex].position;
}



int main() {
    srand(static_cast<unsigned int>(time(0)));

    Metro metro;
    metro.initialiser(1700);

    Metro deuxiemeMetro;
    deuxiemeMetro.initialiser(1700);

    bool arretUrgence = false;


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



    // Création de l'horloge et du temps total d'arrêt
    sf::Clock clock;
    sf::Time tempsTotalArrete;



    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Metro Lille");
    Superviseur superviseur(metro, deuxiemeMetro, window);

    superviseur.demarrageClock.restart();




    std::thread metroThread1(simulerMetro, std::ref(metro), std::cref(stations), false, std::ref(superviseur), std::ref(tempsTotalArrete));
    std::thread metroThread2(simulerMetro, std::ref(deuxiemeMetro), std::cref(stations), true, std::ref(superviseur), std::ref(tempsTotalArrete));



    metro.quitterStation();
    deuxiemeMetro.quitterStation();

    bool simulationDebut = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            superviseur.verifierEtReagir(event, clock, tempsTotalArrete);


        }
        sf::Time elapsedTime = clock.restart(); // Récupère le temps écoulé depuis le dernier appel

        for (auto& station : stations) {

            size_t stationIndexMetro = getProchaineStationIndex(metro, stations);
            size_t stationIndexDeuxiemeMetro = getProchaineStationIndex(deuxiemeMetro, stations);

            // Vérifier si le métro est à une station
            if (metro.estALaStation()) {
                stations[stationIndexMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexMetro].signalerDepartMetro();
            }

            // Vérifier si le deuxième métro est à une station
            if (deuxiemeMetro.estALaStation()) {
                stations[stationIndexDeuxiemeMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexDeuxiemeMetro].signalerDepartMetro();
            }
        }

        superviseur.verifierDistanceSecurite(metro, deuxiemeMetro);
        superviseur.mettreAJourLesPositions(metro, deuxiemeMetro, elapsedTime, stations, simulationDebut);



        if (!superviseur.getArretUrgence()) {
            afficherMetro(window, metro, deuxiemeMetro, stations, true, superviseur);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    metroThread1.join();
    metroThread2.join();

    return 0;
}