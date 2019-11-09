//
// Created by User on 09.11.2019.
//

#ifndef UNTITLED1_DIJKSTRA_H
#define UNTITLED1_DIJKSTRA_H

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_INT 2147483647

class Dijkstra {
private:
    struct Vertice {
        Vertice() : _neighbor(0), _value(-1), _visited(false) {};

        Vertice(bool visit) : _visited(visit) {};
        std::vector<std::pair<int, int>> _neighbor; // lista sąsiedznich wierzchołków pair = to wektor 2 liczb  wierzchołek + jego waga
        int _value; // koszt dostania się do wierzchołka
        bool _visited; //czy został odwiedzony wieszchołek

    };

    std::vector<Vertice> _vertices; // wektor struktury
    fstream file;

public:

    //główny konstruktor, (nazwa wejścia, z jakiego wierzchołka, do jakiego)
    Dijkstra(std::string filepath, int from, int to) {
        file.open(filepath, ios::in);

        int temp;

        int v1 = -1; // wierzchołek 1
        int v2 = -1; // wierzchołek 2
        int value; // waga

        // wejscie ilosc wierzchołków
        file >> temp;

        _vertices.reserve(temp + 1);
        _vertices.emplace_back(true); // wypełnienie wektora wartościami true

        for (int i = 0; i < temp; i++)
            _vertices.emplace_back();

        // liczba krawędzi
        file >> temp;

        while (temp--) {
            file >> v1;
            file >> v2;
            file >> value;

            // wprowadza przykładowo od wierzchołka 1 + wartość do wierzchołka 2 i rewers
            _vertices[v1]._neighbor.push_back(std::make_pair(v2, value));
            _vertices[v2]._neighbor.push_back(std::make_pair(v1, value));
        }

        file.close();






        // Czyszczenie struktury
        reset();

        // wierzchołek startowy ( max_int = wartość jaką należy dostarczyć z pkt a do pkt b)
        _vertices[from]._value = MAX_INT;

        // czasowmierz
        auto start = std::chrono::steady_clock::now();
        find(from);
        int val = _vertices.at(to)._value;
        auto end = std::chrono::steady_clock::now();


        file.open(filepath + "Dijkstra-OUT.txt", std::ios::out);
        // zapisz do pliku czas trwania algorytmu
        file << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        file.close();


    }
/*
    Dijkstra& operator=(const Dijkstra& nd)
    {
        if (this != &nd)
            _vertices = nd._vertices;

        return *this;
    }*/

    // Sprawdź 2 liczby zwróć mniejszą, porównywanie mniejszą z większą i ustalenie na wierzchołku wagi przepływu
    int getLower(int first, int second) {
        return first > second ? second : first;
    }

    // Jeżeli wierzchołek ma jakiś przepływ ale dochodzi do niego jeszcze inna krawędź z innego wierzchołka wówczas
    // wykonaj porównanie mniejszego z większym i zwraca większy (ustawia przepływ)
    int getHigher(int first, int second) {
        return first > second ? first : second;
    }

    /**
     * szukaj trasy od początku (kosztu) do każdego wierzchołka
     * @param index Początek
     */
    void find(int index) {

        for (size_t i = 0; i < _vertices[index]._neighbor.size(); i++) {

            // .first wierzchołek
            int tempFirst = _vertices[index]._neighbor[i].first;
            // .second wartosc drogi
            int tempSecond = _vertices[index]._neighbor[i].second;


            //Dijkstra start przechowuje wartość początkową
            int tempValueStart = _vertices[index]._value;

            //Dijkstra koncowy wartość przepływu do sąsiada
            int tempValueEnd = _vertices[tempFirst]._value;



            //Branie wartości mniejszej z funkcji getLower potem porównuje ją z sąsiadem i wybieram większą
            int calculate = getHigher(getLower(tempValueStart, tempSecond), tempValueEnd);

            // Jeżeli wierzchołek był odwiedzony ale znalazł lepszą ścieżkę to wykonaj się jeszcze raz i od razu
            //przypisz lepszą wartość (calculated) a w innym wypadku zapisz ustaloną ścieżkę
            if (tempValueEnd < calculate && _vertices[tempFirst]._visited == true) {
                _vertices[tempFirst]._value = calculate;
                find(tempFirst);
            } else
                _vertices[tempFirst]._value = calculate;

        }

        //oznacz wierzchołek jako odwiedzony
        _vertices[index]._visited = true;


        // Rekurencyjnie wykonaj się dla keżdego sąsiada jeżeli nie został odwiedzony
        for (size_t i = 0; i < _vertices.at(index)._neighbor.size(); i++) {
            //wyliczam sąsiada jeżeli nieowiedzony = true
            if (_vertices[_vertices[index]._neighbor[i].first]._visited != true)
                find(_vertices[index]._neighbor[i].first);
        }
    }

    void reset() {
        for (int i = 0; i < _vertices.size(); i++) {
            _vertices[i]._visited = false;
            _vertices[i]._value = -1;
        }
    }
};


#endif //UNTITLED1_DIJKSTRA_H
