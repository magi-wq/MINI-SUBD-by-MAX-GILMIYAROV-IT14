#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "database.h"
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Функция очистки экрана
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

// Настройка кодировки
void setupConsole() {
    #ifdef _WIN32
    // Устанавливаем кодировку Windows-1251
    system("chcp 1251 > nul");
    #endif
}

// Проверка русских символов
bool isRussianText(const string& text) {
    if (text.empty()) return false;
    
    for (char c : text) {
        unsigned char uc = static_cast<unsigned char>(c);
        
        // Разрешаем пробелы, дефисы, апострофы
        if (c == ' ' || c == '-' || c == '\'') continue;
        
        // Русские буквы в Windows-1251
        bool isRussian = (uc >= 192 && uc <= 255);
        
        // Также разрешаем английские буквы
        bool isEnglish = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z');
        
        if (!isRussian && !isEnglish) {
            return false;
        }
    }
    return true;
}

// Функция для получения корректного целого числа
int getValidInt(const string& prompt, int min = 0, int max = 1000) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Пожалуйста, введите целое число." << endl;
        } else if (value < min || value > max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: Значение должно быть от " << min << " до " << max << "." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Функция для получения корректного дробного числа
double getValidDouble(const string& prompt, double min = 0, double max = 1000000000) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Пожалуйста, введите число." << endl;
        } else if (value < min || value > max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: Значение должно быть от " << min << " до " << max << "." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Функция для получения имени на русском языке
string getRussianName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);
        
        // Убираем лишние пробелы
        name.erase(0, name.find_first_not_of(" "));
        name.erase(name.find_last_not_of(" ") + 1);
        
        // Проверка 1: Не пустое
        if (name.empty()) {
            cout << "Ошибка: Имя не может быть пустым." << endl;
            continue;
        }
        
        // Проверка 2: Максимальная длина
        if (name.length() > 50) {
            cout << "Ошибка: Имя слишком длинное (максимум 50 символов)." << endl;
            continue;
        }
        
        // Проверка 3: Содержит ли цифры
        bool hasDigits = false;
        for (char c : name) {
            if (isdigit(c)) {
                hasDigits = true;
                break;
            }
        }
        if (hasDigits) {
            cout << "Ошибка: Имя не должно содержать цифры." << endl;
            continue;
        }
        
        // Проверка 4: Русские/английские буквы
        if (!isRussianText(name)) {
            cout << "Ошибка: Пожалуйста, используйте русские буквы для имени." << endl;
            cout << "Пример: Иван, Анна-Мария, Сергей" << endl;
            continue;
        }
        
        return name;
    }
}

void showMainMenu() {
    clearScreen();
    cout << "СИСТЕМА УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ" << endl;
    cout << "\nГЛАВНОЕ МЕНЮ" << endl;
    cout << "1. Добавить запись" << endl;
    cout << "2. Показать все записи" << endl;
    cout << "3. Редактировать запись" << endl;
    cout << "4. Удалить запись" << endl;
    cout << "5. Поиск записей" << endl;
    cout << "6. Сортировка записей" << endl;
    cout << "7. Сохранить в файл" << endl;
    cout << "8. Загрузить из файла" << endl;
    cout << "9. Добавить тестовые данные" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите пункт: ";
}

void showSearchMenu(Database& db) {
    int choice;
    
    do {
        clearScreen();
        cout << "\nПОИСК ЗАПИСЕЙ" << endl;
        cout << "1. Поиск по имени" << endl;
        cout << "2. Поиск по возрасту" << endl;
        cout << "3. Поиск по зарплате" << endl;
        cout << "0. Назад в главное меню" << endl;
        
        choice = getValidInt("Выберите пункт: ");
        
        switch (choice) {
            case 1: {
                string name;
                cout << "Введите имя для поиска: ";
                getline(cin, name);
                
                vector<Record> results = db.searchByName(name);
                
                clearScreen();
                if (results.empty()) {
                    cout << "Записей с именем '" << name << "' не найдено." << endl;
                } else {
                    cout << "Найдено " << results.size() << " запись(ей):" << endl;
                    for (const auto& record : results) {
                        record.display();
                    }
                }
                break;
            }
            
            case 2: {
                int age = getValidInt("Введите возраст для поиска: ");
                
                vector<Record> results = db.searchByAge(age);
                
                clearScreen();
                if (results.empty()) {
                    cout << "Записей с возрастом " << age << " не найдено." << endl;
                } else {
                    for (const auto& record : results) {
                        record.display();
                    }
                }
                break;
            }
            
            case 3: {
                double salary = getValidDouble("Введите зарплату для поиска: ");
                
                vector<Record> results = db.searchBySalary(salary);
                
                clearScreen();
                if (results.empty()) {
                    cout << "Записей с зарплатой " << salary << " не найдено." << endl;
                } else {
                    for (const auto& record : results) {
                        record.display();
                    }
                }
                break;
            }
            
            case 0:
                cout << "Возврат в главное меню..." << endl;
                break;
                
            default:
                cout << "Неверный пункт!" << endl;
        }
        
        if (choice != 0 && choice >= 1 && choice <= 3) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }
        
    } while (choice != 0);
}

