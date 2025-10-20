/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2
 *  http://ericbeaudry.uqam.ca/INF3105/tp2/         
 */
#include <cassert>
#include "personne.h"

std::istream& operator >> (std::istream& is, Personne& p){
    std::string s;
    int n;
    char c;
    // lire nom, nombre de passagers, l'acceptation de prêter le véhicule
    is >> p.nom >> n >> c;
    p.nb_passagers = n;
    p.pret = (c == 'O' || c == 'o');
    // lire l'origine (résidence) et la destination (travail / étude)
    is >> p.o >> s >> p.d;
    assert(s == "-->"); // robustesse
    // lire les contraintes de temps (4 heures)
    is >> p.heure_depart >> p.heure_arrivee >> p.heure_depart_retour >> p.heure_arrivee_retour;
    // lire le point-virgule de fin de ligne
    is >> c;
    assert(c== ';'); // robustesse
    return is;
}

std::ostream& operator << (std::ostream& os, const Personne& p){
     // À compléter si vous en avez besoin.
    return os;
}

