#ifndef DYNAMIC_COLLECTIONS_H
#define DYNAMIC_COLLECTIONS_H

#include <iostream>
#include <string>
#include <vector> // Використовуємо для допоміжних операцій, наприклад, сортування
#include <algorithm> // Для std::sort
#include <fstream>   // Для роботи з файлами
#include <stdexcept> // Для обробки винятків
#include <Windows.h> // Для SetConsoleOutputCP

// --- Шаблонний клас Stack (Стек) ---
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* topNode; // Покажчик на верхній елемент стеку
    size_t currentSize; // Поточний розмір стеку

public:
    // Конструктор за замовчуванням
    Stack() : topNode(nullptr), currentSize(0) {}

    // Конструктор копіювання
    Stack(const Stack<T>& other) : topNode(nullptr), currentSize(0) {
        if (other.topNode) {
            std::vector<T> temp;
            Node* current = other.topNode;
            while (current) {
                temp.push_back(current->data);
                current = current->next;
            }
            // Додаємо елементи в зворотному порядку, щоб зберегти порядок стеку
            for (int i = temp.size() - 1; i >= 0; --i) {
                push(temp[i]);
            }
        }
    }

    // Оператор присвоєння
    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {
            // Очищаємо поточний стек
            while (!isEmpty()) {
                pop();
            }
            // Копіюємо елементи з іншого стеку
            if (other.topNode) {
                std::vector<T> temp;
                Node* current = other.topNode;
                while (current) {
                    temp.push_back(current->data);
                    current = current->next;
                }
                for (int i = temp.size() - 1; i >= 0; --i) {
                    push(temp[i]);
                }
            }
        }
        return *this;
    }

    // Деструктор
    ~Stack() {
        while (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    // Додавання елементу (введення з клавіатури)
    void pushFromKeyboard() {
        T value;
        std::cout << "Введіть елемент для додавання в стек: ";
        std::cin >> value;
        push(value);
    }

    // Додавання елементу
    void push(const T& value) {
        Node* newNode = new Node(value);
        if (!newNode) {
            throw std::bad_alloc(); // Виняток, якщо не вдалося виділити пам'ять
        }
        newNode->next = topNode;
        topNode = newNode;
        currentSize++;
        std::cout << "Елемент '" << value << "' додано до стеку." << std::endl;
    }

    // Видалення верхнього елементу
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Стек порожній. Невдала спроба видалення.");
        }
        Node* temp = topNode;
        T data = temp->data;
        topNode = topNode->next;
        delete temp;
        currentSize--;
        std::cout << "Елемент '" << data << "' видалено зі стеку." << std::endl;
        return data;
    }

    // Перегляд верхнього елементу
    T peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Стек порожній. Немає елементів для перегляду.");
        }
        return topNode->data;
    }

    // Перевірка на порожнечу
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // Пошук елементу за ключовим полем
    bool search(const T& key) const {
        Node* current = topNode;
        while (current) {
            if (current->data == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Видалення всіх елементів за заданим значенням ключового поля
    void removeByValue(const T& value) {
        if (isEmpty()) {
            std::cout << "Стек порожній. Немає елементів для видалення." << std::endl;
            return;
        }

        Stack<T> tempStack; // Тимчасовий стек для зберігання елементів, які не потрібно видаляти
        bool found = false;

        while (!isEmpty()) {
            T currentData = pop(); // Видаляємо елемент з поточного стеку
            if (currentData == value) {
                found = true;
                std::cout << "Знайдено та видалено '" << currentData << "' зі стеку." << std::endl;
            }
            else {
                tempStack.push(currentData); // Додаємо елемент, який не співпадає, в тимчасовий стек
            }
        }

        // Повертаємо елементи з тимчасового стеку назад у вихідний
        while (!tempStack.isEmpty()) {
            push(tempStack.pop());
        }

        if (!found) {
            std::cout << "Елемент '" << value << "' не знайдено в стеку." << std::endl;
        }
    }

    // Виведення всього списку на екран
    void display() const {
        if (isEmpty()) {
            std::cout << "Стек порожній." << std::endl;
            return;
        }
        Node* current = topNode;
        std::cout << "Елементи стеку (від вершини до основи):" << std::endl;
        while (current) {
            std::cout << "| " << current->data << " |" << std::endl;
            current = current->next;
        }
        std::cout << "------" << std::endl;
    }

    // Виведення всього списку у типізований файл
    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary); // Бінарний режим
        if (!outFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для запису: " + filename);
        }

        Node* current = topNode;
        // Зберігаємо елементи в зворотному порядку, щоб при завантаженні стек був у правильному порядку
        std::vector<T> temp;
        while (current) {
            temp.push_back(current->data);
            current = current->next;
        }
        for (int i = temp.size() - 1; i >= 0; --i) {
            outFile.write(reinterpret_cast<const char*>(&temp[i]), sizeof(T));
        }

        outFile.close();
        std::cout << "Стек успішно збережено у файл '" << filename << "'." << std::endl;
    }


    // Введення всього списку із типізованого файлу
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary); // Бінарний режим
        if (!inFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для читання: " + filename);
        }

        // Очищаємо поточний стек перед завантаженням
        while (!isEmpty()) {
            pop();
        }

        T value;
        // Читаємо елементи та додаємо їх до стеку
        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            push(value);
        }

        inFile.close();
        std::cout << "Стек успішно завантажено з файлу '" << filename << "'." << std::endl;
    }

    // Сортування (для стеку це нестандартна операція, але реалізуємо через тимчасовий вектор)
    void sort() {
        if (isEmpty() || currentSize == 1) {
            std::cout << "Стек порожній або містить один елемент, сортування не потрібне." << std::endl;
            return;
        }

        std::vector<T> tempVec;
        while (!isEmpty()) {
            tempVec.push_back(pop()); // Вивантажуємо елементи в вектор
        }

        // Використовуємо бульбашкове сортування
        for (size_t i = 0; i < tempVec.size() - 1; ++i) {
            for (size_t j = 0; j < tempVec.size() - 1 - i; ++j) {
                if (tempVec[j] > tempVec[j + 1]) {
                    std::swap(tempVec[j], tempVec[j + 1]);
                }
            }
        }

        // Завантажуємо відсортовані елементи назад у стек (зберігаючи порядок стеку)
        // Елементи повинні бути завантажені у зворотному порядку, щоб найменший був зверху
        for (int i = tempVec.size() - 1; i >= 0; --i) {
            push(tempVec[i]);
        }
        std::cout << "Стек відсортовано (бульбашкове сортування)." << std::endl;
    }
};

