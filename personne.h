/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2
 *  http://ericbeaudry.uqam.ca/INF3105/tp2/         
 */

#if !defined(__PERSONNE_H__)
#define __PERSONNE_H__

#include <iostream>
#include "heure.h"
#include "pointst.h"

class Personne{
  public:
    Personne() : nb_passagers(0), pret(false) {}
    
    // Getters
    std::string getNom() const { return nom; }
    int getNbPassagers() const { return nb_passagers; }
    bool getPret() const { return pret; }
    PointST getOrigine() const { return o; }
    PointST getDestination() const { return d; }
    Heure getHeureDepart() const { return heure_depart; }
    Heure getHeureArrivee() const { return heure_arrivee; }
    Heure getHeureDepartRetour() const { return heure_depart_retour; }
    Heure getHeureArriveeRetour() const { return heure_arrivee_retour; }
    
    // Setters
    void setNom(const std::string& n) { nom = n; }
    void setNbPassagers(int nb) { nb_passagers = nb; }
    void setPret(bool p) { pret = p; }
    void setOrigine(const PointST& orig) { o = orig; }
    void setDestination(const PointST& dest) { d = dest; }
    void setHeureDepart(const Heure& h) { heure_depart = h; }
    void setHeureArrivee(const Heure& h) { heure_arrivee = h; }
    void setHeureDepartRetour(const Heure& h) { heure_depart_retour = h; }
    void setHeureArriveeRetour(const Heure& h) { heure_arrivee_retour = h; }
    
    // Opérateurs de comparaison
    bool operator ==(const Personne& p) const { return nom == p.nom; }
    bool operator !=(const Personne& p) const { return nom != p.nom; }
  
  private:
    std::string nom;
    int nb_passagers; // nombre de passagers que peut embarquer (incluant elle-même)
    bool pret; // accepte de prêter son véhicule
    PointST o, d; // origine et destination
    Heure heure_depart; // heure à laquelle elle est prête à quitter sa résidence
    Heure heure_arrivee; // heure pour laquelle elle doit être rendue à destination
    Heure heure_depart_retour; // heure à laquelle elle peut quitter la destination
    Heure heure_arrivee_retour; // heure pour laquelle elle doit être de retour à sa résidence

  friend std::istream& operator >> (std::istream&, Personne&);
  friend std::ostream& operator << (std::ostream&, const Personne&);
};

#endif

