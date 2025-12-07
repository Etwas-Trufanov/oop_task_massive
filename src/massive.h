#pragma once

#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>

// Пространство имён
namespace ars {
    // Класс массива чисел
    template<typename T>
    requires std::is_arithmetic_v<T>
    class array {
        // Данные и методы доступные только методам класса
        private:
            // Указатель на массив
            T *data = nullptr;
            // Размер массива
            std::size_t size = 0;
            // Процедура добавления элемента
            // value - значение
            void add_value(T value) {
                // Переносим указатель старого массива в новую переменную
                T *old_data = data;
                // Выделяем память под новый массив
                data = new T[size+1];
                // Копируем
                for (std::size_t i = 0; i < size; i++) {
                    data[i] = old_data[i];
                }
                // Увеличиваем переменную хранящую кол-во элементов
                size++;
                // Записываем значение
                data[size-1] = value;
                // Если указатель на старый массив не пуст - чистим его
                if (old_data != nullptr) {
                    delete[] old_data;
                }
            }
            // Процедура выделения памяти
            // quantity - количество
            void alloc_mem(std::size_t quantity) {
                data = new T[quantity];
                size = quantity;
                for (std::size_t i = 0; i < size; i++) {
                    data[i] = 0;
                }
            }
        public:
        // Методы работы с классом
            // Процедура загрузки данных из файла
            // filename - имя файла источника
            // При ошибке загрузки бросается исключение
            void load_from_file(const std::string& filename) {
                std::ifstream f(filename);
                if (!f.is_open()) { // Проверяем и бросаем исклюючение если файл не открылся
                    throw std::invalid_argument("File does not exist: " + filename);
                }

                // Очистка старых данных
                if (data != nullptr) {
                    delete[] data;
                    data = nullptr;
                    size = 0;
                }

                T value;
                while (true) {
                    if (!(f >> value)) {            // попытка чтения значения
                        if (f.eof()) break;         // файл закончился
                        throw std::invalid_argument(    // Если в файле что-то навалено
                            "Invalid data in file: " + filename
                        );
                    }
                    add_value(value);
                }
            }


            // Процедура заполнения массива сгенерированными значениями
            // quantity - количество
            // from_value - значение от, to_value значение до
            void load_rand_range(std::size_t quantity, T from_value, T to_value) {

                // Очистка старых данных
                if (data != nullptr) {
                    delete[] data;
                    data = nullptr;
                    size = 0;
                }

                alloc_mem(quantity);    // Выделяем память

                std::random_device rd;
                std::mt19937 gen(rd());

                // Если T — целочисленный тип то integer distribution
                if constexpr (std::is_integral_v<T>) {
                    std::uniform_int_distribution<T> dis(from_value, to_value);
                    for (std::size_t i = 0; i < quantity; i++) {
                        data[i] = dis(gen);
                    }
                }
                // Иначе real distribution
                else {
                    std::uniform_real_distribution<T> dis(from_value, to_value);
                    for (std::size_t i = 0; i < quantity; i++) {
                        data[i] = dis(gen);
                    }
                }
            }

            // Процедура загрузки значений из пользовательского ввода
            // input_massive - на самом деле вектор, а не массив входных значений
            void load_direct_input(std::vector<T> &input_massive) {
                for (std::size_t i = 0; i < input_massive.size(); i++) {
                   add_value(input_massive[i]);
               }
            }
            // Функция, возвращает ответ напрямую
            T calc_func() {
                T tmp = 0;
                int cur_mult = -1;
                const int mult = -1;
                for (std::size_t i = 0; i < size; i++) {
                    cur_mult *= mult;
                    tmp += data[i]*cur_mult;
                }
                return tmp;
            }
            // Процедура записи ответа в файл
            // filename - имя файла
            // При ошибке бросается исключение
            void calc_func_to_file(std::string filename) {
                // Создаём файл
                std::ofstream f(filename);
                // Проверяем файл на открытость
                if (f.is_open()) {
                    // Записываем
                    f << calc_func() << std::endl;
                } else {
                    // Если не открыт то бросаем исключения
                    throw std::invalid_argument("Can't create file: " + filename);
                }
                // Закрываем файл
                f.close();
            }
            // Процедура вывода массива
            void print_array() const {
                for (std::size_t i = 0; i < size; i++) {
                    std::cout << data[i] << " ";
                }
                std::cout << std::endl;
            }
            // Деструктор класса
            ~array() {
                // Если в data содержатся данные, то очищаем
                if (data != nullptr) {
                    delete[] data;
                }
            }
    };
}
