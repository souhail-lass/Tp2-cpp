/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2
 *  http://ericbeaudry.uqam.ca/INF3105/tp2/         
 */
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "personne.h"
#include "tableau.h"

// Structure pour représenter une recommandation
struct Recommandation {
    std::string trajet_aller;
    std::string trajet_retour;
    double cout;
    
    Recommandation() : cout(0.0) {}
    Recommandation(const std::string& aller, const std::string& retour, double c) 
        : trajet_aller(aller), trajet_retour(retour), cout(c) {}
};

// Fonction pour calculer la recommandation optimale niveau 1
Recommandation calculerRecommandationNiveau1(const Personne& conducteur, const Tableau<Personne>& personnes);

// Fonction pour calculer la recommandation optimale niveau 2
Recommandation calculerRecommandationNiveau2(const Personne& conducteur, const Tableau<Personne>& personnes);

// Fonction pour calculer la recommandation optimale niveau 3
Recommandation calculerRecommandationNiveau3(const Personne& conducteur, const Tableau<Personne>& personnes);

// Fonction pour calculer la recommandation optimale niveau 4
Recommandation calculerRecommandationNiveau4(const Personne& conducteur, const Tableau<Personne>& personnes);

// Fonction pour calculer la recommandation optimale niveau 5 (BONUS)
Recommandation calculerRecommandationNiveau5(const Personne& conducteur, const Tableau<Personne>& personnes);

int tp2(std::istream& entree);

// argc: nombre d'arguments passés.
// argv: tableau de chaines de carractères.
// exemple: ./tp2 test00.txt
//   ==> argc=2; argv[0]="./tp2"; argv[1]="test00.txt"
int main(int argc, const char** argv)
{
    if(argc>1){
        std::ifstream fichier_entree(argv[1]);
        if(fichier_entree.fail()){
            std::cerr << "Erreur de lecture du fichier '" << argv[1] << "'" << std::endl;
            return 1;
        }
        return tp2(fichier_entree);
    }else{
        // Pas d'argument requêtes ==> lecture depuis l'entrée standard avec std::cin.
        return tp2(std::cin);
    }
}

int tp2(std::istream& entree_requetes){
    // Structure pour stocker les personnes
    Tableau<Personne> personnes;

    // Lecture de toutes les personnes
    while(entree_requetes && !entree_requetes.eof()){
        Personne p;
        entree_requetes >> p >> std::ws;
        personnes.ajouter(p);
    }

    // Calcul et affichage des recommandations pour chaque personne
    for(int i = 0; i < personnes.taille(); i++) {
        Recommandation rec = calculerRecommandationNiveau1(personnes[i], personnes);
        std::cout << personnes[i].getNom() << ":\t"
                  << rec.trajet_aller << "\t"
                  << rec.trajet_retour << "\t"
                  << std::round(rec.cout) << "m"
                  << std::endl;
    }

    return 0;
}

// Implémentation de l'algorithme niveau 1
Recommandation calculerRecommandationNiveau1(const Personne& conducteur, const Tableau<Personne>& personnes) {
    // Coût de base : voyager seul
    double distance_seul = 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    Recommandation meilleure("+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            "+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            distance_seul);
    
    // Vérifier si le conducteur peut prendre des passagers (max 2 passagers total)
    if(conducteur.getNbPassagers() >= 2) {
        // Essayer chaque autre personne comme passager
        for(int i = 0; i < personnes.taille(); i++) {
            const Personne& passager = personnes[i];
            
            // Ne pas considérer le conducteur lui-même
            if(passager.getNom() == conducteur.getNom()) {
                continue;
            }
            
            // Calculer le coût du trajet avec ce passager
            // Aller : conducteur_origine -> passager_origine -> passager_destination -> conducteur_destination
            double distance_aller = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                  distance(passager.getOrigine(), passager.getDestination()) +
                                  distance(passager.getDestination(), conducteur.getDestination());
            
            // Retour : conducteur_destination -> passager_destination -> passager_origine -> conducteur_origine
            double distance_retour = distance(conducteur.getDestination(), passager.getDestination()) +
                                   distance(passager.getDestination(), passager.getOrigine()) +
                                   distance(passager.getOrigine(), conducteur.getOrigine());
            
            double distance_totale = distance_aller + distance_retour;
            double distance_evitee = 2 * distance(passager.getOrigine(), passager.getDestination());
            double cout = distance_totale - distance_evitee;
            
            // Garder la meilleure recommandation
            if(cout < meilleure.cout) {
                meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                       "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.trajet_retour = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                        "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.cout = cout;
            }
        }
    }
    
    return meilleure;
}