// --- Шаблонний клас Queue (Черга) ---
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* frontNode; // Покажчик на початок черги
    Node* rearNode;  // Покажчик на кінець черги
    size_t currentSize; // Поточний розмір черги

public:
    // Конструктор за замовчуванням
    Queue() : frontNode(nullptr), rearNode(nullptr), currentSize(0) {}

    // Конструктор копіювання
    Queue(const Queue<T>& other) : frontNode(nullptr), rearNode(nullptr), currentSize(0) {
        Node* current = other.frontNode;
        while (current) {
            enqueue(current->data);
            current = current->next;
        }
    }

    // Оператор присвоєння
    Queue<T>& operator=(const Queue<T>& other) {
        if (this != &other) {
            // Очищаємо поточну чергу
            while (!isEmpty()) {
                dequeue();
            }
            // Копіюємо елементи з іншої черги
            Node* current = other.frontNode;
            while (current) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Деструктор
    ~Queue() {
        while (frontNode) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
        rearNode = nullptr; // Запобігання висячому покажчику
    }

    // Додавання елементу (введення з клавіатури)
    void enqueueFromKeyboard() {
        T value;
        std::cout << "Введіть елемент для додавання в чергу: ";
        std::cin >> value;
        enqueue(value);
    }

    // Додавання елементу в кінець черги (enqueue)
    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (!newNode) {
            throw std::bad_alloc();
        }
        if (isEmpty()) {
            frontNode = newNode;
            rearNode = newNode;
        }
        else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        currentSize++;
        std::cout << "Елемент '" << value << "' додано до черги." << std::endl;
    }

    // Видалення елементу з початку черги (dequeue)
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Черга порожня. Невдала спроба видалення.");
        }
        Node* temp = frontNode;
        T data = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr) { // Якщо черга стала порожньою
            rearNode = nullptr;
        }
        delete temp;
        currentSize--;
        std::cout << "Елемент '" << data << "' видалено з черги." << std::endl;
        return data;
    }

    // Перегляд першого елементу
    T front() const {
        if (isEmpty()) {
            throw std::out_of_range("Черга порожня. Немає елементів для перегляду.");
        }
        return frontNode->data;
    }

    // Перевірка на порожнечу
    bool isEmpty() const {
        return frontNode == nullptr;
    }

    // Пошук елементу за ключовим полем
    bool search(const T& key) const {
        Node* current = frontNode;
        while (current) {
            if (current->data == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Видалення всіх елементів за заданим значенням ключового поля
    void removeByValue(const T& value) {
        if (isEmpty()) {
            std::cout << "Черга порожня. Немає елементів для видалення." << std::endl;
            return;
        }

        Queue<T> tempQueue; // Тимчасова черга для зберігання елементів, які не потрібно видаляти
        bool found = false;

        while (!isEmpty()) {
            T currentData = dequeue(); // Видаляємо елемент з поточної черги
            if (currentData == value) {
                found = true;
                std::cout << "Знайдено та видалено '" << currentData << "' з черги." << std::endl;
            }
            else {
                tempQueue.enqueue(currentData); // Додаємо елемент, який не співпадає, в тимчасову чергу
            }
        }

        // Повертаємо елементи з тимчасової черги назад у вихідну
        while (!tempQueue.isEmpty()) {
            enqueue(tempQueue.dequeue());
        }

        if (!found) {
            std::cout << "Елемент '" << value << "' не знайдено в черзі." << std::endl;
        }
    }

    // Виведення всього списку на екран
    void display() const {
        if (isEmpty()) {
            std::cout << "Черга порожня." << std::endl;
            return;
        }
        Node* current = frontNode;
        std::cout << "Елементи черги (від початку до кінця):" << std::endl;
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Виведення всього списку у типізований файл
    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для запису: " + filename);
        }

        Node* current = frontNode;
        while (current) {
            outFile.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
            current = current->next;
        }

        outFile.close();
        std::cout << "Чергу успішно збережено у файл '" << filename << "'." << std::endl;
    }

    // Введення всього списку із типізованого файлу
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для читання: " + filename);
        }

        // Очищаємо поточну чергу перед завантаженням
        while (!isEmpty()) {
            dequeue();
        }

        T value;
        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            enqueue(value);
        }

        inFile.close();
        std::cout << "Чергу успішно завантажено з файлу '" << filename << "'." << std::endl;
    }

    // Сортування (для черги це нестандартна операція, але реалізуємо через тимчасовий вектор)
    void sort() {
        if (isEmpty() || currentSize == 1) {
            std::cout << "Черга порожня або містить один елемент, сортування не потрібне." << std::endl;
            return;
        }

        std::vector<T> tempVec;
        while (!isEmpty()) {
            tempVec.push_back(dequeue()); // Вивантажуємо елементи в вектор
        }

        // Використовуємо бульбашкове сортування
        for (size_t i = 0; i < tempVec.size() - 1; ++i) {
            for (size_t j = 0; j < tempVec.size() - 1 - i; ++j) {
                if (tempVec[j] > tempVec[j + 1]) {
                    std::swap(tempVec[j], tempVec[j + 1]);
                }
            }
        }

        // Завантажуємо відсортовані елементи назад у чергу
        for (const T& val : tempVec) {
            enqueue(val);
        }
        std::cout << "Чергу відсортовано (бульбашкове сортування)." << std::endl;
    }
};

