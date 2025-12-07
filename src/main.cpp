#include <stdexcept>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "massive.h"

//

// Строка содержащая строку ошибки аргументов
const std::string input_arg_error = ("Wrong input arguments\nArg's example: -gen quantity range_from range_to\nOr: -f filename\nOr: -d (for direct input)\n");
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
        if (value == "stop" or value == "q") {
            break;
        }
        // Отлавливаем мусор пользователя
        try {
            tpm.push_back(std::stod(value));
        } catch (...) {
            std::cerr << "Invalid number, try again\n";
        }
    }
    return tpm;
}

// Порядок аргументов:
// 0 - программа
// 1 - режим ввода -f/-d (для -f после идёт имя входного файла)
// 2 - режим вывода пустой для простого вывода, -of для вывода в файл (после -of идём имя файла результата)
int main(int argc, char **argv) {

    // тут ассерты, выключается ключом -DNDEBUG
    {
        ars::array<double> *assert_test = new ars::array<double>;
        std::vector<double> tmp = {2, 2, 3};
        assert_test->load_direct_input(tmp);
        assert(assert_test->calc_func() == 3);
        delete assert_test;
    }

    {
        ars::array<double> *assert_test = new ars::array<double>;
        std::vector<double> tmp = {10, 10, 20, 30, 20, 3};
        assert_test->load_direct_input(tmp);
        assert(assert_test->calc_func() == 7);
        delete assert_test;
    }
    {
        ars::array<double> *assert_test = new ars::array<double>;
        std::vector<double> tmp = {5, 6, 7};
        assert_test->load_direct_input(tmp);
        assert(assert_test->calc_func() == 6);
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
            std::cout << "Enter values, 'stop' or 'q' for end:\n";
            std::vector<double> user_input;
            // Мучаем пользователя пока не введёт норм вектор
            while (user_input.size() == 0) {
                user_input = get_user_input();
                // Если пользователь навалил кринжа (ничего не ввёл)
                if (user_input.size() == 0) {
                    std::cout << "Input list is clear, enter values again\n";
                }
            }
            array.load_direct_input(user_input);
            output_point = 2;
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
                    std::cerr << "An exception was encountered: " << e.what() << std::endl;
                    return 1;
                }
            } else if (argc >= 5) {
                if (strcmp(argv[1], "-gen") == 0) {
                    // Если случайные числа

                    // Отлавливаем если пользователь ввёл мусор
                    try {
                        array.load_rand_range(std::stoi(argv[2]), std::stod(argv[3]), std::stod(argv[4]));
                    } catch (const std::invalid_argument &e) {
                        // Выводим ошибку и завершаем выполнение
                        std::cerr << "Entered values not valid!" << std::endl;
                        return 1;
                    }

                    // Устанавливаем точку смещения
                    output_point = 5;
                } else {
                    std::cerr << input_arg_error;
                    return 1;
                }
            }
        }
    } else {
        // Если аргументы не верные, то выводим ошибку и завершаем программу
        std::cerr << input_arg_error;
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
        } catch (const std::runtime_error &e) {
            // При исключении выводим ошибку и завершаем программу
            std::cerr << "An exception was encountered: " << e.what() << std::endl;
            return 1;
        }
    } else {
        // Если пользователь ввёл что-то не понятное
        std::cerr << output_arg_error;
        return 1;
    }

    return 0;
}