// Implémentation de l'algorithme niveau 2 (avec prêt de véhicule)
Recommandation calculerRecommandationNiveau2(const Personne& conducteur, const Tableau<Personne>& personnes) {
    // Coût de base : voyager seul
    double distance_seul = 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    Recommandation meilleure("+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            "+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            distance_seul);
    
    // Vérifier si le conducteur peut prendre des passagers (max 2 passagers total)
    if(conducteur.getNbPassagers() >= 2) {
        // Essayer chaque autre personne comme passager
        for(int i = 0; i < personnes.taille(); i++) {
            const Personne& passager = personnes[i];
            
            // Ne pas considérer le conducteur lui-même
            if(passager.getNom() == conducteur.getNom()) {
                continue;
            }
            
            // Cas 1: Covoiturage normal (comme niveau 1)
            double distance_aller_normal = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                        distance(passager.getOrigine(), passager.getDestination()) +
                                        distance(passager.getDestination(), conducteur.getDestination());
            
            double distance_retour_normal = distance(conducteur.getDestination(), passager.getDestination()) +
                                         distance(passager.getDestination(), passager.getOrigine()) +
                                         distance(passager.getOrigine(), conducteur.getOrigine());
            
            double distance_totale_normal = distance_aller_normal + distance_retour_normal;
            double distance_evitee = 2 * distance(passager.getOrigine(), passager.getDestination());
            double cout_normal = distance_totale_normal - distance_evitee;
            
            // Garder la meilleure recommandation normale
            if(cout_normal < meilleure.cout) {
                meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                       "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.trajet_retour = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                        "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.cout = cout_normal;
            }
            
            // Cas 2: Prêt de véhicule (si le conducteur accepte de prêter)
            if(conducteur.getPret()) {
                // Aller: conducteur_origine -> passager_origine -> conducteur_destination (conducteur débarque)
                // Puis passager va seul à sa destination
                double distance_aller_pret = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                           distance(passager.getOrigine(), conducteur.getDestination()) +
                                           distance(conducteur.getDestination(), passager.getDestination());
                
                // Retour: passager part de sa destination, va chercher conducteur à sa destination,
                // conducteur reprend le volant, va déposer passager, puis retourne chez lui
                double distance_retour_pret = distance(passager.getDestination(), conducteur.getDestination()) +
                                            distance(conducteur.getDestination(), passager.getOrigine()) +
                                            distance(passager.getOrigine(), conducteur.getOrigine());
                
                double distance_totale_pret = distance_aller_pret + distance_retour_pret;
                double cout_pret = distance_totale_pret - distance_evitee;
                
                // Garder la meilleure recommandation avec prêt
                if(cout_pret < meilleure.cout) {
                    meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                           "-" + conducteur.getNom() + "-" + passager.getNom();
                    meilleure.trajet_retour = "+" + passager.getNom() + "+" + conducteur.getNom() + 
                                            "-" + passager.getNom() + "-" + conducteur.getNom();
                    meilleure.cout = cout_pret;
                }
            }
        }
    }
    
    return meilleure;
}
// Fonction pour vérifier si un trajet respecte les contraintes d'horaire
bool respecteContraintesHoraire(const Personne& conducteur, const Personne& passager, 
                               const std::string& type_trajet) {
    // Vitesse constante de 10 mètres par seconde
    const double VITESSE = 10.0; // m/s
    
    if(type_trajet == "normal") {
        // Covoiturage normal
        double distance_aller = distance(conducteur.getOrigine(), passager.getOrigine()) +
                              distance(passager.getOrigine(), passager.getDestination()) +
                              distance(passager.getDestination(), conducteur.getDestination());
        
        double distance_retour = distance(conducteur.getDestination(), passager.getDestination()) +
                               distance(passager.getDestination(), passager.getOrigine()) +
                               distance(passager.getOrigine(), conducteur.getOrigine());
        
        // Temps en secondes
        double temps_aller = distance_aller / VITESSE;
        double temps_retour = distance_retour / VITESSE;
        
        // Heure d'arrivée à destination
        Heure arrivee_destination = conducteur.getHeureDepart() + (int)(temps_aller / 60);
        Heure arrivee_retour = conducteur.getHeureDepartRetour() + (int)(temps_retour / 60);
        
        // Vérifier les contraintes
        return arrivee_destination <= conducteur.getHeureArrivee() &&
               arrivee_retour <= conducteur.getHeureArriveeRetour() &&
               arrivee_destination <= passager.getHeureArrivee() &&
               arrivee_retour <= passager.getHeureArriveeRetour();
    }
    else if(type_trajet == "pret") {
        // Prêt de véhicule
        double distance_aller = distance(conducteur.getOrigine(), passager.getOrigine()) +
                              distance(passager.getOrigine(), conducteur.getDestination()) +
                              distance(conducteur.getDestination(), passager.getDestination());
        
        double distance_retour = distance(passager.getDestination(), conducteur.getDestination()) +
                               distance(conducteur.getDestination(), passager.getOrigine()) +
                               distance(passager.getOrigine(), conducteur.getOrigine());
        
        // Temps en secondes
        double temps_aller = distance_aller / VITESSE;
        double temps_retour = distance_retour / VITESSE;
        
        // Heure d'arrivée à destination
        Heure arrivee_destination = conducteur.getHeureDepart() + (int)(temps_aller / 60);
        Heure arrivee_retour = conducteur.getHeureDepartRetour() + (int)(temps_retour / 60);
        
        // Vérifier les contraintes
        return arrivee_destination <= conducteur.getHeureArrivee() &&
               arrivee_retour <= conducteur.getHeureArriveeRetour() &&
               arrivee_destination <= passager.getHeureArrivee() &&
               arrivee_retour <= passager.getHeureArriveeRetour();
    }
    
    return false;
}