// --- Шаблонний клас DoublyLinkedList (Двоспрямований циклічний список) ---
template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head; // Покажчик на перший елемент
    size_t currentSize; // Поточний розмір списку

public:
    // Конструктор за замовчуванням
    DoublyLinkedList() : head(nullptr), currentSize(0) {}

    // Конструктор копіювання
    DoublyLinkedList(const DoublyLinkedList<T>& other) : head(nullptr), currentSize(0) {
        if (other.head) {
            Node* current = other.head;
            do {
                addAtEnd(current->data);
                current = current->next;
            } while (current != other.head);
        }
    }

    // Оператор присвоєння
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other) {
        if (this != &other) {
            // Очищаємо поточний список
            while (!isEmpty()) {
                removeByIndex(0); // Видаляємо перший елемент
            }
            // Копіюємо елементи з іншого списку
            if (other.head) {
                Node* current = other.head;
                do {
                    addAtEnd(current->data);
                    current = current->next;
                } while (current != other.head);
            }
        }
        return *this;
    }

    // Деструктор
    ~DoublyLinkedList() {
        if (head) {
            Node* current = head->next;
            while (current != head) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            delete head; // Видаляємо останній елемент (head)
        }
    }

    // Перевірка на порожнечу
    bool isEmpty() const {
        return head == nullptr;
    }

    // Додавання елементу (введення з клавіатури)
    void addFromKeyboard() {
        T value;
        std::cout << "Введіть елемент для додавання в список: ";
        std::cin >> value;
        int choice;
        std::cout << "Куди додати? (1 - на початок, 2 - в кінець, 3 - за індексом): ";
        std::cin >> choice;
        switch (choice) {
        case 1:
            addAtBeginning(value);
            break;
        case 2:
            addAtEnd(value);
            break;
        case 3:
            int index;
            std::cout << "Введіть індекс: ";
            std::cin >> index;
            addAtIndex(value, index);
            break;
        default:
            std::cout << "Некоректний вибір. Додано в кінець." << std::endl;
            addAtEnd(value);
            break;
        }
    }

    // Додавання елементу на початок
    void addAtBeginning(const T& value) {
        Node* newNode = new Node(value);
        if (!newNode) {
            throw std::bad_alloc();
        }
        if (isEmpty()) {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else {
            Node* last = head->prev;
            newNode->next = head;
            newNode->prev = last;
            head->prev = newNode;
            last->next = newNode;
            head = newNode;
        }
        currentSize++;
        std::cout << "Елемент '" << value << "' додано на початок списку." << std::endl;
    }

    // Додавання елементу в кінець
    void addAtEnd(const T& value) {
        Node* newNode = new Node(value);
        if (!newNode) {
            throw std::bad_alloc();
        }
        if (isEmpty()) {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else {
            Node* last = head->prev;
            newNode->next = head;
            newNode->prev = last;
            last->next = newNode;
            head->prev = newNode;
        }
        currentSize++;
        std::cout << "Елемент '" << value << "' додано в кінець списку." << std::endl;
    }

    // Додавання елементу за індексом
    void addAtIndex(const T& value, int index) {
        if (index < 0 || index > currentSize) {
            throw std::out_of_range("Недійсний індекс для додавання.");
        }
        if (index == 0) {
            addAtBeginning(value);
            return;
        }
        if (index == currentSize) {
            addAtEnd(value);
            return;
        }

        Node* newNode = new Node(value);
        if (!newNode) {
            throw std::bad_alloc();
        }

        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
        currentSize++;
        std::cout << "Елемент '" << value << "' додано за індексом " << index << "." << std::endl;
    }

    // Пошук елементу за ключовим полем
    bool search(const T& key) const {
        if (isEmpty()) {
            return false;
        }
        Node* current = head;
        do {
            if (current->data == key) {
                return true;
            }
            current = current->next;
        } while (current != head);
        return false;
    }

    // Видалення елементів за заданим значенням ключового поля
    void removeByValue(const T& value) {
        if (isEmpty()) {
            std::cout << "Список порожній. Немає елементів для видалення." << std::endl;
            return;
        }

        bool found = false;
        Node* current = head;
        Node* nextNode;
        int initialSize = currentSize;

        for (int i = 0; i < initialSize; ++i) { // Проходимося по копії розміру, щоб уникнути нескінченного циклу
            nextNode = current->next; // Зберігаємо наступний елемент до видалення

            if (current->data == value) {
                found = true;
                if (currentSize == 1) { // Якщо це останній елемент
                    delete head;
                    head = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    if (current == head) { // Якщо видаляємо голову
                        head = current->next;
                    }
                    delete current;
                }
                currentSize--;
                std::cout << "Елемент '" << value << "' видалено зі списку." << std::endl;
            }
            if (currentSize == 0) break; // Якщо список став порожнім
            current = nextNode; // Переходимо до наступного елемента
            if (i == initialSize - 1 && current == head && current->data == value) { //Особливий випадок для останнього елемента в циклі, якщо він співпадає і був видалений
                if (currentSize == 1) {
                    delete head;
                    head = nullptr;
                    currentSize--;
                }
            }
        }
        if (!found) {
            std::cout << "Елемент '" << value << "' не знайдено в списку." << std::endl;
        }
    }


    // Видалення елементу за індексом
    void removeByIndex(int index) {
        if (isEmpty()) {
            throw std::out_of_range("Список порожній. Немає елементів для видалення.");
        }
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Недійсний індекс для видалення.");
        }

        Node* nodeToRemove = head;
        if (index == 0) { // Видаляємо голову
            if (currentSize == 1) {
                delete head;
                head = nullptr;
            }
            else {
                Node* last = head->prev;
                head = head->next;
                head->prev = last;
                last->next = head;
                delete nodeToRemove;
            }
        }
        else {
            for (int i = 0; i < index; ++i) {
                nodeToRemove = nodeToRemove->next;
            }
            nodeToRemove->prev->next = nodeToRemove->next;
            nodeToRemove->next->prev = nodeToRemove->prev;
            delete nodeToRemove;
        }
        currentSize--;
        std::cout << "Елемент за індексом " << index << " видалено зі списку." << std::endl;
    }

    // Виведення всього списку на екран
    void display() const {
        if (isEmpty()) {
            std::cout << "Список порожній." << std::endl;
            return;
        }
        Node* current = head;
        std::cout << "Елементи двоспрямованого циклічного списку:" << std::endl;
        do {
            std::cout << current->data;
            current = current->next;
            if (current != head) {
                std::cout << " <-> ";
            }
        } while (current != head);
        std::cout << std::endl;
    }

    // Виведення всього списку у типізований файл
    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для запису: " + filename);
        }

        if (isEmpty()) {
            outFile.close();
            std::cout << "Список порожній, нічого не збережено." << std::endl;
            return;
        }

        Node* current = head;
        do {
            outFile.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
            current = current->next;
        } while (current != head);

        outFile.close();
        std::cout << "Список успішно збережено у файл '" << filename << "'." << std::endl;
    }

    // Введення всього списку із типізованого файлу
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile.is_open()) {
            throw std::runtime_error("Не вдалося відкрити файл для читання: " + filename);
        }

        // Очищаємо поточний список перед завантаженням
        while (!isEmpty()) {
            removeByIndex(0);
        }

        T value;
        // Читаємо елементи та додаємо їх до списку
        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(T))) {
            addAtEnd(value);
        }

        inFile.close();
        std::cout << "Список успішно завантажено з файлу '" << filename << "'." << std::endl;
    }

    // Сортування (бульбашкове)
    void sort() {
        if (isEmpty() || currentSize == 1) {
            std::cout << "Список порожній або містить один елемент, сортування не потрібне." << std::endl;
            return;
        }

        // Перетворюємо список у вектор для сортування, потім назад у список
        std::vector<T> tempVec;
        Node* current = head;
        do {
            tempVec.push_back(current->data);
            current = current->next;
        } while (current != head);

        // Бульбашкове сортування
        for (size_t i = 0; i < tempVec.size() - 1; ++i) {
            for (size_t j = 0; j < tempVec.size() - 1 - i; ++j) {
                if (tempVec[j] > tempVec[j + 1]) {
                    std::swap(tempVec[j], tempVec[j + 1]);
                }
            }
        }

        // Очищаємо поточний список
        while (!isEmpty()) {
            removeByIndex(0);
        }

        // Додаємо відсортовані елементи назад у список
        for (const T& val : tempVec) {
            addAtEnd(val);
        }
        std::cout << "Список відсортовано (бульбашкове сортування)." << std::endl;
    }
};

#endif // DYNAMIC_COLLECTIONS_H
