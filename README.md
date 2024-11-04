# Szablon projektu C++

- Pliki źródłowe (`.cpp`) dodawaj do folderu `src`.
- Pliki nagłówkowe (`.hpp`) dodawaj do folderu `include`.
- Jeżeli do prawidłowego działania projektu potrzebne są dodatkowe pliki wsadowe (np. dane w arkuszu kalkulacyjnym), stwórz nowy katalog `data` i dodaj je tam. **Nie commituj plików powstałych w czasie kompilacji do repozytorium.**
- Jeżeli kompilujesz w oparciu o CMake'a, pliki źródłowe są automatycznie wykrywane w katalogu `src`, nie musisz ręcznie modyfikować pliku `CMakeLists.txt`.
- Postaraj się zachować schludne formatowanie kodu. Pomocny może okazać się przykładowo podany plik `.clang-format`.
- Postaraj się zachować konwencję nazewnictwa pól klas, tj. prefiks "`m_`" albo sufiks "`_`".
- Postaraj się zachować wybraną konwencję nazewnictwa zmiennych, klas i funkcji, np. "`NazwaKlasy`", "`nazwaFunkcji`", "`nazwa_zmiennej`". Nie jest ważne jakiej konwencji użyjesz - ważne jest, aby stosować ją konsystentnie.
