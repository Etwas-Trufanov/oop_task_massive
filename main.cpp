#include <stdexcept>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "massive.h"

//

// Строка содежащая строку ошибки аргументов
const std::string input_arg_error = ("Wrong input arguments\nArg's example: -gen quantity range_from range_to\nOr: -f filename\nOr: -d (for direct input)");
const std::string output_arg_error = ("Wrong output arguments\nArg's example: -of fileout.txt\n");

// Считывает пользоватеьмкий ввод до "stop" и возвращает вектор
std::vector<double> get_user_input() {
    // Временный вектор ввода
    std::vector<double> tpm;
    std::string value = "0";
    // Цикл ввода
    while (true) {
        std::cin >> value;
        // Проверка на stop
        if (value == "stop") {
            break;
        }
        tpm.push_back(stod(value));
    }
    return tpm;
}

// Порядок аргументов:
// 0 - программа
// 1 - режим ввода -f/-d (для -f после идёт имя входного файла)
// 2 - режим вывода пустой для простого вывода, -of для вывода в файл (после -of идём имя файла результата)
int main(int argc, char **argv) {

    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << "|";
    }
    std::cout << std::endl;
    // тут ассерт, выключается ключом -DNDEBUG
    {
        ars::array<double> *assert_test = new ars::array<double>;
        std::vector<double> tmp = {2, 2, 3};
        assert_test->load_direct_input(tmp);
        assert(assert_test->calc_func() == 3);
        delete assert_test;
    }


    // Экземпляр класса массива
    ars::array<double> array;
    // Переменная хранящая смещение от аргументов ввода
    int output_point = 0;

    // Проверяем откуда взять значения
    if (argc >= 2) {
        if (strcmp(argv[1], "-d") == 0) {
            // Пользовательский ввод
            std::cout << "Enter values, 'stop' for end:\n";
            array.load_direct_input(get_user_input());
            output_point = 2;
        }
    } else if (argc >= 3) {
        if (strcmp(argv[1], "-f") == 0) {
            // Если из файла
            try {
                // Загружаем из файла
                array.load_from_file(argv[2]);
                // Устанавливаем точку смещения
                output_point = 3;
            } catch (const std::invalid_argument &e) {
                // При возникновлении исключения, выводим его и завершаем программу.
                std::cout << &e << std::endl;
                return 1;
            }
        } else if ((strcmp(argv[1], "-gen") == 0) and (argc >= 5)) {
            // Если случайные числа
            array.load_rand_range(std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]));
            // Устанавливаем точку смещения
            output_point = 5;
        }
    } else {
        // Если аргументы не верные, то выводим ошибку и завершаем программу
        std::cout << input_arg_error;
        return 1;
    }

    // Если количество аргументов совпало с точкой смещения, т.е. аргументов на вывод нет
    if (output_point == argc) {
        // Просто выводим получившееся число
        std::cout << array.calc_func() << std::endl;
    // Если есть аргументы отвечающие за запись в файл - проверяем их
    } else if ((strcmp(argv[output_point], "-of") == 0) and (argc == output_point+2)) {
        // Пробуем записать
        try {
            array.calc_func_to_file(argv[output_point+1]);
        } catch (const std::invalid_argument &e) {
            // При исключении выводим ошибку и завершаем программу
            std::cerr << "An exception was encountered: " << e.what() << std::endl;
            return 1;
        }
    } else {
        // Если пользователь ввёл что-то не понятное
        std::cout << output_arg_error;
    }

    return 0;
}
