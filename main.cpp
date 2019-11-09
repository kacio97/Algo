#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct dane {

    //dane(int m1, int m2, int maksWartosc) : m1(m1),  m2(m2), maksWartosc(maksWartosc) {}

    long m1;
    long m2;
    long maksWartosc;

    bool operator < (const dane& struktura) const // funkcja niemodyfikuje danych przeciazony operator porownania
    {
        return (maksWartosc < struktura.maksWartosc);
    }
};

class Turystyka {
private:

    long poczatekTrasy;
    long koniecTrasy;
    long iloscPasazerow;
    long iloscMiast;
    long iloscDrog;
    long MSTwaga;

    std::vector<dane> krawedzie;
    std::vector<dane> MST; //Minimalne drzewo rozpinające
    std::vector<long> drzewa;
    std::vector<long> id; //Vector przechowujący informacje o połączeniach między drzewami pozwalający na określenie wszystkich optymalnych dróg


    void rozmiarVector(int);

    void sortuj();

    void wypiszMST();

    void szukajTrasy(std::vector<dane> &droga, long poczatek, long koniec, std::vector<bool> &odwiedzone);

public:
    //Wartości definiowane domyślnie
    Turystyka() : poczatekTrasy(-1), koniecTrasy(-1), iloscPasazerow(-1), iloscMiast(-1), iloscDrog(-1), MSTwaga(-1) {} //konstruktor domyślny

    void dodajDane(); //wprowadź dane z pliku wejściowego

    void wypisz();

    void kruskal(); // Algorytm kruskala

    void dfs();


};

void Turystyka::dodajDane()
{

    long tmp;
    std::cin >> iloscMiast;
    std::cin >> iloscDrog;

    rozmiarVector(iloscDrog);

    id.resize(iloscMiast, -1);

    for (int i = 0; i < iloscDrog; i++)
    {
        std::cin >> tmp;
        krawedzie[i].m1 = tmp;

        std::cin >> tmp;
        krawedzie[i].m2 = tmp;

        std::cin >> tmp;
        krawedzie[i].maksWartosc = tmp;


    }

    sortuj();

}

void Turystyka::sortuj()
{
    std::sort(krawedzie.rbegin() , krawedzie.rend());
}

void Turystyka::kruskal() {

    std::vector<dane>::iterator it;
    std::vector<long>::iterator it2;

    int u, v, licznik = 0;

    for (it = krawedzie.begin(); it != krawedzie.end(); it++) {

        if (MST.size() == iloscMiast - 1)
        {
            break;
        }

        u = id[it->m1 - 1];

        v = id[it->m2 - 1];

        //Jeżeli oba wierzchołki nie są z niczym połączone mają wartość -1, po przypisaniu wartości licznika pushuj
        // do vectora id
        if (u == -1 && v == -1) {

            id[it->m1 - 1] = licznik;

            id[it->m2 - 1] = licznik;

            MST.push_back(*it);

            licznik++;
        }
            //Jeżeli tylko 2 wierzchołek ma domyślną wartość to przypisz mu wartość 1 wierzchołka z pod konkretnej pozycji w vectorze
        else if (u != -1 && v == -1) {

            id[it->m2 - 1] = id[it->m1 - 1];

            MST.push_back(*it);
        }
            //Jeżeli tylko 1 wierzchołek ma domyślną wartość to przypisz mu wartość 2 wierzchołka z pod konkretnej pozycji w vectorze
        else if (u == -1 && v != -1) {
            id[it->m1 - 1] = id[it->m2 - 1];

            MST.push_back(*it);
        }
            // Jeżeli wierzchołek u jest różna od v oraz wartość vector'a o danym adresie jest równa wierzchołkowi v to przypisz mu wartość wierzchołka u
            // ma to na celu złączeniu wrzystkich poddrzew w 1 drzewo.
        else if (u != v) {

            for (it2 = id.begin(); it2 != id.end(); it2++)
            {
                if (*it2 == v)
                {
                    *it2 = u;
                }
            }
            MST.push_back(*it);

        }
    }

    //wypiszMST();

}

