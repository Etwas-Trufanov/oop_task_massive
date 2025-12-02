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
    class array {
        // Данные и методы доступные только методам класса
        private:
            // Указатель на массив
            T *data = nullptr;
            // Размер массива
            std::size_t size = 0;
            // Процедура добавления элемента
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
            void load_from_file(std::string filename) {
                // Грузим файл
                std::ifstream f(filename);
                // Временная строка
                std::string cur_str;
                // Проверяем файл на открытие
                if (f.is_open()) {
                    // Пока файл не закончится считываем
                    while (std::getline(f, cur_str)) {
                        add_value(std::stod(cur_str));
                    }
                } else {
                    // Если файл не открыт, то бросаем исключение
                    throw std::invalid_argument("File non exist: "+filename);
                }
                // Закрываем файл
                f.close();
            }

            // Процедура заполнения массива сгенерированными значениями
            // quantity - количество
            // from_value - значение от, to_value значение до
            void load_rand_range(std::size_t quantity, T from_value, T to_value) {
                // Вообще как я почитал random_device позволяет считывать карту теплового шума с процессора
                std::random_device rd;
                std::mt19937 gen(rd());
                // Устанавливаем диапазон
                std::uniform_real_distribution<> dis(from_value, to_value);
                // Выделяем память под числа
                alloc_mem(quantity);
                // Заполняем
                for (std::size_t i = 0; i < quantity; i++) {
                    data[i] = dis(gen);
                }
            }
            // Процедура загрузки значений из пользовательского ввода
            void load_direct_input(std::vector<T> input_massive) {
                for (int i = 0; i < input_massive.size(); i++) {
                   add_value(input_massive[i]);
               }
            }
            // Функция, возвращает ответ напрямую
            T calc_func() {
                T tmp;
                int cur_mult = -1;
                const int mult = -1;
                for (std::size_t i = 0; i < size; i++) {
                    cur_mult *= mult;
                    tmp += data[i]*cur_mult;
                }
                return tmp;
            }
            // Процедура записи ответа в файл
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
            void print_array() {
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
