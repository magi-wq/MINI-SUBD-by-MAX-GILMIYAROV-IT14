#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>

struct Record {
    int id;
    std::string name;
    int age;
    double salary;
    
    void display() const;
};

class Database {
private:
    std::vector<Record> records;
    int next_id;
    
public:
    Database();
    
    bool addRecord(const std::string& name, int age, double salary);
    void displayAll() const;
    bool editRecord(int id, const std::string& new_name, int new_age, double new_salary);
    bool deleteRecord(int id);
    
    std::vector<Record> searchByName(const std::string& name) const;
    std::vector<Record> searchByAge(int age) const;
    std::vector<Record> searchBySalary(double salary) const;
    
    void sortByName(bool ascending = true);
    void sortByAge(bool ascending = true);
    void sortBySalary(bool ascending = true);
    void sortById(bool ascending = true);
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    
    bool recordExists(int id) const;
    void displayCurrentOrder() const;
    const std::vector<Record>& getRecords() const { return records; }
};

#endif