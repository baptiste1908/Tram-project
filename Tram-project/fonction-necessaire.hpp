


#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


// On définit un chemin pour les images: ici nous avons choisi de le mettre dans programme / SFML / img
constexpr const char* _PATH_IMG_ = "C:/Program Files/SFML/img/";



// On va créer une fonction qui va nous permettre de récupérer l'index de la prochaine station
size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations) {
    size_t stationIndex = 0;

    if (metro.directionPositive) {
		
        // Lorsque le métro se déplace vers la droite, trouvez la prochaine station devant lui. On va commencer par la première station et ce déplacer vers la droite.
        for (size_t i = 0; i < stations.size() - 1; ++i) {
            if (metro.getPosition() >= stations[i].position && metro.getPosition() < stations[i + 1].position) {
                stationIndex = i;
                break;
            }
        }
    }
    else {
		
		// Lorsque le métro se déplace vers la gauche, trouvez la prochaine station derrière lui. On va commencer par la dernière station et ce déplacer vers la gauche.
        for (size_t i = stations.size() - 1; i > 0; --i) {
            if (metro.getPosition() <= stations[i].position && metro.getPosition() > stations[i - 1].position) {
                stationIndex = i;
                break;
            }
        }
    }

    // Ensuite si le métro est à la dernière station et se déplace vers la droite alors il est en train de tourner.
    if (metro.directionPositive && metro.getPosition() >= stations.back().position) {
        stationIndex = stations.size() - 1;
    }

    //Et si le métro est à la première station et se déplace vers la gauche alors il est en train de tourner.
    if (!metro.directionPositive && metro.getPosition() <= stations.front().position) {
        stationIndex = 0;
    }

	// et bien sur on renvoie l'index de la prochaine station
    return stationIndex;
}






// On va ensuite créer une fonction qui va nous permettre de récupérer cette fois-ci la position de la prochaine station

double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations) {
	
    size_t stationIndex = getProchaineStationIndex(metro, stations);

    return stations[stationIndex].position;
}












// On va aussi créer une fonction pour la beauté de l'affichage, elle va nous afficher un rectangle avec un magnifique dégradé de couleur.
// On va par la suite afficher les informations des metro dedans

void drawTextWithBackground(sf::RenderWindow& window, sf::Text& text, sf::FloatRect& bounds, sf::Color topColor, sf::Color bottomColor) {
	
	// On va créer un rectangle qui va nous servir de fond
    sf::RectangleShape backgroundRect;
	
	// On va lui donner sa taille et sa position
    backgroundRect.setSize(sf::Vector2f(bounds.width, bounds.height + 10)); 
    backgroundRect.setPosition(text.getPosition().x, text.getPosition().y);

	// On va ensuite créer un dégradé de couleur
    sf::VertexArray gradient(sf::Quads, 4);

	// Le principe est assez simple : on va créer un quadrilatère qui va nous servir de dégradé de couleur
    gradient[0].position = backgroundRect.getPosition();
    gradient[1].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y);
    gradient[2].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);
    gradient[3].position = sf::Vector2f(backgroundRect.getPosition().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);


	// Et on va lui donner les couleurs que l'on veut en haut et en bas. 
    gradient[0].color = topColor;
    gradient[1].color = topColor;
    gradient[2].color = bottomColor;
    gradient[3].color = bottomColor;

	// Enfin on dessie le tout
    window.draw(gradient);

	// et on dessine le texte par dessus
    window.draw(text);
}












// Cette classe est assez longue et va regrouper pas mal de chose tel que l'affichage des stations, des métros, des informations sur les métros, etc...