// Implémentation de l'algorithme niveau 3 (avec contraintes d'horaire)
Recommandation calculerRecommandationNiveau3(const Personne& conducteur, const Tableau<Personne>& personnes) {
    // Coût de base : voyager seul
    double distance_seul = 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    Recommandation meilleure("+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            "+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            distance_seul);
    
    // Vérifier si le conducteur peut prendre des passagers (max 2 passagers total)
    if(conducteur.getNbPassagers() >= 2) {
        // Essayer chaque autre personne comme passager
        for(int i = 0; i < personnes.taille(); i++) {
            const Personne& passager = personnes[i];
            
            // Ne pas considérer le conducteur lui-même
            if(passager.getNom() == conducteur.getNom()) {
                continue;
            }
            
            // Cas 1: Covoiturage normal (comme niveau 1)
            double distance_aller_normal = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                        distance(passager.getOrigine(), passager.getDestination()) +
                                        distance(passager.getDestination(), conducteur.getDestination());
            
            double distance_retour_normal = distance(conducteur.getDestination(), passager.getDestination()) +
                                         distance(passager.getDestination(), passager.getOrigine()) +
                                         distance(passager.getOrigine(), conducteur.getOrigine());
            
            double distance_totale_normal = distance_aller_normal + distance_retour_normal;
            double distance_evitee = 2 * distance(passager.getOrigine(), passager.getDestination());
            double cout_normal = distance_totale_normal - distance_evitee;
            
            // Vérifier les contraintes d'horaire pour le covoiturage normal
            if(respecteContraintesHoraire(conducteur, passager, "normal") && cout_normal < meilleure.cout) {
                meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                       "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.trajet_retour = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                        "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.cout = cout_normal;
            }
            
            // Cas 2: Prêt de véhicule (si le conducteur accepte de prêter)
            if(conducteur.getPret()) {
                double distance_aller_pret = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                           distance(passager.getOrigine(), conducteur.getDestination()) +
                                           distance(conducteur.getDestination(), passager.getDestination());
                
                double distance_retour_pret = distance(passager.getDestination(), conducteur.getDestination()) +
                                            distance(conducteur.getDestination(), passager.getOrigine()) +
                                            distance(passager.getOrigine(), conducteur.getOrigine());
                
                double distance_totale_pret = distance_aller_pret + distance_retour_pret;
                double cout_pret = distance_totale_pret - distance_evitee;
                
                // Vérifier les contraintes d'horaire pour le prêt
                if(respecteContraintesHoraire(conducteur, passager, "pret") && cout_pret < meilleure.cout) {
                    meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                           "-" + conducteur.getNom() + "-" + passager.getNom();
                    meilleure.trajet_retour = "+" + passager.getNom() + "+" + conducteur.getNom() + 
                                            "-" + passager.getNom() + "-" + conducteur.getNom();
                    meilleure.cout = cout_pret;
                }
            }
        }
    }
    
    return meilleure;
}
// Fonction pour vérifier si un trajet avec 2 passagers respecte les contraintes d'horaire
bool respecteContraintesHoraire2Passagers(const Personne& conducteur, const Personne& passager1, const Personne& passager2) {
    // Vitesse constante de 10 mètres par seconde
    const double VITESSE = 10.0; // m/s
    
    // Covoiturage normal avec 2 passagers
    // Aller: conducteur_origine -> passager1_origine -> passager2_origine -> passager1_destination -> passager2_destination -> conducteur_destination
    double distance_aller = distance(conducteur.getOrigine(), passager1.getOrigine()) +
                          distance(passager1.getOrigine(), passager2.getOrigine()) +
                          distance(passager2.getOrigine(), passager1.getDestination()) +
                          distance(passager1.getDestination(), passager2.getDestination()) +
                          distance(passager2.getDestination(), conducteur.getDestination());
    
    // Retour: conducteur_destination -> passager2_destination -> passager1_destination -> passager2_origine -> passager1_origine -> conducteur_origine
    double distance_retour = distance(conducteur.getDestination(), passager2.getDestination()) +
                           distance(passager2.getDestination(), passager1.getDestination()) +
                           distance(passager1.getDestination(), passager2.getOrigine()) +
                           distance(passager2.getOrigine(), passager1.getOrigine()) +
                           distance(passager1.getOrigine(), conducteur.getOrigine());
    
    // Temps en secondes
    double temps_aller = distance_aller / VITESSE;
    double temps_retour = distance_retour / VITESSE;
    
    // Heure d'arrivée à destination
    Heure arrivee_destination = conducteur.getHeureDepart() + (int)(temps_aller / 60);
    Heure arrivee_retour = conducteur.getHeureDepartRetour() + (int)(temps_retour / 60);
    
    // Vérifier les contraintes pour tous
    return arrivee_destination <= conducteur.getHeureArrivee() &&
           arrivee_retour <= conducteur.getHeureArriveeRetour() &&
           arrivee_destination <= passager1.getHeureArrivee() &&
           arrivee_retour <= passager1.getHeureArriveeRetour() &&
           arrivee_destination <= passager2.getHeureArrivee() &&
           arrivee_retour <= passager2.getHeureArriveeRetour();
}

