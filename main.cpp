#define NOMINMAX // Додаємо цю директиву перед включенням Windows.h

#include "DynamicCollections.h"
#include <limits> // Для std::numeric_limits
#include <Windows.h> // Для SetConsoleOutputCP

// Функція для очищення буфера введення
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Меню для Stack
template <typename T>
void runStackMenu(Stack<T>& stack, const std::string& typeName) {
    int choice;
    do {
        std::cout << "\n--- Меню для Стеку (" << typeName << ") ---" << std::endl;
        std::cout << "1. Додати елемент (з клавіатури)" << std::endl;
        std::cout << "2. Видалити верхній елемент (pop)" << std::endl;
        std::cout << "3. Переглянути верхній елемент (peek)" << std::endl;
        std::cout << "4. Пошук елементу" << std::endl;
        std::cout << "5. Видалити елементи за значенням" << std::endl;
        std::cout << "6. Вивести всі елементи" << std::endl;
        std::cout << "7. Зберегти у файл" << std::endl;
        std::cout << "8. Завантажити з файлу" << std::endl;
        std::cout << "9. Сортувати" << std::endl;
        std::cout << "0. Повернутися в головне меню" << std::endl;
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        clearInputBuffer(); // Очищаємо буфер після введення числа

        try {
            switch (choice) {
            case 1:
                stack.pushFromKeyboard();
                break;
            case 2:
                stack.pop();
                break;
            case 3:
                std::cout << "Верхній елемент: " << stack.peek() << std::endl;
                break;
            case 4: {
                T key;
                std::cout << "Введіть елемент для пошуку: ";
                std::cin >> key;
                if (stack.search(key)) {
                    std::cout << "Елемент '" << key << "' знайдено в стеку." << std::endl;
                }
                else {
                    std::cout << "Елемент '" << key << "' не знайдено в стеку." << std::endl;
                }
                break;
            }
            case 5: {
                T value;
                std::cout << "Введіть значення елементу для видалення: ";
                std::cin >> value;
                stack.removeByValue(value);
                break;
            }
            case 6:
                stack.display();
                break;
            case 7: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для збереження: ";
                std::getline(std::cin, filename); // Зчитуємо весь рядок
                stack.saveToFile(filename);
                break;
            }
            case 8: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для завантаження: ";
                std::getline(std::cin, filename); // Зчитуємо весь рядок
                stack.loadFromFile(filename);
                break;
            }
            case 9:
                stack.sort();
                break;
            case 0:
                break;
            default:
                std::cout << "Некоректний вибір. Спробуйте ще раз." << std::endl;
                break;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Помилка: " << e.what() << std::endl;
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Помилка виділення пам'яті: " << e.what() << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Помилка файлової операції: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Виникла невідома помилка: " << e.what() << std::endl;
        }
    } while (choice != 0);
}

// Меню для Queue
template <typename T>
void runQueueMenu(Queue<T>& queue, const std::string& typeName) {
    int choice;
    do {
        std::cout << "\n--- Меню для Черги (" << typeName << ") ---" << std::endl;
        std::cout << "1. Додати елемент (з клавіатури)" << std::endl;
        std::cout << "2. Видалити елемент з початку (dequeue)" << std::endl;
        std::cout << "3. Переглянути перший елемент (front)" << std::endl;
        std::cout << "4. Пошук елементу" << std::endl;
        std::cout << "5. Видалити елементи за значенням" << std::endl;
        std::cout << "6. Вивести всі елементи" << std::endl;
        std::cout << "7. Зберегти у файл" << std::endl;
        std::cout << "8. Завантажити з файлу" << std::endl;
        std::cout << "9. Сортувати" << std::endl;
        std::cout << "0. Повернутися в головне меню" << std::endl;
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        clearInputBuffer(); // Очищаємо буфер після введення числа

        try {
            switch (choice) {
            case 1:
                queue.enqueueFromKeyboard();
                break;
            case 2:
                queue.dequeue();
                break;
            case 3:
                std::cout << "Перший елемент: " << queue.front() << std::endl;
                break;
            case 4: {
                T key;
                std::cout << "Введіть елемент для пошуку: ";
                std::cin >> key;
                if (queue.search(key)) {
                    std::cout << "Елемент '" << key << "' знайдено в черзі." << std::endl;
                }
                else {
                    std::cout << "Елемент '" << key << "' не знайдено в черзі." << std::endl;
                }
                break;
            }
            case 5: {
                T value;
                std::cout << "Введіть значення елементу для видалення: ";
                std::cin >> value;
                queue.removeByValue(value);
                break;
            }
            case 6:
                queue.display();
                break;
            case 7: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для збереження: ";
                std::getline(std::cin, filename);
                queue.saveToFile(filename);
                break;
            }
            case 8: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для завантаження: ";
                std::getline(std::cin, filename);
                queue.loadFromFile(filename);
                break;
            }
            case 9:
                queue.sort();
                break;
            case 0:
                break;
            default:
                std::cout << "Некоректний вибір. Спробуйте ще раз." << std::endl;
                break;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Помилка: " << e.what() << std::endl;
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Помилка виділення пам'яті: " << e.what() << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Помилка файлової операції: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Виникла невідома помилка: " << e.what() << std::endl;
        }
    } while (choice != 0);
}

