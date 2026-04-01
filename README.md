# 🌲 Analiza Wydajności Drzew Poszukiwań (Splay, AVL, BST)

## 📌 O projekcie
Projekt to zaawansowane środowisko testowe napisane w języku C++, służące do analizy porównawczej trzech struktur danych: **Splay Tree**, **AVL Tree** oraz klasycznego **Binary Search Tree (BST)**. 

Głównym celem projektu nie była jedynie implementacja struktur od zera, ale przede wszystkim zbadanie, jak drzewo Splay zachowuje się w warunkach nierównomiernego dostępu do danych (symulacja efektu *cache/locality of reference*), w porównaniu do idealnie zbalansowanego drzewa AVL.

## 🚀 Kluczowe funkcjonalności
* **Własna implementacja struktur:** Drzewa Splay, AVL oraz BST napisane od podstaw bez użycia gotowych kontenerów STL (poza wektorem do tasowania kluczy).
* **Precyzyjne profilowanie:** Pomiar czasu operacji `Search` z dokładnością do nanosekund przy użyciu biblioteki `<chrono>`.
* **Symulacja rzeczywistego obciążenia:** Wykorzystanie biblioteki `<random>` do generowania asymetrycznych zapytań (szukanie z określonym prawdopodobieństwem $\alpha$ w małym podzbiorze danych).

## 📊 Metodologia testów
W programie zaimplementowano funkcję losującą `losujZRozkladem`, która symuluje sytuację, w której niewielka część danych (np. 100 pierwszych elementów) jest wyszukiwana znacznie częściej niż reszta ogromnego zbioru (składającego się z milionów elementów). 

Testy iterują po różnych wartościach parametru prawdopodobieństwa $\alpha$ (od 0.01 do 0.99), wykonując miliony operacji wyszukiwania i porównując średni czas odpowiedzi dla każdej ze struktur. Pozwala to udowodnić w praktyce amortyzowaną złożoność czasową drzewa Splay rzędu $O(\log n)$ i jego przewagę w specyficznych scenariuszach nad sztywno zbalansowanym drzewem AVL.

## 🛠️ Technologie
* C++ (Standard C++11/C++14 lub nowszy)
* Standard Template Library (STL)

## ⚙️ Jak uruchomić
Kompilacja przy użyciu kompilatora g++:
```bash
g++ Projekt_Drzewa_splay.cpp -o benchmark -O3