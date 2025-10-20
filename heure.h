/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2
 *  http://ericbeaudry.uqam.ca/INF3105/tp2/         
 */
#if !defined(__HEURE_H__)
#define __HEURE_H__

#include <iostream>

class Heure{
  public:
    Heure() : minutes(0) {}
    Heure(int h, int m) : minutes(h * 60 + m) {}
    
    bool operator <(const Heure& h) const;
    bool operator <=(const Heure& h) const;
    bool operator >(const Heure& h) const;
    bool operator >=(const Heure& h) const;
    bool operator ==(const Heure& h) const;
    bool operator !=(const Heure& h) const;
    
    Heure operator +(int minutes_ajoutees) const;
    int operator -(const Heure& h) const; // retourne la différence en minutes
    
    int getMinutes() const { return minutes; }
    int getHeures() const { return minutes / 60; }
    int getMinutesRestantes() const { return minutes % 60; }
  
  private:
    int minutes; // nombre total de minutes depuis minuit


  friend std::istream& operator >> (std::istream&, Heure&);
  friend std::ostream& operator << (std::ostream&, const Heure&);
};

#endif

