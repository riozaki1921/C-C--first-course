#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <windows.h>
#include <cstdio>
using namespace std;

const char* filename = "lab2_data.txt";
class man {
    char* fam;
    char* name;

public:
    man() {
        fam = new char[1];
        name = new char[1];
        fam[0] = '\0';
        name[0] = '\0';
    }

    man(const char* f, const char* n){
        fam = new char[strlen(f) + 1];
        name = new char[strlen(n) + 1];
        strcpy(fam, f);
        strcpy(name, n);
    }

    man(const man& other){
        fam = new char[strlen(other.fam) + 1];
        name = new char[strlen(other.name) + 1];
        strcpy(fam, other.fam);
        strcpy(name, other.name);
    }

    ~man() {
        delete[] fam;
        delete[] name;
    }

    man& operator=(const man& other) {
        if (this != &other) {
            delete[] fam;
            delete[] name;

            fam = new char[strlen(other.fam) + 1];
            name = new char[strlen(other.name) + 1];
            strcpy(fam, other.fam);
            strcpy(name, other.name);
        }
        return *this;
    }

    void setFam(const char* f) {
        delete[] fam;
        fam = new char[strlen(f) + 1];
        strcpy(fam, f);
    }

    void setName(const char* n) {
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    const char* getFam() const { return fam; }
    const char* getName() const { return name; }

    void print() const {
        cout << fam << " " << name;
    }
};

class Bank {
    man fname;
    double sum;
    static int count;

public:
    Bank() : fname(), sum(0.0) {
        count++;
    }

    Bank(const char* fam, const char* name, double s) : fname(fam, name), sum(s) {
        count++;
    }

    Bank(const Bank& other) : fname(other.fname), sum(other.sum) {
        count++;
    }

    ~Bank() {
        count--;
    }

    void setSum(double s) { sum = s; }
    double getSum() const { return sum; }
    man getFname() const { return fname; }
    static int getCount() { return count; }

    friend istream& operator>>(istream& is, Bank& b);

    bool operator==(const man& m) const;

    friend ostream& operator<<(ostream& os, const Bank& b);
};

int Bank::count = 0;

istream& operator>>(istream& is, Bank& b) {
    char fam[100], name[100];
    double s;

    cout << "Введите фамилию клиента: ";
    is >> fam;
    cout << "Введите имя клиента: ";
    is >> name;
    cout << "Введите сумму депозита: ";
    is >> s;

    b.fname = man(fam, name);
    b.sum = s;

    return is;
}

bool Bank::operator==(const man& m) const {
    return (strcmp(fname.getFam(), m.getFam()) == 0 &&
        strcmp(fname.getName(), m.getName()) == 0);
}

ostream& operator<<(ostream& os, const Bank& b) {
    os << "Клиент: ";
    b.fname.print();
    os << ", Сумма депозита: " << b.sum << " руб.";
    return os;
}

void save_to_file(Bank* deposits, int clientCount) {
    if (deposits == NULL || clientCount == 0) {
        cout << "Нет данных для сохранения!\n";
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        cout << "Ошибка создания файла!\n";
        return;
    }

    fwrite(&clientCount, sizeof(int), 1, file);

    for (int i = 0; i < clientCount; i++) {
        man m = deposits[i].getFname();
        size_t last_len = strlen(m.getFam());
        fwrite(&last_len, sizeof(size_t), 1, file);
        fwrite(m.getFam(), 1, last_len, file);

        size_t first_len = strlen(m.getName());
        fwrite(&first_len, sizeof(size_t), 1, file);
        fwrite(m.getName(), 1, first_len, file);

        double balance = deposits[i].getSum();
        fwrite(&balance, sizeof(double), 1, file);
    }

    fclose(file);
    cout << "Данные сохранены в файл. Записей: " << clientCount << endl;
}

void load_from_file(Bank*& deposits, int& clientCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        cout << "Файл данных не найден. Будет создан новый.\n";
        clientCount = 0;
        deposits = NULL;
        return;
    }

    int countq;
    size_t readCount = fread(&countq, sizeof(int), 1, file);
    if (readCount != 1 || countq <= 0) {
        cout << "Ошибка чтения количества записей!" << endl;
        fclose(file);
        clientCount = 0;
        deposits = NULL;
        return;
    }

    if (deposits != NULL) {
        delete[] deposits;
    }

    deposits = new Bank[countq];
    clientCount = countq;

