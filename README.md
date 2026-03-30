# lab1 square matrix with int and complex

Учебный проект на C: квадратные матрицы с поддержкой двух типов элементов:

- `int`
- `complex` (`a + bi`)

Операции:

- сложение матриц
- умножение матриц
- умножение матрицы на скаляр

## Структура проекта

- `main.c` - консольная программа
- `matrix.h/.c` - универсальная матрица
- `field_info.h/.c` - таблица операций для типа (runtime polymorphism)
- `complex_number.h/.c` - операции с комплексными числами
- `tests.h/.c` - unit-тесты
- `test_runner.c` - точка входа для запуска тестов
- `CMakeLists.txt` - сборка через CMake

## Сборка и запуск (CMake)

```powershell
cmake -S . -B build
cmake --build build
.\build\lab1.exe
```

## Запуск тестов

```powershell
.\build\lab1_tests.exe
```

или через CTest:

```powershell
ctest --test-dir build --output-on-failure
```

## Быстрая сборка через GCC (без CMake)

```powershell
gcc -std=c11 -Wall -Wextra -pedantic main.c matrix.c field_info.c complex_number.c -o main.exe
.\main.exe
```
