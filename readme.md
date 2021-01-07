# Zadanie zaliczeniowe

Zamiast potoków - kolejki komunikatów.

## Program główny

1. Stwórz potoki (nie nazwane) dla wątków (finder->proc's, proc's->counter)
1. Stwórz wątek finder
1. Stwórz N wątków proc
1. Stwórz wątek counter
1. Poczekaj na zakończenie działania przez wątki (`pthread_join`)

## Wątek `finder`

1. Przglądaj rekurencyjnie drzewo katalogów
   - Każdy napotkany plik wysyłaj na potok do `proc`
   - podnieś mutex o jeden dla kazðego przesłąnego pliku
1. Zakończ działanie

## Wątek `proc`

1. Spróbuj odczytać kolejny plik z potoku
   - opuść mutex dostępu do potoku
   - odczytuj znak po znaku, aż do osiągnięcia `'\n'`
   - opuśc mutex liczby plików w potoku
   - opuść mutex dostępu do potoku
     - jeżeli odczytano kumunikat końca przetwarzania przęslij do `counter` informacje o końcu i zakończ działanie wątku
2. odczytaj plik, przelicz znaki itp
3. prześlij wyniki na potoki do `counter` (osobno liczbe znakow, osobno liczbe linii)
   - opuść mutex dostępu do potoku
   - wyśllij wyniki
   - podnieś mutex liczby wyników w potoku
   - podnieś mutex dostępu do potoku

## Wątek `counter`

1. Spróbuj odczytać kolejny wynik z potoków (osobno liczba znaków, osobno liczba linii)
   - odcztuj znak po znaku aż do `'\n'`
   - jeżeli odczytano informacje o końcu przetwarzania tyle razy, ile jest logicznych procesorów, zakończ działanie wypisując na ekran wynik
2. dodaj do obecnego wyniku otrzyamne dane
