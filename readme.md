# Zadanie zaliczeniowe

Zamiast potoków - kolejki komunikatów.

Odpowiednik systemowy (kinda): `find ./ -type f | xargs wc`

Oddanie projektu:

1. Wysłanie maila z kodem i objaśnieniem
1. Obrona projektu na zoomie

## Program główny

1. Stwórz kolejki komunikatów dla wątków (main-> finder, finder->proc's, proc's->counter 2x)
1. Stwórz wątek finder
1. Stwórz N wątków proc
1. Stwórz wątek counter
1. Poczekaj na zakończenie działania przez wątki (`pthread_join`)

## Wątek `finder`

1. Przglądaj rekurencyjnie drzewo katalogów
   - Każdy napotkany plik wysyłaj na kolejke do `proc`
   - podnieś semafor o jeden dla kazðego przesłanego pliku
1. Zakończ działanie

## Wątek `proc`

1. Spróbuj odczytać kolejny plik z kolejki
   - opuść mutex dostępu do kolejki
   - odczytuj znak po znaku, aż do osiągnięcia `'\n'`
   - opuśc mutex liczby plików w kolejki
   - opuść mutex dostępu do kolejki
     - jeżeli odczytano kumunikat końca przetwarzania przęslij do `counter` informacje o końcu i zakończ działanie wątku
2. odczytaj plik, przelicz znaki itp
3. prześlij wyniki na potoki do `counter` (osobno liczbe znakow, osobno liczbe linii)
   - opuść mutex dostępu do kolejki
   - wyśllij wyniki
   - podnieś mutex liczby wyników w kolejki
   - podnieś mutex dostępu do kolejki

## Wątek `counter`

1. Spróbuj odczytać kolejny wynik z kolejki (osobno liczba znaków, osobno liczba linii)
   - odcztuj znak po znaku aż do `'\n'`
   - jeżeli odczytano informacje o końcu przetwarzania tyle razy, ile jest logicznych procesorów, zakończ działanie wypisując na ekran wynik
2. dodaj do obecnego wyniku otrzyamne dane