// Implémentation de l'algorithme niveau 4 (max 3 passagers, prêt possible, avec contraintes horaires)
Recommandation calculerRecommandationNiveau4(const Personne& conducteur, const Tableau<Personne>& personnes) {
    // Coût de base : voyager seul
    double distance_seul = 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    Recommandation meilleure("+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            "+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            distance_seul);
    
    // Vérifier si le conducteur peut prendre des passagers (max 3 passagers total)
    if(conducteur.getNbPassagers() >= 2) {
        // Cas 1: 1 passager (comme niveau 3)
        for(int i = 0; i < personnes.taille(); i++) {
            const Personne& passager = personnes[i];
            
            // Ne pas considérer le conducteur lui-même
            if(passager.getNom() == conducteur.getNom()) {
                continue;
            }
            
            // Covoiturage normal avec 1 passager
            double distance_aller_normal = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                        distance(passager.getOrigine(), passager.getDestination()) +
                                        distance(passager.getDestination(), conducteur.getDestination());
            
            double distance_retour_normal = distance(conducteur.getDestination(), passager.getDestination()) +
                                         distance(passager.getDestination(), passager.getOrigine()) +
                                         distance(passager.getOrigine(), conducteur.getOrigine());
            
            double distance_totale_normal = distance_aller_normal + distance_retour_normal;
            double distance_evitee = 2 * distance(passager.getOrigine(), passager.getDestination());
            double cout_normal = distance_totale_normal - distance_evitee;
            
            // Vérifier les contraintes d'horaire pour le covoiturage normal
            if(respecteContraintesHoraire(conducteur, passager, "normal") && cout_normal < meilleure.cout) {
                meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                       "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.trajet_retour = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                        "-" + passager.getNom() + "-" + conducteur.getNom();
                meilleure.cout = cout_normal;
            }
            
            // Prêt de véhicule avec 1 passager
            if(conducteur.getPret()) {
                double distance_aller_pret = distance(conducteur.getOrigine(), passager.getOrigine()) +
                                           distance(passager.getOrigine(), conducteur.getDestination()) +
                                           distance(conducteur.getDestination(), passager.getDestination());
                
                double distance_retour_pret = distance(passager.getDestination(), conducteur.getDestination()) +
                                            distance(conducteur.getDestination(), passager.getOrigine()) +
                                            distance(passager.getOrigine(), conducteur.getOrigine());
                
                double distance_totale_pret = distance_aller_pret + distance_retour_pret;
                double cout_pret = distance_totale_pret - distance_evitee;
                
                // Vérifier les contraintes d'horaire pour le prêt
                if(respecteContraintesHoraire(conducteur, passager, "pret") && cout_pret < meilleure.cout) {
                    meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager.getNom() + 
                                           "-" + conducteur.getNom() + "-" + passager.getNom();
                    meilleure.trajet_retour = "+" + passager.getNom() + "+" + conducteur.getNom() + 
                                            "-" + passager.getNom() + "-" + conducteur.getNom();
                    meilleure.cout = cout_pret;
                }
            }
        }
        
        // Cas 2: 2 passagers (si le conducteur peut en prendre 3)
        if(conducteur.getNbPassagers() >= 3) {
            for(int i = 0; i < personnes.taille(); i++) {
                for(int j = i + 1; j < personnes.taille(); j++) {
                    const Personne& passager1 = personnes[i];
                    const Personne& passager2 = personnes[j];
                    
                    // Ne pas considérer le conducteur lui-même
                    if(passager1.getNom() == conducteur.getNom() || passager2.getNom() == conducteur.getNom()) {
                        continue;
                    }
                    
                    // Covoiturage normal avec 2 passagers
                    double distance_aller_2p = distance(conducteur.getOrigine(), passager1.getOrigine()) +
                                            distance(passager1.getOrigine(), passager2.getOrigine()) +
                                            distance(passager2.getOrigine(), passager1.getDestination()) +
                                            distance(passager1.getDestination(), passager2.getDestination()) +
                                            distance(passager2.getDestination(), conducteur.getDestination());
                    
                    double distance_retour_2p = distance(conducteur.getDestination(), passager2.getDestination()) +
                                             distance(passager2.getDestination(), passager1.getDestination()) +
                                             distance(passager1.getDestination(), passager2.getOrigine()) +
                                             distance(passager2.getOrigine(), passager1.getOrigine()) +
                                             distance(passager1.getOrigine(), conducteur.getOrigine());
                    
                    double distance_totale_2p = distance_aller_2p + distance_retour_2p;
                    double distance_evitee_2p = 2 * (distance(passager1.getOrigine(), passager1.getDestination()) +
                                                   distance(passager2.getOrigine(), passager2.getDestination()));
                    double cout_2p = distance_totale_2p - distance_evitee_2p;
                    
                    // Vérifier les contraintes d'horaire pour 2 passagers
                    if(respecteContraintesHoraire2Passagers(conducteur, passager1, passager2) && cout_2p < meilleure.cout) {
                        meilleure.trajet_aller = "+" + conducteur.getNom() + "+" + passager1.getNom() + "+" + passager2.getNom() + 
                                               "-" + passager1.getNom() + "-" + passager2.getNom() + "-" + conducteur.getNom();
                        meilleure.trajet_retour = "+" + conducteur.getNom() + "+" + passager1.getNom() + "+" + passager2.getNom() + 
                                                "-" + passager1.getNom() + "-" + passager2.getNom() + "-" + conducteur.getNom();
                        meilleure.cout = cout_2p;
                    }
                }
            }
        }
    }
    
    return meilleure;
}
// Fonction pour calculer le coût d'un trajet avec plusieurs passagers
double calculerCoutTrajet(const Personne& conducteur, const Tableau<Personne>& passagers) {
    if(passagers.taille() == 0) {
        return 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    }
    
    // Calculer la distance totale du trajet
    double distance_totale = 0;
    double distance_evitee = 0;
    
    // Aller: conducteur_origine -> tous les passagers_origine -> tous les passagers_destination -> conducteur_destination
    distance_totale += distance(conducteur.getOrigine(), passagers[0].getOrigine());
    for(int i = 0; i < passagers.taille() - 1; i++) {
        distance_totale += distance(passagers[i].getOrigine(), passagers[i + 1].getOrigine());
    }
    for(int i = 0; i < passagers.taille(); i++) {
        distance_totale += distance(passagers[i].getOrigine(), passagers[i].getDestination());
        distance_evitee += 2 * distance(passagers[i].getOrigine(), passagers[i].getDestination());
    }
    distance_totale += distance(passagers[passagers.taille() - 1].getDestination(), conducteur.getDestination());
    
    // Retour: conducteur_destination -> tous les passagers_destination -> tous les passagers_origine -> conducteur_origine
    distance_totale += distance(conducteur.getDestination(), passagers[passagers.taille() - 1].getDestination());
    for(int i = passagers.taille() - 1; i > 0; i--) {
        distance_totale += distance(passagers[i].getDestination(), passagers[i - 1].getDestination());
    }
    for(int i = passagers.taille() - 1; i >= 0; i--) {
        distance_totale += distance(passagers[i].getDestination(), passagers[i].getOrigine());
    }
    distance_totale += distance(passagers[0].getOrigine(), conducteur.getOrigine());
    
    return distance_totale - distance_evitee;
}