void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur) {
    
	// On définit tout d'abord la couleur de notre fenêtre 
    window.clear(sf::Color(28, 48, 68));

	// La on va commencer a dessiner les rails en dessinant un rectangle mais pas plein pour faire un contour
    sf::RectangleShape outerTrackRect;
	// On va lui donner sa taille 
    outerTrackRect.setSize(sf::Vector2f(static_cast<float>(stations.back().position + 20), 40));
	// On va lui donner sa couleur
    outerTrackRect.setFillColor(sf::Color::Transparent);
    outerTrackRect.setOutlineColor(sf::Color(170, 140, 100));
	// On va lui donner son épaisseur
    outerTrackRect.setOutlineThickness(20);
	// On va lui donner sa position
    outerTrackRect.setPosition(0.0f, static_cast<float>(window.getSize().y) / 2.0f - 20.0f);
	// et enfin on dessine le tout
    window.draw(outerTrackRect);

	

	// On va ensuite charger l'image de la station et on affiche un message d'erreur si jamais on arrive pas a chargé l'image
    sf::Texture stationTexture;
    if (!stationTexture.loadFromFile(std::string(_PATH_IMG_) + "Logo_Metro.png")) {
        std::cerr << "Erreur lors du chargement de l'image de la station" << std::endl;
        return;
    }

    // On va ensuite faire en sorte que pour chaque station, on dessine la station et son nom
    for (const auto& station : stations) {
		
		// On va créer un sprite pour la station
        sf::Sprite stationSprite; 
		// On va lui donner la texture de la station
        stationSprite.setTexture(stationTexture);

         // On va lui donner une taille pas trop grande mais pas trop petite non plus
		stationSprite.setScale(0.06f, 0.06f);

        // On va récupérer la taille de la texture de la station
		sf::Vector2u textureSize = stationTexture.getSize(); 
		// On va lui donner son origine
		stationSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
		// On va aussi lui donner sa position
		stationSprite.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f); 
		//Et bien sur on dessine le tout
        window.draw(stationSprite);

		
        // on va ensuite charger une police d'ecriture pour afficher le nom des stations et bien sur on affiche un message d'erreur si jamais on arrive pas a chargé la police
        sf::Font fontStation;
        if (!fontStation.loadFromFile("C:/Windows/Fonts/BAUHS93.TTF")) {
            std::cerr << "Erreur lors du chargement de la police" << std::endl;
            return;
        }

		
        //On va ensuite creer un Texte pour le nom de la station 
        sf::Text stationText;
		// On va lui donner sa police
		stationText.setFont(fontStation); 
		// On va lui donner le nom de la station
		stationText.setString(station.nom); 
		// On va lui donner sa taille 
		stationText.setCharacterSize(16); 
		// On va lui donner sa couleur 
		stationText.setFillColor(sf::Color::White); 

        // On va ensuite positionner le texte au-dessus de la station
		sf::FloatRect textRect = stationText.getLocalBounds(); // On va récupérer la taille du texte
        // Meme principe qu'avant on va lui donner son origine et On va lui donner sa position
		stationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f); 
		stationText.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f - 60.0f); 

		// Et bien sur on dessine le tout
        window.draw(stationText);
    }

 

	// On définit la position de la ligne du haut pour pouvoir cirlculer aprés
	const float ligneHaut = static_cast<float>(window.getSize().y) / 2.0f - 40.0f; 
	 // On définit la position de la ligne du bas
	const float ligneBas = static_cast<float>(window.getSize().y) / 2 + 20;





	// On va ensuite charger l'image du  premier métro et on affiche un message d'erreur si jamais on arrive pas a chargé l'image
    sf::Texture metroTexture;
    if (!metroTexture.loadFromFile(std::string(_PATH_IMG_) + "metro-test.png")) {
        std::cerr << "Erreur lors du chargement de l'image du métro" << std::endl;
		return;
    }

	// On créer donc un sprite pour notre 1er métro et lui assimilé son image
    sf::Sprite metroSprite;
    metroSprite.setTexture(metroTexture); 

    // On va lui donner une taille pas trop grande mais pas trop petite non plus
    metroSprite.setScale(0.1f, 0.1f); 

	// On va positionner notre metro grace au ligne du haut et du bas définit juste un peu plus haut
    float metroPosY = metro.directionPositive ? ligneHaut : ligneBas; 
    metroSprite.setPosition(static_cast<float>(metro.getPosition() - 20), metroPosY);

	// On va ensuite dessiner le métro
    window.draw(metroSprite);



	// Exactement même principe que pour le premier métro
    sf::Texture metroTexture2;
    if (!metroTexture2.loadFromFile(std::string(_PATH_IMG_) + "metro1.png")) {
        std::cerr << "Erreur lors du chargement de l'image du deuxième métro" << std::endl;
   
    }

    sf::Sprite deuxiemeMetroSprite;
    deuxiemeMetroSprite.setTexture(metroTexture2);
    deuxiemeMetroSprite.setScale(0.2f, 0.3f); 
    float deuxiemeMetroPosY = deuxiemeMetro.directionPositive ? ligneHaut : ligneBas;
    deuxiemeMetroSprite.setPosition(static_cast<float>(deuxiemeMetro.getPosition() - 20), deuxiemeMetroPosY);

    window.draw(deuxiemeMetroSprite);
   



    // on va ensuite charger une police d'ecriture pour affichertoutes nos imformations et bien sur on affiche un message d'erreur si jamais on arrive pas a chargé la police
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }


	// On va ensuite s'attacker a l'une des plus importantes partie de notre programme, l'affichage des informations
	// On va donc créer un texte pour afficher les informations du premier métro
    sf::Text textMetro1;
	textMetro1.setFont(font); // On va lui donner la police du dessus
	textMetro1.setCharacterSize(20); // On va lui donner sa taille
	textMetro1.setFillColor(sf::Color::White); // On va lui donner sa couleur

	// et on affiche toutes les informations necessaire : 
    textMetro1.setString(std::string("               Métro 1 (ID: ") + std::to_string(metro.getID()) + ")" +
        "\n\n --> Capacité maximale : " + std::to_string(metro.getCapaciteMaximale()) +
        "\n\n | Entrants : " + std::to_string(metro.getPassagersEntrants()) +
        "\n | Sortants : " + std::to_string(metro.getPassagersSortants()) +
        "\n | Passagers à bord : " + std::to_string(metro.getNombrePassagers()) +
        "\n | Places disponibles : " + std::to_string(metro.getCapaciteMaximale() - metro.getNombrePassagers()) +
        "\n | Vitesse : " + std::to_string(metro.getVitesse()) + " m/s" +
        "\n\n | Station actuelle : " + (metro.estALaStation() ? stations[getProchaineStationIndex(metro, stations)].nom : "En déplacement"));

    // On rajoute aussi une info du nombre de passagers restant sur le quai si le nombre de passagers à bord est supérieur à 200
    if (metro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(metro, stations)].gererPassagers()));
    }

	// et on définit la position du premier texte
    textMetro1.setPosition(350, 50); 


	// On refait la meme chose mais pour le deuxieme metro cette fois ci
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

    if (deuxiemeMetro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(deuxiemeMetro, stations)].gererPassagers()));
    }

    textMetro2.setPosition(1050, 50);

	
    // On récupère ensuite la limite des 2 textes
	sf::FloatRect boundsMetro1 = textMetro1.getLocalBounds(); 
	sf::FloatRect boundsMetro2 = textMetro2.getLocalBounds(); 

	// On appelle enfin que l'on a définit avant pour afficher les textes ainsi que leurs fonts de couleur.
    drawTextWithBackground(window, textMetro1, boundsMetro1, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));
    drawTextWithBackground(window, textMetro2, boundsMetro2, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));



	// On va ensuite créer le bouton d'arret d'urgence en définissant : sa taille, sa position et sa couleur (ici rouge)
    superviseur.boutonArret.setSize(sf::Vector2f(100, 50));
    superviseur.boutonArret.setPosition(850, 800); 
    superviseur.boutonArret.setFillColor(sf::Color::Red);

	// On va charge la polici pour le texte du bouton
    sf::Font font_stop;
    if (!font_stop.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

	// On va lui donner la police du dessus
	superviseur.texteBouton.setFont(font_stop);
	// On choisit ce qu'on veut afficher
    superviseur.texteBouton.setString("STOP");
	// On choisit la taille du texte
    superviseur.texteBouton.setCharacterSize(24);
	// On choisit la couleur du texte
    superviseur.texteBouton.setFillColor(sf::Color::White);

    // On va ensuite calculer la position pour centrer le texte sur le bouton
	sf::FloatRect textRect = superviseur.texteBouton.getLocalBounds(); // On récupère la limite du texte
	
	// et on definit l'origine et la position du texte
    superviseur.texteBouton.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    superviseur.texteBouton.setPosition(850 + 50, 800 + 25); // Ajustez en fonction de la taille du bouton

	// Enfin on affiche le bouton et son texte 
    window.draw(superviseur.boutonArret);
    window.draw(superviseur.texteBouton);

	// et on affiche TOUT !
    window.display();
}








