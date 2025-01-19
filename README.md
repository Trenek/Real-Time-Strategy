# Gra Strategiczna RTS
Projekt jest grą strategiczną typu Real Time Strategy.

## Budowanie projektu
Projekt można skompilować korzystając z narzędzia Cmake w połączeniu z kompilatorem MinGW.
Dodatkowo do kompilacji potrzebne będzie Vulkan SDK https://vulkan.lunarg.com/sdk/home
Projekt można uruchomić/skompilować za pomocą skryptów `buildDebug.bat`, `buildRelease.bat` i `run.bat`

## Użyte technologie
  * C23
  * Vulkan
  * CGLM
  * CGLTF
  * GLTW
  * STB
  * tinyobjloader

## Użyte wzorce projektowe
  Klasy VulkanTools i Model stanowią fasady.
  Klasy ModelBuilder, fighterRange, fighterAttack i fighterDestination reprezentują wzorzec Builder

## Krótka Instrukcja obsługi
Po uruchomieniu wyświetla się menu główne w którym można zakończyć działanie aplikacji, bądź rozpocząć grę.
W samej gdzie celem jest pokonanie wszystkich jednostek wroga. W zależności od tego, czy się to powiedzie
czy nie zostanie pokazany odpowiedni ekran końcowy, po czym wskutek kliknięcia lewym przyciskiem myszki
przechodzimy do menu głównego.
<<<<<<< HEAD

=======
### Menu (?)
  * Menu Główne:
    * Przycisk rozpoczynający grę
    * Przycisk zamykający aplikację
  * Menu Pauzy:
    * Przycisk wznawiający rozgrywkę
    * Przycisk zamykający aplikację
  * Menu Wygranej/Przegranej:
    * Kliknięcie lewym przyciskiem myszy powoduje przejście do menu głównego
  * Menu rozgrywki (?)
    * Przycisk wstrzymujący rozgrywkę
    * Przyciski pozwalający wybrać posiadanych armii
>>>>>>> 00fcf1baead6d68e159314c958113356e99e7b9e
### Sterowanie:
  * WASD - przemieszczanie się
  * Spacja - lecenie w górę
  * Shift - lecenie w dół
  * strzałki - poruszania kamerą
  * C - przejęcie / oddanie kontroli nad kamerą
### Sterowanie Wojskami:
  Aby wybrać jedną ze swoich armii należy kliknąć na jedną z jej jednostek lub na odpowiedni przycisk
    znajdujący się na dole ekranu
  Aby przemieścić gdzieś wojsko należy po wybraniu go kliknąć gdzieś prawym przyciskiem myszy. Zmienia to centrum armii na kliknięte miejsce
  Aby zaatakować inne wojsko należy po wybraniu swojego wojska kliknąć na wrodzie wojsko prawym przyciskiem myszy. Od tego momentu nasze wojsko porusza się do odpowiednio zdefiniowanego dla jednostki centrum.
### Zasady przemieszczania się wojska
  Każda jednostka posiada zależnie od wybranej formacji docelową pozycję względem centrum armii, do którego przyspiesza. W momencie w którym odpowiednio się do niego zbliży zaczyna spowalniać aby się zatrzymać
### Walka
  Jednostki walczą ze sobą na zasadzie zderzeń. Zderzenie co określony czas zmniejsza życie uderzonej jednostki. Jednostki mają różne tekstury w zależności od poziomu życia, od 100% do 66.6%, od 66.6% do 33.3% i od 33.3% do 0%. Jednostki nie posiadające życia znikają.
## Diagram Klas
![Wzorce Projektowe](https://github.com/user-attachments/assets/7393aacf-5bfe-44ac-adb9-739fc06070db)
## Zrzuty Ekranu
![Nowy obraz mapy bitowej](https://github.com/user-attachments/assets/e56b954a-259d-4d1f-8dfa-d4bce8fba502)
![Nowy obraz mapy bitowej (2)](https://github.com/user-attachments/assets/bfb2c3ca-14d6-4ec6-a133-7ac793c65a97)
![win](https://github.com/user-attachments/assets/85129632-959d-48bc-8ea7-b7954ef86d94)