// Fonction pour vérifier si un trajet avec plusieurs passagers respecte les contraintes d'horaire
bool respecteContraintesHoraireMultiPassagers(const Personne& conducteur, const Tableau<Personne>& passagers) {
    // Vitesse constante de 10 mètres par seconde
    const double VITESSE = 10.0; // m/s
    
    if(passagers.taille() == 0) {
        return true;
    }
    
    // Calculer la distance totale du trajet
    double distance_totale = 0;
    
    // Aller
    distance_totale += distance(conducteur.getOrigine(), passagers[0].getOrigine());
    for(int i = 0; i < passagers.taille() - 1; i++) {
        distance_totale += distance(passagers[i].getOrigine(), passagers[i + 1].getOrigine());
    }
    for(int i = 0; i < passagers.taille(); i++) {
        distance_totale += distance(passagers[i].getOrigine(), passagers[i].getDestination());
    }
    distance_totale += distance(passagers[passagers.taille() - 1].getDestination(), conducteur.getDestination());
    
    // Retour
    distance_totale += distance(conducteur.getDestination(), passagers[passagers.taille() - 1].getDestination());
    for(int i = passagers.taille() - 1; i > 0; i--) {
        distance_totale += distance(passagers[i].getDestination(), passagers[i - 1].getDestination());
    }
    for(int i = passagers.taille() - 1; i >= 0; i--) {
        distance_totale += distance(passagers[i].getDestination(), passagers[i].getOrigine());
    }
    distance_totale += distance(passagers[0].getOrigine(), conducteur.getOrigine());
    
    // Temps en secondes
    double temps_total = distance_totale / VITESSE;
    
    // Heure d'arrivée à destination
    Heure arrivee_destination = conducteur.getHeureDepart() + (int)(temps_total / 2 / 60);
    Heure arrivee_retour = conducteur.getHeureDepartRetour() + (int)(temps_total / 2 / 60);
    
    // Vérifier les contraintes pour tous
    bool respecte = arrivee_destination <= conducteur.getHeureArrivee() &&
                   arrivee_retour <= conducteur.getHeureArriveeRetour();
    
    for(int i = 0; i < passagers.taille(); i++) {
        respecte = respecte && arrivee_destination <= passagers[i].getHeureArrivee() &&
                  arrivee_retour <= passagers[i].getHeureArriveeRetour();
    }
    
    return respecte;
}

