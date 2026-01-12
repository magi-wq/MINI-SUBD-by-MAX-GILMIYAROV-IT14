#include "database.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cctype>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

Database::Database() : next_id(1) {
    // Устанавливаем локаль для Windows
    #ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    #endif
}

bool Database::addRecord(const string& name, int age, double salary) {
    if (age <= 0) {
        cout << "Ошибка: Возраст должен быть положительным" << endl;
        return false;
    }
    if (salary < 0) {
        cout << "Ошибка: Зарплата не может быть отрицательной" << endl;
        return false;
    }
    if (name.empty()) {
        cout << "Ошибка: Имя не может быть пустым." << endl;
        return false;
    }
    
    if (name.length() > 50) {
        cout << "Ошибка: Слишком длинное имя (максимум 50 символов)." << endl;
        return false;
    }
    
    if (age > 150) {  // Реалистичный максимум
        cout << "Ошибка: Некорректный возраст (максимум 150 лет)." << endl;
        return false;
    }
    
    if (salary > 1000000000) {  // 1 миллиард максимум
        cout << "Ошибка: Слишком большая зарплата." << endl;
        return false;
    }
    Record newRecord;
    
    // Создаем множество использованных ID для быстрого поиска
    vector<bool> id_used(1000, false); // предполагаем максимум 1000 записей
    
    for (const auto& record : records) {
        if (record.id < 1000) {
            id_used[record.id] = true;
        }
    }
    
    // Ищем первый свободный ID начиная с 1
    int new_id = 1;
    while (new_id < 1000 && id_used[new_id]) {
        new_id++;
    }
    
    if (new_id >= 1000) {
        // Если все ID до 1000 заняты, ищем максимальный и добавляем +1
        int max_id = 0;
        for (const auto& record : records) {
            if (record.id > max_id) {
                max_id = record.id;
            }
        }
        new_id = max_id + 1;
    }
    
    newRecord.id = new_id;
    newRecord.name = name;
    newRecord.age = age;
    newRecord.salary = salary;
    
    records.push_back(newRecord);
    
    cout << "Запись добавлена (ID: " << newRecord.id << ")" << endl;
    return true;
}

void Database::displayAll() const {
    if (records.empty()) {
        cout << "База данных пуста." << endl;
        return;
    }
    
    // Создаем копию и сортируем по ID (возрастание)
    vector<Record> sorted_records = records;
    sort(sorted_records.begin(), sorted_records.end(),
        [](const Record& a, const Record& b) {
            return a.id < b.id;  // По возрастанию ID
        });
    
    cout << "ВСЕ ЗАПИСИ (отсортировано по ID)" << endl;
    cout << "Всего записей: " << records.size() << endl;
    cout << "ID\tИмя\t\tВозраст\tЗарплата" << endl;
    
    for (const auto& record : sorted_records) {
        // Форматируем вывод с табуляцией
        cout << record.id << "\t";
        
        // Имя с выравниванием
        if (record.name.length() < 8) {
            cout << record.name << "\t\t";
        } else {
            cout << record.name.substr(0, 8) << "...\t";
        }
        
        cout << record.age << "\t";
        cout << fixed << setprecision(2) << record.salary << endl;
    }
    
}

bool Database::editRecord(int id, const string& new_name, int new_age, double new_salary) {
    if (new_age <= 0) {
        cout << "Ошибка: Возраст должен быть положительным" << endl;
        return false;
    }
    if (new_salary < 0) {
        cout << "Ошибка: Зарплата не может быть отрицательной" << endl;
        return false;
    }
    
    for (auto& record : records) {
        if (record.id == id) {
            record.name = new_name;
            record.age = new_age;
            record.salary = new_salary;
            cout << "Запись " << id << " обновлена." << endl;
            return true;
        }
    }
    cout << "Запись " << id << " не найдена." << endl;
    return false;
}

bool Database::deleteRecord(int id) {
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->id == id) {
            records.erase(it);
            cout << "Запись " << id << " удалена." << endl;
            return true;
        }
    }
    cout << "Запись " << id << " не найдена." << endl;
    return false;
}

vector<Record> Database::searchByName(const string& name) const {
    vector<Record> result;
    for (const auto& record : records) {
        if (record.name == name) {
            result.push_back(record);
        }
    }
    return result;
}

vector<Record> Database::searchByAge(int age) const {
    vector<Record> result;
    for (const auto& record : records) {
        if (record.age == age) {
            result.push_back(record);
        }
    }
    return result;
}

vector<Record> Database::searchBySalary(double salary) const {
    vector<Record> result;
    for (const auto& record : records) {
        if (record.salary == salary) {
            result.push_back(record);
        }
    }
    return result;
}

// Функция для сравнения русских строк без учета регистра
bool compareRussianStrings(const string& a, const string& b, bool ascending) {
    string a_lower = a;
    string b_lower = b;
    
    // Преобразуем в нижний регистр для Windows-1251
    for (char& c : a_lower) {
        if (c >= 'А' && c <= 'Я') {
            c = c - 'А' + 'а';
        }
    }
    
    for (char& c : b_lower) {
        if (c >= 'А' && c <= 'Я') {
            c = c - 'А' + 'а';
        }
    }
    
    if (ascending) {
        return a_lower < b_lower;
    } else {
        return a_lower > b_lower;
    }
}

