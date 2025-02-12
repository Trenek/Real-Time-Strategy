# Wymagania Funkcjonalne
## Ekrany
### 1. Menu Startowe
  System będzie zawierał menu startowe w ramach którego możliwy będzie wybór jednej z poniższych opcji:
  * Rozpocznij - rozpoczyna rozgrywkę
  * Wyjdź - kończy działanie aplikacji
  Ponadto menu startowe posiada tło
### 2. Menu Pauzy
  System będzie zawierał menu pauzy w ramach którego możliwy będzie wybór jednej z poniższych opcji:
  * Pauza - wznawia rozgrywkę zamykając menu pauzy
  * Wyjdź - kończy rozgrywkę przechodząc do menu głównego
  Ponadto tłem menu pauzy jest ostatnia klatka rozgrywki przed zapauzowaniem
### 3. Ekran Końca Gry
  System będzie zawierał ekran końca gry, który wyświetla się po zakończeniu rozgrywki.
  Ekran ten przekazuje informację o wyniku gry.
  Na skutek kliknięcia lewym przyciskiem myszy w dowolne miejsce system przechodzi do menu głównego
### 4. Ekran Rozgrywki
  System będzie zawierał ekran rozgrywki w ramach którego możliwy będzie wybór jednej z poniższych opcji:
  * Pauza - wstrzymuje rozgrywkę i przechodzi do menu Pauzy
  Ponadto ekran rozgrywki:
  * posiada tło jak w menu startowym
  * pokazuje w czasie rzeczywistym rozgrywkę i pozwala na kontrolę armii
  * pokazuje przyciski odpowiedzialne za wybór jednej z posiadanych armii
## Przyciski
  Każdy przycisk po najechaniu na niego zostaje podświetlony.
  Po kliknięciu na przycisk zostaje aktywowany i robi rzeczy w zależności od przeznaczenia.
## Rozgrywka
### Generalnia
  Każda ze stron rozpoczyna rozgrywnę z 6 armiami składającymi się z jednostek tego samego typu.
### Wybór armii
  Wyboru jednej ze swoich armii można dokonać na 2 sposoby:
  * Poprzez kliknięcie przycisku reprezentującego daną armię
  * Poprzez bezpośrednie kliknięcie jednej z jednostek należących do danej armii
  Najechanie na jednostkę/przycisk powoduje podświetlenie odpowiedniej armii.
  Wybór armii powoduje jej podświetlenie tak długo, jak jest wybrana.
  Armia która została wybrana jest wybrana aż do momentu:
  * wybrania innej armii
  * kliknięcia gdzieś lewym przyciskiem myszy
### Zmiana centrum armii
  Gdy któraś z armii została wybrana możliwym staje się zmiana centrum armii.
  Centrum armii można zmienić klikając gdzieś prawym przyciskiem myszki.
  Zmiana centrum armii powoduje rozpoczęcie przemieszczania się armii w jego stronę.
### Wybór celu ataku
  Gdy któraś z armii została wybrana możliwym staje się wybór celu ataku.
  Cel ataku można wybrać poprzez kliknięcie prawym przyciskiem myszy na jedną z wrogich jednostek.
  Powoduje to obranie za cel całej armii przeciwnika.
### Zderzenia
  Jeśli dana jednostka zderzy się z inną jednostką, to następuje zderzenie sprężyste
### Kierowanie się do celu
  Każda jednostka posiada pozycję względem centrum armii do której dąży.
  Jeśli odległość jednostki od celu jest większa niż jej promień, to ta zaczyna przyspieszać w kierunku docelowej pozycji aż do osiągnięcia maksymalnej prędkości.
  Jeśli odległość jednostki od celu jest mniejsza niż jej promień, to ta zaczyna spowalniać tak aby zatrzymać się docelowej pozycji.
### Walka
  Jeśli armia posiada cel ataku, to jej centrum staje się pozycją pierwszej jednostki wrogiej armii.
  Każda armia może atakować jednocześnie tylko jedną armię. Wiele armii może atakować jednocześnie jedną armię.
  Jeśli armia będąca w stanie spoczynku zostanie zaatakowana przez inną armię, automatycznie atakuje inną armię.
  Jeśli armia atakująca inną armię nie jest atakowana, to atak można anulować poprzez rozkaz kierowania się do pewnego celu.
  
  Wrogie jednostki walczą ze sobą poprzez uderzanie się. Po uderzeniu następuje cooldown trwający 5 sekund w czasie którego jednostka nie może atakować.
  Jednostki w zależności od poziomu zdrowia zmieniają kolor tekstury. Są koloru:
  * Zielonego dla poziomu życia > 66.6%
  * Żółtego dla poziomu życia > 33.3%
  * Czerwonego dla poziomu życia > 0.0%
  Jeśli życie jednostki spadnie poniżej 0.0%, to ta znika.
  Jeśli liczba jednostek w armii osiągnie 0, to armia przestaje istnieć, a przycisk z nią związany znika
## Użyte wzorce projektowe
  Klasy `VulkanTools` i `Model` stanowią fasady.
  Klasy `ModelBuilder`, `fighterRange`, `fighterAttack` i `fighterDestination` reprezentują wzorzec Builder
## Diagram Klas
![Wzorce Projektowe](https://github.com/user-attachments/assets/7393aacf-5bfe-44ac-adb9-739fc06070db)