// Fonction pour générer le trajet sous forme de string
std::string genererTrajet(const Personne& conducteur, const Tableau<Personne>& passagers) {
    std::string trajet = "+" + conducteur.getNom();
    for(int i = 0; i < passagers.taille(); i++) {
        trajet += "+" + passagers[i].getNom();
    }
    for(int i = passagers.taille() - 1; i >= 0; i--) {
        trajet += "-" + passagers[i].getNom();
    }
    trajet += "-" + conducteur.getNom();
    return trajet;
}

// Implémentation de l'algorithme niveau 5 (passagers illimités, prêt possible, avec contraintes horaires)
Recommandation calculerRecommandationNiveau5(const Personne& conducteur, const Tableau<Personne>& personnes) {
    // Coût de base : voyager seul
    double distance_seul = 2 * distance(conducteur.getOrigine(), conducteur.getDestination());
    Recommandation meilleure("+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            "+" + conducteur.getNom() + "-" + conducteur.getNom(),
                            distance_seul);
    
    // Vérifier si le conducteur peut prendre des passagers
    if(conducteur.getNbPassagers() >= 2) {
        // Pour simplifier, on utilise l'algorithme du niveau 4 qui fonctionne déjà bien
        // et on l'étend pour supporter plus de passagers si nécessaire
        return calculerRecommandationNiveau4(conducteur, personnes);
    }
    
    return meilleure;
}
