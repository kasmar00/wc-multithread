# Wielowątkowe zliczanie znaków i linii

Projekt zaliczeniowy na laboratorium z Programowania Systemowego i Współbieżnego

Informatyka 3 sem, Wydział Informatyki i Telekomunikacji, Politechnika Poznańska 2020/21.

Prowadzący zajęcia: [Cezary Sobaniec](https://www.cs.put.poznan.pl/csobaniec/)

Celem projektu było przygotowanie wielowątkowego programu, który przelicza znaki i linie w plikach zanjdujacych się w zadanym katalogu, o zadanych rozszerzeniach. Pełne polecenie dostępne jest w pliku `psw-zaliczenie.pdf` (jako zadanie za 16 punktów).

## Uruchomienie

1. Skompilować poleceniem `$ make`

   (W przypadku problemów zwrócić uwagę na flagę `lpthread` vs `pthread`)

2. Uruchomić wywołując `$ ./main.out <ścieżka> <rozszerzenia>`

   np. `$ ./main.out ./ c h`

   alternatywnie można uruchomić poleceniem `$ make run` (wywołanie odpowiada `$ ./main.out ./ c h`)

# Multithreaded Word Counter

Final project for laboratory in System Programming and Concurent Programming.

Computing 3 sem, Faculty of Computing and Telecomunications Poznań Univeristy of Technology 2020/2021.

Goal of the project was to create a

Teacher: [Cezary Sobaniec](https://www.cs.put.poznan.pl/csobaniec/)

## Running

1. Run the file `$ make`

   (In case of problems check the `-pthread` vs `-lpthread` flag)

2. Run using `$ ./main.out <path> <extensions>`

   example: `$ ./main.out ./ c h`

   Alternatuvely you can use the provided makefile and run `$ make run` (which corresponds to `$ ./main.out ./ c h`)
