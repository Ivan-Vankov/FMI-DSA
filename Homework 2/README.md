﻿# Сортировки
Имате данни за изпити проведени във ФМИ, които са ви дадени като текстов файл, във формат CSV. Първият ред на CSV файла съответства на заглавния ред на таблицата. В него са записани имената на колоните. Ще считаме, че таблицата има следните колони:
*   `name`: имената на студента, явил се на изпита;
*   `age`: възраст на студента;
*   `subject`: дисциплина;
*   `grade`: оценка от изпита по съответната дисциплина (цяло число, от 2 до 6);
*   `date`: дата, на която се е провел изпитът.

Следващите редове на файла съдържат данните на таблицата. На всеки ред има точно толкова елементи, колкото са колоните на таблицата.

Програмата ви трябва, зареждайки тези данни, да може да ги представи сортирани, по всяка една от колоните. Данните могат да се сортират във възходящ или низходящ ред. Също така програмата ви трябва да може да използва няколко различни алгоритъма за сортиране, спрямо колоните, по които сортирате.

Няма ограничение за точния брой и вида на сортиращите алгоритми, но като минимум трябва да реализирате:

*   Поне една `“стабилна”` и поне една `“нестабилна”` сортировка;
*   Поне една сортировка, която `не използва сравнение` между елементите;
*   Две сортировки, които имат сложност по време `O(nlogn)`в общия случай;
*   Поне една сортировка, която има `най-лоша` сложност по време `O(nlogn)`;
*   Една сортировка, която има `O(n)` сложност по време;
*   Да се реализира една сортировка със сложност по време `O(nlogn)`, която  работи без да използва допълнителна памет (`in-place`).
*   Всяка от реализираните сортировки да може да сортира в произволен ред (или низходящ, или възходящ);

Например може да реализирате един алгоритъм за сортиране, който да се използва за имената, друг - за годините (такъв, че да е подходящ за тези числа) и т.н.

На програмата ви ще се подават аргументи от стандартния вход: 
*   път до файла 
*   колона, по която ще сортирате `(name, age, subject, grade, date)`
*   параметър за това в какъв ред ще са сортирани данните `(asc, desc)`

При решаването на задачата трябва сами да реализирате всички алгоритми и структури от данни, които ще използвате – както сортировките, така и контейнера (или контейнерите, ако са различни), в който ще пазите информация по време на работата на програмата.

### Пример за съдържанието на входен файл:
```
name, age, subject, grade, date
Pesho Petrov,22,DS,6,2017-03-01
Gosho Ivanov,20,LA,3,2016-09-15
Ivan Todorov,23,SDP,5,2007-10-30
Stamat Georgiev,29,LP,3,2020-02-29
Haralampi Kolev,23,KM,6,2017-12-30
Gosho Zdravkov,21,DAA,4,2016-09-15
```
### Примери за изход при съответната команда:
```
> data.csv age asc
Gosho Ivanov,20,LA,3,2016-09-15
Gosho Zdravkov,21,DAA,4,2016-09-15
Pesho Petrov,22,DS,6,2017-03-01
Ivan Todorov,23,SDP,5,2007-10-30
Haralampi Kolev,23,KM,6,2017-12-30
Stamat Georgiev,29,LP,3,2020-02-29

> data.csv name desc
Stamat Georgiev,29,LP,3,2020-02-29
Pesho Petrov,22,DS,6,2017-03-01
Ivan Todorov,23,SDP,5,2007-10-30
Haralampi Kolev,23,KM,6,2017-12-30
Gosho Zdravkov,21,DAA,4,2016-09-15
Gosho Ivanov,20,LA,3,2016-09-15
```