/* Squelette pour classe générique Tableau<T>.
 * Lab3 + TP1 -- Tableau dynamique générique
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/tp1/
 * http://ericbeaudry.uqam.ca/INF3105/tp2/
 */

#if !defined(TABLEAU_INF3105_H_)
#define TABLEAU_INF3105_H_

#include <cassert>

template <class T>
class Tableau{

  public:
    Tableau(int capacite_initiale=4);
    Tableau(const Tableau&);
    ~Tableau();

    // Ajoute un element à la fin
    void           ajouter(const T& element);
    // Vide le tableau
    void           vider();
    // Retourne le nombre d'éléments dans le tableau
    int            taille() const;

    // Insère element à position index. Les éléments à partir de index sont décalés d'une position.
    void           inserer(const T& element, int index=0);

    // Enlève l'element à position index. Les éléments après index sont décalés d'une position.
    void           enlever(int index=0);

    // Cherche et retourne la position de l'élément. Si non trouvé, retourne -1.
    int            chercher(const T& element) const;

    const T&       operator[] (int index) const;
    T&             operator[] (int index);

    bool           operator == (const Tableau<T>& autre) const;
    Tableau<T>&    operator = (const Tableau<T>& autre);

    // ----------------- Fonctionnalités spécifiques au TP1 ------------
    // Retourne une copie du tableau, mais sans doublons.
    Tableau<T> sans_doublons(bool garderPremier=true) const;
    // Supprime les doublons du tableau et retourne le nombre d'occurrences supprimées.
    int supprimer_doublons(bool garderPremier=true);
    // -----------------------------------------------------------------
    
    
  private:
    T*             elements;
    int            nbElements;
    int            capacite;
};


// ---------- Définitions, modèles de -------------
// En C++, les définitions sont habituellement dans des fichiers sources .cpp.
// Les classes "template" sont souvent une exception à cette règle.
// Ces modèles devant être instanciés, on les mets dans un fichier .h.


template <class T>
Tableau<T>::Tableau(int capacite_)
{
    capacite = capacite_;
    nbElements = 0;
    elements = new T[capacite];
}

template <class T>
Tableau<T>::Tableau(const Tableau& autre)
{
    capacite = autre.capacite;
    nbElements = autre.nbElements;
    elements = new T[capacite];
    for(int i = 0; i < nbElements; i++) {
        elements[i] = autre.elements[i];
    }
}

template <class T>
Tableau<T>::~Tableau()
{
    delete[] elements;
}

template <class T>
int Tableau<T>::taille() const
{
    return nbElements;
}

template <class T>
void Tableau<T>::ajouter(const T& item)
{
    if(nbElements >= capacite) {
        // Agrandir le tableau
        T* nouveau = new T[capacite * 2];
        for(int i = 0; i < nbElements; i++) {
            nouveau[i] = elements[i];
        }
        delete[] elements;
        elements = nouveau;
        capacite *= 2;
    }
    elements[nbElements] = item;
    nbElements++;
}

template <class T>
void Tableau<T>::inserer(const T& element, int index)
{
    assert(index >= 0 && index <= nbElements);
    if(nbElements >= capacite) {
        // Agrandir le tableau
        T* nouveau = new T[capacite * 2];
        for(int i = 0; i < nbElements; i++) {
            nouveau[i] = elements[i];
        }
        delete[] elements;
        elements = nouveau;
        capacite *= 2;
    }
    // Décaler les éléments
    for(int i = nbElements; i > index; i--) {
        elements[i] = elements[i-1];
    }
    elements[index] = element;
    nbElements++;
}

template <class T>
void Tableau<T>::enlever(int index)
{
    assert(index >= 0 && index < nbElements);
    // Décaler les éléments
    for(int i = index; i < nbElements - 1; i++) {
        elements[i] = elements[i+1];
    }
    nbElements--;
}

template <class T>
int Tableau<T>::chercher(const T& element) const
{
    for(int i = 0; i < nbElements; i++) {
        if(elements[i] == element) {
            return i;
        }
    }
    return -1;
}

template <class T>
void Tableau<T>::vider()
{
    nbElements = 0;
}

template <class T>
const T& Tableau<T>::operator[] (int index) const
{
    assert(index >= 0 && index < nbElements);
    return elements[index];
}

template <class T>
T& Tableau<T>::operator[] (int index)
{
    assert(index >= 0 && index < nbElements);
    return elements[index];
}

template <class T>
Tableau<T>& Tableau<T>::operator = (const Tableau<T>& autre)
{
    if(this != &autre) {
        delete[] elements;
        capacite = autre.capacite;
        nbElements = autre.nbElements;
        elements = new T[capacite];
        for(int i = 0; i < nbElements; i++) {
            elements[i] = autre.elements[i];
        }
    }
    return *this;
}

template <class T>
bool Tableau<T>::operator == (const Tableau<T>& autre) const
{
    if(nbElements != autre.nbElements) {
        return false;
    }
    for(int i = 0; i < nbElements; i++) {
        if(elements[i] != autre.elements[i]) {
            return false;
        }
    }
    return true;
}

// Retourne une copie du tableau, mais sans doublons.
template <class T>
Tableau<T> Tableau<T>::sans_doublons(bool garderPremier) const {
    Tableau<T> resultat;
    for(int i = 0; i < nbElements; i++) {
        bool dejaPresent = false;
        for(int j = 0; j < resultat.nbElements; j++) {
            if(resultat.elements[j] == elements[i]) {
                dejaPresent = true;
                break;
            }
        }
        if(!dejaPresent) {
            resultat.ajouter(elements[i]);
        }
    }
    return resultat;
}

// Supprime les doublons du tableau et retourne le nombre d'occurrences supprimées.
template <class T>
int Tableau<T>::supprimer_doublons(bool garderPremier) {
    int nbSupprimes = 0;
    for(int i = 0; i < nbElements; i++) {
        for(int j = i + 1; j < nbElements; j++) {
            if(elements[i] == elements[j]) {
                enlever(j);
                j--; // Ajuster l'index après suppression
                nbSupprimes++;
            }
        }
    }
    return nbSupprimes;
}


#endif //define TABLEAU_INF3105_H_