void Turystyka::dfs() //do wyszukiwania drogi stosuje algorytm przeszukiwania w głąb
{
    std::vector<bool> odwiedzone;
    odwiedzone.resize(iloscMiast, false);
    std::vector<dane> droga;
    int minWaga;

    std::cin >> poczatekTrasy;
    std::cin >> koniecTrasy;

    while (poczatekTrasy > 0 && koniecTrasy > 0)
    {
        std::cin >> iloscPasazerow;
        szukajTrasy(droga, poczatekTrasy, koniecTrasy, odwiedzone);
        minWaga = droga[0].maksWartosc;

        for (int i = 1; i < droga.size(); i++)
        {
            if (droga[i].maksWartosc < minWaga)
            {
                minWaga = droga[i].maksWartosc;
            }
        }
        std::cout << ceil(double(iloscPasazerow) / double(minWaga - 1)) << std::endl; //wyliczamy ilość kursów jakie należy wykonać

        std::cin >> poczatekTrasy;
        std::cin >> koniecTrasy;

        //Czyszczenie vectorów
        droga.clear();

        for (int i = 0; i < iloscMiast; i++)
        {
            odwiedzone[i] = false;
        }
    }

}

void Turystyka::szukajTrasy(std::vector<dane> &droga, long poczatek, long koniec, std::vector<bool> &odwiedzone)
{
    for (int i = 0; i < MST.size(); i++)
    {
        if (MST[i].m1 == poczatek)// Miasto 1 jest miastem startowym
        {

            if (odwiedzone[MST[i].m2 - 1] == false) {

                droga.push_back(MST[i]);

                //jeżeli miasto 2 okazuje się ślepym zaułkiem wychodzimy z funkcji i cofamy się do poprzedniego miasta
                if (MST[i].m2 == koniec)
                {
                    return; //wyjdz z funkcji
                }

                odwiedzone[MST[i].m2 - 1] = true;

                szukajTrasy(droga, MST[i].m2, koniec, odwiedzone);

                if (droga.back().m1 == koniec || droga.back().m2 == koniec)
                {
                    return;
                }
            }
        }
        else if (MST[i].m2 == poczatek) // Miasto 2 jest miastem startowym
        {
            if (odwiedzone[MST[i].m1 - 1] == false) {

                droga.push_back(MST[i]);

                if (MST[i].m1 == koniec)
                {
                    return; //wyjdz z funkcji
                }

                odwiedzone[MST[i].m1 - 1] = true;

                szukajTrasy(droga, MST[i].m1, koniec, odwiedzone);

                if (droga.back().m1 == koniec || droga.back().m2 == koniec)
                {
                    return;
                }
            }
        }
    }
    //Wycofaj ostatni element jeżeli był ślepym zaułkiem
    droga.pop_back();
}

void Turystyka::wypisz()
{
    for (int i = 0; i < krawedzie.size(); i++)
    {
        std::cout << krawedzie[i].m1 << " " << krawedzie[i].m2 << " " << krawedzie[i].maksWartosc << " "
                  << std::endl;
    }
}

void Turystyka::wypiszMST()
{
    std::vector<dane>::iterator it;
    std::vector<long>::iterator it2;
    for (it = MST.begin(); it != MST.end(); it++) {
        std::cout << it->m1 << " - " << it->m2 << " - " << it->maksWartosc << std::endl;
    }
    for (it2 = id.begin(); it2 != id.end(); it2++)
    {
        std::cout << *it2 << std::endl;
    }
}

void Turystyka::rozmiarVector(int rozmiar)
{
    krawedzie.resize(rozmiar);
}


int main() {

    Turystyka graf;

    graf.dodajDane();

    graf.kruskal();

    //graf.wypisz();

    graf.dfs();

}