// Меню для DoublyLinkedList
template <typename T>
void runDoublyLinkedListMenu(DoublyLinkedList<T>& list, const std::string& typeName) {
    int choice;
    do {
        std::cout << "\n--- Меню для Двоспрямованого Циклічного Списку (" << typeName << ") ---" << std::endl;
        std::cout << "1. Додати елемент (з клавіатури)" << std::endl;
        std::cout << "2. Пошук елементу" << std::endl;
        std::cout << "3. Видалити елементи за значенням" << std::endl;
        std::cout << "4. Видалити елемент за індексом" << std::endl;
        std::cout << "5. Вивести всі елементи" << std::endl;
        std::cout << "6. Зберегти у файл" << std::endl;
        std::cout << "7. Завантажити з файлу" << std::endl;
        std::cout << "8. Сортувати" << std::endl;
        std::cout << "0. Повернутися в головне меню" << std::endl;
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        clearInputBuffer(); // Очищаємо буфер після введення числа

        try {
            switch (choice) {
            case 1:
                list.addFromKeyboard();
                break;
            case 2: {
                T key;
                std::cout << "Введіть елемент для пошуку: ";
                std::cin >> key;
                if (list.search(key)) {
                    std::cout << "Елемент '" << key << "' знайдено в списку." << std::endl;
                }
                else {
                    std::cout << "Елемент '" << key << "' не знайдено в списку." << std::endl;
                }
                break;
            }
            case 3: {
                T value;
                std::cout << "Введіть значення елементу для видалення: ";
                std::cin >> value;
                list.removeByValue(value);
                break;
            }
            case 4: {
                int index;
                std::cout << "Введіть індекс елементу для видалення: ";
                std::cin >> index;
                list.removeByIndex(index);
                break;
            }
            case 5:
                list.display();
                break;
            case 6: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для збереження: ";
                std::getline(std::cin, filename);
                list.saveToFile(filename);
                break;
            }
            case 7: {
                std::string filename;
                std::cout << "Введіть ім'я файлу для завантаження: ";
                std::getline(std::cin, filename);
                list.loadFromFile(filename);
                break;
            }
            case 8:
                list.sort();
                break;
            case 0:
                break;
            default:
                std::cout << "Некоректний вибір. Спробуйте ще раз." << std::endl;
                break;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Помилка: " << e.what() << std::endl;
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Помилка виділення пам'яті: " << e.what() << std::endl;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Помилка файлової операції: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Виникла невідома помилка: " << e.what() << std::endl;
        }
    } while (choice != 0);
}


int main() {
    // Встановлення кодування для консолі Windows на українське (CP1251)
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251); // Також для вводу

    int mainChoice;
    do {
        std::cout << "\n--- Головне меню ---" << std::endl;
        std::cout << "1. Робота зі Стеком (int)" << std::endl;
        std::cout << "2. Робота з Чергою (float)" << std::endl;
        std::cout << "3. Робота з Двоспрямованим циклічним списком (char)" << std::endl;
        std::cout << "4. Робота зі Стеком (string)" << std::endl;
        std::cout << "5. Робота з Чергою (string)" << std::endl;
        std::cout << "6. Робота з Двоспрямованим циклічним списком (string)" << std::endl;
        std::cout << "0. Вихід" << std::endl;
        std::cout << "Ваш вибір: ";
        std::cin >> mainChoice;

        clearInputBuffer();

        switch (mainChoice) {
        case 1: {
            Stack<int> intStack;
            runStackMenu(intStack, "int");
            break;
        }
        case 2: {
            Queue<float> floatQueue;
            runQueueMenu(floatQueue, "float");
            break;
        }
        case 3: {
            DoublyLinkedList<char> charList;
            runDoublyLinkedListMenu(charList, "char");
            break;
        }
        case 4: {
            Stack<std::string> stringStack;
            runStackMenu(stringStack, "string");
            break;
        }
        case 5: {
            Queue<std::string> stringQueue;
            runQueueMenu(stringQueue, "string");
            break;
        }
        case 6: {
            DoublyLinkedList<std::string> stringList;
            runDoublyLinkedListMenu(stringList, "string");
            break;
        }
        case 0:
            std::cout << "Завершення програми." << std::endl;
            break;
        default:
            std::cout << "Некоректний вибір. Спробуйте ще раз." << std::endl;
            break;
        }
    } while (mainChoice != 0);

    return 0;
}