// Finit les beaux affichages ! On va créer la fonction qui va gerer la simulation du metro
void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete) {

    // On crée un délai pour le 2eme metro
    if (avecDelai) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
    }

    sf::Clock clock; // On crée une horloge
    sf::Time tempsArrete; // On crée un temps d'arret
    bool estArrete = false; // On crée un booléen pour savoir si le metro est arreté ou non
    size_t stationIndex = 0; // On crée un index pour la station
    bool aller = true; // On crée un booléen pour savoir si le metro est dans le sens aller ou retour 
    bool simulationDebut = true; // On crée un booléen pour savoir si la simulation a commencé ou non

    while (true) {

        // On vérifie si l'arrêt d'urgence n'est pas activé.
        if (!superviseur.getArretUrgence()) {

            // et on calcule le temps total écoulé depuis le dernier arrêt.
            sf::Time tempsTotalEcoulé = clock.getElapsedTime() - tempsTotalArrete;

            // On vérifie si le métro était précédemment arrêté et on met à jour le temps d'arrêt.
            if (estArrete) {
                tempsArrete += clock.restart(); // On ajoute le temps depuis le dernier arrêt.
                estArrete = false; // Et on réinitialise l'état d'arrêt.
            }

            // Si le métro n'est pas en arrêt alors il continue son trajet.
            if (!metro.enArret) {
                sf::Time tempsEcoulé = clock.getElapsedTime(); // On obtient le temps écoulé.

                // Le métro parcourt la ligne avec le temps écoulé.
                metro.parcourirLigne(tempsEcoulé, stations, static_cast<double>(stationIndex), superviseur);
                // On dtermine la position de la prochaine station.
                double nextStationPos = stations[stationIndex].position;
                // On vérifie si le métro a atteint la prochaine station.
                if ((aller && metro.getPosition() >= nextStationPos) ||
                    (!aller && metro.getPosition() <= nextStationPos)) {

                    metro.arreterAStation(); // On arrête le métro à la station.

                    // Ensuite on vérifie si le métro a atteint le terminus.
                    if ((stationIndex == stations.size() - 1 && aller) ||
                        (stationIndex == 0 && !aller)) {
                        // On gère les passagers au terminus.
                        metro.passagersSortants = metro.nombrePassagers;
                        metro.nombrePassagers = 0;
                        metro.passagersEntrants = 0;
                        std::cout << "Terminus, tout le monde descend !\n";

                        aller = !aller; // On change alors la direction du métro.
                        stationIndex = aller ? 0 : stations.size() - 1; // On réinitialise l'index de la station.
                        clock.restart(); // On redémarre l'horloge.
                        tempsArrete = sf::Time::Zero; // On réinitialise le temps d'arrêt.
                    }
                    else {
                        // On gère les passagers à la station.
                        metro.gererPassagers(stations[stationIndex].gererPassagers());
                        stationIndex += (aller) ? 1 : -1; // On incrémente ou décrémente l'index de la station.
                    }
                    metro.directionPositive = aller; // On met à jour la direction du métro.
                }
            }

            // On vérifie si le métro est en arrêt.
            if (metro.enArret && tempsTotalEcoulé.asSeconds() - tempsArrete.asSeconds() >= metro.getDureeArret()) {
                metro.quitterStation(); // On fait quitter la station au métro.
                clock.restart(); // On redémarre l'horloge.
                tempsArrete = sf::Time::Zero; // On réinitialise le temps d'arrêt.
            }
        }
        else {
            // On gère le cas où l'arrêt d'urgence est activé.
            if (!estArrete) {
                tempsArrete = clock.getElapsedTime(); // On met à jour le temps d'arrêt.
                estArrete = true; // On met à jour l'état d'arrêt.
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause pour l'arrêt d'urgence.
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Temporisation pour la boucle.
    }
}



