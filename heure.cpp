/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2
 *  http://ericbeaudry.uqam.ca/INF3105/tp2/         
 */
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include "heure.h"

bool Heure::operator <(const Heure& h) const{
    return minutes < h.minutes;
}

bool Heure::operator <=(const Heure& h) const{
    return minutes <= h.minutes;
}

bool Heure::operator >(const Heure& h) const{
    return minutes > h.minutes;
}

bool Heure::operator >=(const Heure& h) const{
    return minutes >= h.minutes;
}

bool Heure::operator ==(const Heure& h) const{
    return minutes == h.minutes;
}

bool Heure::operator !=(const Heure& h) const{
    return minutes != h.minutes;
}

Heure Heure::operator +(int minutes_ajoutees) const{
    return Heure(0, minutes + minutes_ajoutees);
}

int Heure::operator -(const Heure& h) const{
    return minutes - h.minutes;
}


std::istream& operator >> (std::istream& is, Heure& heure){
    int heures, minutes;
    char h;
    is >> heures >> h >> minutes;
    if(!is) return is;
    assert(h=='h' || h=='H'); // pour robustesse: valider le caractère 'h'
    heure.minutes = heures * 60 + minutes;
    return is;
}

std::ostream& operator << (std::ostream& os, const Heure& h){
    char chaine[40];
    sprintf(chaine, "%02dh%02d", h.getHeures(), h.getMinutesRestantes());
    os << chaine;
    return os;
}

