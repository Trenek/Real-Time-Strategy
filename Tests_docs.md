# Testy Czarnoskrzynkowe

## Testy Ekranów

### Menu Startowe
- **Akcja:** Użytkownik wybiera opcję "Rozpocznij".
    - **Oczekiwany rezultat:** Rozgrywka się rozpoczyna, użytkownik widzi ekran rozgrywki.
- **Akcja:** Użytkownik wybiera opcję "Wyjdź".
    - **Oczekiwany rezultat:** Aplikacja zamyka się.
- **Akcja:** Użytkownik przemieszcza kursor na przyciski.
    - **Oczekiwany rezultat:** Przyciski podświetlają się.
- **Akcja:** Obserwacja tła w menu startowym.
    - **Oczekiwany rezultat:** Tło jest widoczne.

### Menu Pauzy
- **Akcja:** Użytkownik wybiera opcję "Pauza".
    - **Oczekiwany rezultat:** Rozgrywka zostaje wznowiona.
- **Akcja:** Użytkownik wybiera opcję "Wyjdź".
    - **Oczekiwany rezultat:** Gra wraca do menu głównego.
- **Akcja:** Obserwacja tła w menu pauzy.
    - **Oczekiwany rezultat:** Tło odpowiada ostatniej klatce rozgrywki przed zapauzowaniem.

### Ekran Końca Gry
- **Akcja:** Rozgrywka kończy się.
    - **Oczekiwany rezultat:** Wyświetla się ekran końca gry z informacją o wyniku.
- **Akcja:** Użytkownik klika dowolnym przyciskiem myszy na ekranie końca gry.
    - **Oczekiwany rezultat:** System przechodzi do menu głównego.

### Ekran Rozgrywki
- **Akcja:** Użytkownik wybiera opcję "Pauza".
    - **Oczekiwany rezultat:** Gra wstrzymuje się i wyświetla menu pauzy.
- **Akcja:** Obserwacja tła na ekranie rozgrywki.
    - **Oczekiwany rezultat:** Tło jest takie samo jak w menu startowym.
- **Akcja:** Użytkownik steruje armiami.
    - **Oczekiwany rezultat:** Możliwość sterowania armią w czasie rzeczywistym.

---

## Testy Przycisków

### Podświetlenie przycisków
- **Akcja:** Użytkownik najeżdża kursorem na przycisk.
    - **Oczekiwany rezultat:** Przycisk podświetla się.

### Aktywacja przycisków
- **Akcja:** Użytkownik klika na przycisk.
    - **Oczekiwany rezultat:** Odpowiednia akcja zostaje wykonana.

---

## Testy Rozgrywki

### Rozpoczęcie rozgrywki
- **Akcja:** Użytkownik wybiera opcję "Rozpocznij" z menu startowego.
    - **Oczekiwany rezultat:** Gra rozpoczyna się z 6 armiami po obu stronach.

### Wybór armii
- **Akcja:** Użytkownik klika na przycisk reprezentujący armię.
    - **Oczekiwany rezultat:** Armia zostaje wybrana i podświetlona.
- **Akcja:** Użytkownik klika na jednostkę należącą do armii.
    - **Oczekiwany rezultat:** Armia zostaje wybrana i podświetlona.
- **Akcja:** Użytkownik klika w dowolne miejsce lewym przyciskiem myszy.
    - **Oczekiwany rezultat:** Wybór armii zostaje anulowany.

### Zmiana centrum armii
- **Akcja:** Użytkownik wybiera armię i klika prawym przyciskiem myszy w dowolne miejsce.
    - **Oczekiwany rezultat:** Armia przemieszcza się w stronę nowego centrum.

### Wybór celu ataku
- **Akcja:** Użytkownik wybiera armię i klika prawym przyciskiem myszy na jednostkę wrogiej armii.
    - **Oczekiwany rezultat:** Armia wybiera za cel wrogą armię.

### Zderzenia
- **Akcja:** Dwie jednostki zderzają się ze sobą.
    - **Oczekiwany rezultat:** Dochodzi do zderzenia sprężystego.

### Kierowanie się do celu
- **Akcja:** Jednostka znajduje się w odległości większej niż jej promień od celu.
    - **Oczekiwany rezultat:** Jednostka przyspiesza w stronę celu, aż osiągnie maksymalną prędkość.
- **Akcja:** Jednostka znajduje się w odległości mniejszej niż jej promień od celu.
    - **Oczekiwany rezultat:** Jednostka spowalnia i zatrzymuje się.

### Walka
- **Akcja:** Armia atakuje wrogą armię.
    - **Oczekiwany rezultat:** Atak następuje, jednostki zmieniają kolory w zależności od poziomu zdrowia, a martwe jednostki znikają.
- **Akcja:** Armia będąca w stanie spoczynku zostaje zaatakowana.
    - **Oczekiwany rezultat:** Armia automatycznie kontratakuje.

### Anulowanie ataku
- **Akcja:** Armia atakująca wrogą armię otrzymuje rozkaz zmiany centrum.
    - **Oczekiwany rezultat:** Atak zostaje anulowany, armia przemieszcza się w stronę nowego centrum.

### Znikanie przycisków i armii
- **Akcja:** Liczba jednostek w armii spada do 0.
    - **Oczekiwany rezultat:** Armia przestaje istnieć, a przycisk z nią związany znika.

---

Testy te pokrywają podstawowe scenariusze funkcjonalne oraz oczekiwane interakcje użytkownika z systemem. W celu zapewnienia pełnego pokrycia testowego należy rozważyć przypadki brzegowe, takie jak kliknięcia poza przyciskami, nagłe zamknięcie aplikacji czy sytuacje wyjątkowe podczas rozgrywki.
