# console-calculator
A simple math interpreter and equation solver

Это консольный калькулятор. 
Он понимает дроби через точку (например 123.456), некоторые математические функции, основные математические операции и даже умеет решать уравнения одной переменной численным методом.
Поддерживает операции: + - * / ^ (возведение в степень, в том числе дробную и/или отрицательную)
Поддерживает функции: sin(x), cos(x), ln(x); Где х - это или число, или переменная. ln - это натуральный логарифм. В принципе, к этому списку довольно просто будет добавить и другие функции.
команда /exit - выход из программы

ВАЖНО: без скобок функции не используются. Никаких sinx (это будет расценено интерпретатором как переменная с именем "sinx", а само выражение будет считаться уравнением).
Пример ВЕРНОЙ записи: sin(x), sin(2*ln(x)), sin(cos(x)^2), cos(x^2), cos(x)^2, 2*sin(3.14/4)/267, 2*x и так далее.
Пример ОШИБОЧКОЙ записи: sinx (уже рассмотрено выше), sin2x, sin^2(x), 2ln(10). Между операндами всегда должен быть оператор, т.е. 2x это неверно. Используйте 2*x.

Про запуск с параметрами
При запуске с параметрами необходимо ввести ВСЕ 5 параметров. Если хотите изменить параметры решателя, придётся перезапускать программу.
Для запуска с параметрами вводите в консоли ./calculator_win.exe <точность отделения корней> <точность решения> <левая граница> <правая граница> <метод> или ./calculator с теми же параметрами, в зависимости от вашей операционной системы.
Параметры следующие: 
<точность отделения корней> - число типа double, задающее шаг метода отделения корней
<точность решения> - число типа double, задающее точность решения уравнения
<левая граница> и <правая граница> - числа типа double, определяют интервал поиска корней уравнения
<метод> - принимает два значения: f или r. f означает быстрый метод решения, при этом в уравнениях вида sin(x)^2 = 0 возможна потеря корней. r означает точный метод решения, при этом в уравнениях вида sin(x)^2 = 0 возможно нахождение нескольких одинаковых (близких по значению) корней. Возможно, для некоторых уравнений придётся подбирать параметр <точность решения>, и с этим пользователю придётся смириться.

Про решение уравнений
Если во введенном выражении появляется что-то символьное, при этом не являющееся sin, cos или ln, то это воспринимается как переменная. Переменная может быть только одна. Имя - любое, написанное латинницей.
Максимальная длина имени - 33 или около того символов. Нахождение в выражении переменной автоматически делает его уравнением. Таким образом, выражение: abracadabra*2 = 25 означает, что у нас уравнение с переменной "abracadabra".
Решатель умеет решать только уравнения с одной переменной. Нахождение во введенном выражении двух и более переменных расценивается как ошибка ввода и приводит к выходу из программы без каких-либо сообщений.
Если вводится выражения без знака "=", т.е. например: 12*х - 45*ln(2*x), то оно автоматически преобразуется к виду 12*х - 45*ln(2*x) = 0 и решаться будет именно такое уравнение.
Уравнение решается единственным доступным в программе численным методом: методом хорд. Если решение вашего уравнения не было найдено, пробуйте менять точность, метод (f/r - пятый параметр запуска в консоли) и/или заданный интервал поиска.

Проверка программы при помощи valgrind показала отсутствие утечек памяти. 

Известные проблемы
Не использовать выражения вида: 1 + 2/-3, вместо этого пишите: 1 + 2/(-3). Впрочем, если для вас такая запись: 1 + 2/(-3) предпочтительнее такой: 1 - 2/3...