void Database::sortByName(bool ascending) {
    if (records.empty()) {
        cout << "База данных пуста. Нечего сортировать." << endl;
        return;
    }
    
    sort(records.begin(), records.end(), 
        [ascending](const Record& a, const Record& b) {
            return compareRussianStrings(a.name, b.name, ascending);
        });
    
    cout << "Записи отсортированы по имени (" 
         << (ascending ? "А-Я" : "Я-А") << ")." << endl;
}

void Database::sortByAge(bool ascending) {
    if (records.empty()) {
        cout << "База данных пуста. Нечего сортировать." << endl;
        return;
    }
    
    sort(records.begin(), records.end(), 
        [ascending](const Record& a, const Record& b) {
            if (ascending) {
                return a.age < b.age;
            } else {
                return a.age > b.age;
            }
        });
    
    cout << "Записи отсортированы по возрасту (" 
         << (ascending ? "возрастание" : "убывание") << ")." << endl;
}

void Database::sortBySalary(bool ascending) {
    if (records.empty()) {
        cout << "База данных пуста. Нечего сортировать." << endl;
        return;
    }
    
    sort(records.begin(), records.end(), 
        [ascending](const Record& a, const Record& b) {
            if (ascending) {
                return a.salary < b.salary;
            } else {
                return a.salary > b.salary;
            }
        });
    
    cout << "Записи отсортированы по зарплате (" 
         << (ascending ? "возрастание" : "убывание") << ")." << endl;
}

void Database::sortById(bool ascending) {
    if (records.empty()) {
        cout << "База данных пуста. Нечего сортировать." << endl;
        return;
    }
    
    sort(records.begin(), records.end(), 
        [ascending](const Record& a, const Record& b) {
            if (ascending) {
                return a.id < b.id;
            } else {
                return a.id > b.id;
            }
        });
    
    cout << "Записи отсортированы по ID (" 
         << (ascending ? "возрастание" : "убывание") << ")." << endl;
}

bool Database::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка: Не удается открыть файл: " << filename << endl;
        return false;
    }
    
    records.clear();
    
    Record r;
    int line_num = 0;
    int loaded_count = 0;
    
    while (file >> r.id >> r.name >> r.age >> r.salary) {
        line_num++;
        
        // Проверка корректности загруженных данных
        if (r.id <= 0) {
            cout << "Предупреждение: Некорректный ID (" << r.id 
                 << ") в строке " << line_num << ", запись пропущена." << endl;
            continue;
        }
        
        if (r.age <= 0 || r.age > 150) {
            cout << "Предупреждение: Некорректный возраст (" << r.age 
                 << ") в строке " << line_num << ", запись пропущена." << endl;
            continue;
        }
        
        if (r.salary < 0 || r.salary > 1000000000) {
            cout << "Предупреждение: Некорректная зарплата (" << r.salary 
                 << ") в строке " << line_num << ", запись пропущена." << endl;
            continue;
        }
        
        // Проверяем уникальность ID (чтобы не было дубликатов)
        bool duplicate_id = false;
        for (const auto& record : records) {
            if (record.id == r.id) {
                cout << "Предупреждение: Дублирующийся ID (" << r.id 
                     << ") в строке " << line_num << ", запись пропущена." << endl;
                duplicate_id = true;
                break;
            }
        }
        
        if (duplicate_id) {
            continue;
        }
        
        records.push_back(r);
        loaded_count++;
    }
    
    file.close();
    
    if (file.bad()) {
        cout << "Ошибка при чтении файла: " << filename << endl;
        return false;
    }
    
    // Обновляем next_id на основе максимального ID
    int max_id = 0;
    for (const auto& record : records) {
        if (record.id > max_id) {
            max_id = record.id;
        }
    }
    next_id = max_id + 1;
    
    cout << "Загружено " << loaded_count << " записей из " << filename << endl;
    
    if (line_num > loaded_count) {
        cout << "Пропущено " << (line_num - loaded_count) 
             << " некорректных записей." << endl;
    }
    
    return true;
}

bool Database::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка: Не удается создать файл: " << filename << endl;
        return false;
    }
    
    file << fixed << setprecision(2);
    for (const auto& record : records) {
        file << record.id << " " << record.name << " " 
             << record.age << " " << record.salary << endl;
    }
    
    file.close();
    cout << "Сохранено " << records.size() << " записей в " << filename << endl;
    return true;
}

bool Database::recordExists(int id) const {
    for (const auto& record : records) {
        if (record.id == id) {
            return true;
        }
    }
    return false;
}

void Record::display() const {
    cout << "ID: " << setw(3) << left << id 
         << "Имя: " << setw(15) << left << name 
         << "Возраст: " << setw(5) << left << age 
         << "Зарплата: " << fixed << setprecision(2) << salary << endl;
}

void Database::displayCurrentOrder() const {
    if (records.empty()) {
        cout << "База данных пуста." << endl;
        return;
    }
    
    cout << "Текущий порядок записей:" << endl;
    for (const auto& record : records) {
        record.display();
    }
}