    for (int i = 0; i < countq; i++) {
        size_t last_len;
        if (fread(&last_len, sizeof(size_t), 1, file) != 1) {
            cout << "Ошибка чтения длины фамилии\n";
            break;
        }

        char* last_name = new char[last_len + 1];
        if (last_len > 0) {
            fread(last_name, 1, last_len, file);
        }
        last_name[last_len] = '\0';

        size_t first_len;
        if (fread(&first_len, sizeof(size_t), 1, file) != 1) {
            cout << "Ошибка чтения длины имени\n";
            delete[] last_name;
            break;
        }

        char* first_name = new char[first_len + 1];
        if (first_len > 0) {
            fread(first_name, 1, first_len, file);
        }
        first_name[first_len] = '\0';

        double balance;
        if (fread(&balance, sizeof(double), 1, file) != 1) {
            cout << "Ошибка чтения баланса\n";
            delete[] last_name;
            delete[] first_name;
            break;
        }

        deposits[i] = Bank(last_name, first_name, balance);

        delete[] last_name;
        delete[] first_name;

        cout << "Запись " << (i + 1) << " загружена: ";
        cout << deposits[i]<<endl;
    }

    fclose(file);
    cout << "Всего загружено: " << clientCount << " записей\n";
}
 
void printAllDeposits(Bank* arr, int size) {
    if (size == 0) {
        cout << "База данных пуста!" << endl;
        return;
    }

    cout << "\n======================================================================" << endl;
    cout << "|      |   Фамилия Имя |   Сумма депозита (руб.)   |" << endl;
    cout << "======================================================================" << endl;

    for (int i = 0; i < size; i++) {
        cout << "| " << i + 1 << "     | ";
        cout << arr[i] << endl;
    }
    cout << "\n======================================================================" << endl;
}

void findAndAdd(Bank* arr, int size) {
    if (size == 0) {
        cout << "База данных пуста!" << endl;
        return;
    }

    char fam[100], name[100];
    double amount;

    cout << "\n=== Поиск депозита и добавление суммы ===" << endl;
    cout << "Введите фамилию клиента: ";
    cin >> fam;
    cout << "Введите имя клиента: ";
    cin >> name;
    cout << "Введите сумму для добавления: ";
    cin >> amount;

    man searchMan(fam, name);
    bool found = false;

    for (int i = 0; i < size; i++) {
        if (arr[i] == searchMan) {
            cout << "\nНайден депозит:" << endl;
            cout << arr[i] << endl;
            arr[i].setSum(arr[i].getSum() + amount);

            cout << "После добавления " << amount << " руб.:" << endl;
            cout << arr[i] << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Клиент с фамилией \"" << fam << "\" и именем \"" << name << "\" не найден!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251); 

    Bank* deposits = nullptr;
    int size = 0;

    load_from_file(deposits, size);


    if (size == 0) {
        cout << "База данных пуста. Хотите ввести данные с клавиатуры? (1 - да, 0 - нет): ";
        int init_choice;
        cin >> init_choice;

        if (init_choice == 1) {
            int initial_count;
            cout << "Введите количество начальных счетов: ";
            cin >> initial_count;

            if (initial_count > 0) {
                deposits = new Bank[initial_count];
                size = initial_count;

                for (int i = 0; i < initial_count; i++) {
                    cout << "\nВвод данных для клиента " << i + 1 << ":\n";
                    cin >> deposits[i];
                }
            }
        }
    }


    int choice;
    do {
        system("pause");
        system("cls");
        cout << "\n=========================================" << endl;
        cout << "   БАНК (Депозитная система)" << endl;
        cout << "=========================================" << endl;
        cout << "1. Добавить новый депозит" << endl;
        cout << "2. Показать все депозиты" << endl;
        cout << "3. Найти депозит по ФИО и добавить сумму" << endl;
        cout << "4. Сохранить базу и выйти" << endl;
        cout << "=========================================" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Bank newDeposit;
            cin >> newDeposit;

            Bank* newArr = new Bank[size + 1];
            for (int i = 0; i < size; i++) {
                newArr[i] = deposits[i];
            }
            newArr[size] = newDeposit;

            delete[] deposits;
            deposits = newArr;
            size++;

            cout << "Депозит успешно добавлен!" << endl;
            break;
        }
        case 2:
            printAllDeposits(deposits, size);
            break;
        case 3:
            findAndAdd(deposits, size);
            break;
        case 4:
            if (size > 0) {
                save_to_file(deposits, size);
            }
            cout << "До свидания!" << endl;
            break;
        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
        }
    } while (choice != 4);

    delete[] deposits;
    deposits = NULL;
    return 0;
}