void showSortMenu(Database& db) {
    int choice;
    
    do {
        clearScreen();
        cout << "\nСОРТИРОВКА ЗАПИСЕЙ" << endl;
        cout << "1. Сортировка по имени (А-Я)" << endl;
        cout << "2. Сортировка по имени (Я-А)" << endl;
        cout << "3. Сортировка по возрасту (по возрастанию)" << endl;
        cout << "4. Сортировка по возрасту (по убыванию)" << endl;
        cout << "5. Сортировка по зарплате (по возрастанию)" << endl;
        cout << "6. Сортировка по зарплате (по убыванию)" << endl;
        cout << "7. Сортировка по ID (по возрастанию)" << endl;
        cout << "8. Сортировка по ID (по убыванию)" << endl;
        cout << "0. Назад в главное меню" << endl;
        
        choice = getValidInt("Выберите пункт: ");
        
        clearScreen();
        switch (choice) {
            case 1:
                db.sortByName(true);
                cout << "\nПосле сортировки по имени (А-Я):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 2:
                db.sortByName(false);
                cout << "\nПосле сортировки по имени (Я-А):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 3:
                db.sortByAge(true);
                cout << "\nПосле сортировки по возрасту (возрастание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 4:
                db.sortByAge(false);
                cout << "\nПосле сортировки по возрасту (убывание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 5:
                db.sortBySalary(true);
                cout << "\nПосле сортировки по зарплате (возрастание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 6:
                db.sortBySalary(false);
                cout << "\nПосле сортировки по зарплате (убывание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 7:
                db.sortById(true);
                cout << "\nПосле сортировки по ID (возрастание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 8:
                db.sortById(false);
                cout << "\nПосле сортировки по ID (убывание):" << endl;
                db.displayCurrentOrder();
                break;
                
            case 0:
                cout << "Возврат в главное меню..." << endl;
                break;
                
            default:
                cout << "Неверный пункт!" << endl;
        }
        
        if (choice != 0 && choice >= 1 && choice <= 8) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }
        
    } while (choice != 0);
}

void addTestData(Database& db) {
    db.addRecord("Иван", 25, 50000);
    db.addRecord("Анна", 30, 75000);
    db.addRecord("Михаил", 35, 60000);
    db.addRecord("Ольга", 28, 55000);
    db.addRecord("Сергей", 40, 80000);
    cout << "Тестовые данные успешно добавлены!" << endl;
}

int main() {
    setupConsole();
    
    Database db;
    int choice;
    
    clearScreen();
    cout << "СИСТЕМА УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ" << endl;
    cout << "Добро пожаловать в систему управления базой данных!" << endl;
    cout << "\nНажмите Enter для продолжения...";
    cin.get();
    
    do {
        showMainMenu();
        choice = getValidInt("", 0, 9);
        
        clearScreen();
        
        switch (choice) {
            case 1: {
                string name;
                int age;
                double salary;
                
                cout << "ДОБАВЛЕНИЕ ЗАПИСИ" << endl;
                name = getRussianName("Введите имя: ");
                age = getValidInt("Введите возраст: ", 1, 150);
                salary = getValidDouble("Введите зарплату: ");
                
                db.addRecord(name, age, salary);
                break;
            }
            
            case 2:
                db.displayAll();
                break;
                
            case 3: {
                int id;
                string new_name;
                int new_age;
                double new_salary;
                
                cout << "РЕДАКТИРОВАНИЕ ЗАПИСИ" << endl;
                db.displayAll();
                
                if (db.getRecords().empty()) {
                    cout << "Нет записей для редактирования." << endl;
                    break;
                }
                
                id = getValidInt("Введите ID записи для редактирования: ");
                if (!db.recordExists(id)) {
                    cout << "Ошибка: Запись с ID " << id << " не существует." << endl;
                    break;
                }

                new_name = getRussianName("Введите новое имя: ");
                new_age = getValidInt("Введите новый возраст: ", 1, 150);
                new_salary = getValidDouble("Введите новую зарплату: ");
                
                db.editRecord(id, new_name, new_age, new_salary);
                break;
            }
            
            case 4: {
                int id;
                
                cout << "УДАЛЕНИЕ ЗАПИСИ" << endl;
                db.displayAll();
                
                if (db.getRecords().empty()) {
                    cout << "Нет записей для удаления." << endl;
                    break;
                }
                
                id = getValidInt("Введите ID записи для удаления: ");
                db.deleteRecord(id);
                break;
            }
            
            case 5:
                showSearchMenu(db);
                break;
                
            case 6:
                showSortMenu(db);
                break;
                
            case 7:
                db.saveToFile("database_save.txt");
                break;
                
            case 8:
                db.loadFromFile("database_loadfrom.txt");
                break;
                
            case 9:
                addTestData(db);
                break;
                
            case 0:
                clearScreen();
                cout << "До свидания!" << endl;
                break;
                
            default:
                cout << "Неверный пункт! Попробуйте снова." << endl;
        }
        
        if (choice != 0 && choice != 5 && choice != 6) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}