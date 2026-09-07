#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <windows.h>
#include <cstdio>
using namespace std;

const char* filename = "lab4_data.txt";

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

    man(const char* f, const char* n) {
        fam = new char[strlen(f) + 1];
        name = new char[strlen(n) + 1];
        strcpy(fam, f);
        strcpy(name, n);
    }

    man(const man& other) {
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

    bool operator==(const man& other) const {
        return (strcmp(fam, other.fam) == 0 && strcmp(name, other.name) == 0);
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

    friend istream& operator>>(istream& is, man& m) {
        char fam[100], name[100];
        cout << "Введите фамилию: ";
        is >> fam;
        cout << "Введите имя: ";
        is >> name;
        m = man(fam, name);
        return is;
    }

    friend ostream& operator<<(ostream& os, const man& m) {
        os << m.fam << " " << m.name;
        return os;
    }
};

class common {
protected:
    man client;
    double amount;

public:
    common() : client(), amount(0.0) {}

    common(const char* fam, const char* name, double amt) : client(fam, name), amount(amt) {}

    common(const common& other) : client(other.client), amount(other.amount) {}

    virtual ~common() {}

    virtual void setAmount(double amt) { amount = amt; }
    virtual double getAmount() const { return amount; }

    man getClient() const { return client; }

    virtual bool operator==(const man& m) const {
        return client == m;
    }

    virtual istream& input(istream& is) {
        is >> client;
        cout << "Введите сумму: ";
        is >> amount;
        return is;
    }

    virtual ostream& output(ostream& os) const {
        os << "Клиент: " << client << ", Сумма: " << amount;
        return os;
    }

    friend istream& operator>>(istream& is, common& obj) {
        return obj.input(is);
    }

    friend ostream& operator<<(ostream& os, const common& obj) {
        return obj.output(os);
    }
};

class deposit : public common {
private:
    double interestRate;

public:
    deposit() : common(), interestRate(0.0) {}

    deposit(const char* fam, const char* name, double amt, double rate)
        : common(fam, name, amt), interestRate(rate) {
    }

    deposit(const deposit& other) : common(other), interestRate(other.interestRate) {}

    virtual ~deposit() {}

    void setInterestRate(double rate) { interestRate = rate; }
    double getInterestRate() const { return interestRate; }

    virtual bool operator==(const man& m) const override {
        return client == m;
    }

    virtual istream& input(istream& is) override {
        is >> client;
        cout << "Введите сумму на счете: ";
        is >> amount;
        cout << "Введите процент по депозиту: ";
        is >> interestRate;
        return is;
    }

    virtual ostream& output(ostream& os) const override {
        os << "Тип: ДЕПОЗИТ | Клиент: " << client
            << " | Сумма на счете: " << amount << " руб."
            << " | Процент: " << interestRate << "%";
        return os;
    }
};

class credit : public common {
private:
    int cardNumber;
    double interestRate;

public:
    credit() : common(), cardNumber(0), interestRate(0.0) {}

    credit(const char* fam, const char* name, double creditAmt, int cardNum, double rate)
        : common(fam, name, creditAmt), cardNumber(cardNum), interestRate(rate) {
    }

    credit(const credit& other) : common(other), cardNumber(other.cardNumber), interestRate(other.interestRate) {}

    virtual ~credit() {}

    void setCardNumber(int num) { cardNumber = num; }
    int getCardNumber() const { return cardNumber; }

    void setInterestRate(double rate) { interestRate = rate; }
    double getInterestRate() const { return interestRate; }

    virtual bool operator==(const man& m) const override {
        return client == m;
    }

    virtual istream& input(istream& is) override {
        is >> client;
        cout << "Введите номер кредитной карты: ";
        is >> cardNumber;
        cout << "Введите сумму кредита: ";
        is >> amount;
        cout << "Введите процент по кредиту: ";
        is >> interestRate;
        return is;
    }

    virtual ostream& output(ostream& os) const override {
        os << "Тип: КРЕДИТ | Клиент: " << client
            << " | Сумма кредита: " << amount << " руб."
            << " | Карта №" << cardNumber
            << " | Процент: " << interestRate << "%";
        return os;
    }
};

void saveDepositsToFile(deposit** arr, int size) {
    if (size == 0) return;

    FILE* file = fopen("deposits.dat", "wb");
    if (!file) return;

    fwrite(&size, sizeof(int), 1, file);
    for (int i = 0; i < size; i++) {
        man m = arr[i]->getClient();
        const char* fam = m.getFam();
        const char* name = m.getName();
        int famLen = strlen(fam) + 1;
        int nameLen = strlen(name) + 1;

        fwrite(&famLen, sizeof(int), 1, file);
        fwrite(fam, sizeof(char), famLen, file);
        fwrite(&nameLen, sizeof(int), 1, file);
        fwrite(name, sizeof(char), nameLen, file);

        double amount = arr[i]->getAmount();
        double rate = arr[i]->getInterestRate();
        fwrite(&amount, sizeof(double), 1, file);
        fwrite(&rate, sizeof(double), 1, file);
    }
    fclose(file);
}

void loadDepositsFromFile(deposit**& arr, int& size) {
    FILE* file = fopen("deposits.dat", "rb");
    if (!file) {
        size = 0;
        arr = nullptr;
        return;
    }

    fread(&size, sizeof(int), 1, file);
    arr = new deposit * [size];

    for (int i = 0; i < size; i++) {
        int famLen, nameLen;
        char* fam, * name;
        double amount, rate;

        fread(&famLen, sizeof(int), 1, file);
        fam = new char[famLen];
        fread(fam, sizeof(char), famLen, file);

        fread(&nameLen, sizeof(int), 1, file);
        name = new char[nameLen];
        fread(name, sizeof(char), nameLen, file);

        fread(&amount, sizeof(double), 1, file);
        fread(&rate, sizeof(double), 1, file);

        arr[i] = new deposit(fam, name, amount, rate);

        delete[] fam;
        delete[] name;
    }
    fclose(file);
}

void saveCreditsToFile(credit** arr, int size) {
    if (size == 0) return;

    FILE* file = fopen("credits.dat", "wb");
    if (!file) return;

    fwrite(&size, sizeof(int), 1, file);
    for (int i = 0; i < size; i++) {
        man m = arr[i]->getClient();
        const char* fam = m.getFam();
        const char* name = m.getName();
        int famLen = strlen(fam) + 1;
        int nameLen = strlen(name) + 1;

        fwrite(&famLen, sizeof(int), 1, file);
        fwrite(fam, sizeof(char), famLen, file);
        fwrite(&nameLen, sizeof(int), 1, file);
        fwrite(name, sizeof(char), nameLen, file);

        double amount = arr[i]->getAmount();
        int cardNum = arr[i]->getCardNumber();
        double rate = arr[i]->getInterestRate();

        fwrite(&amount, sizeof(double), 1, file);
        fwrite(&cardNum, sizeof(int), 1, file);
        fwrite(&rate, sizeof(double), 1, file);
    }
    fclose(file);
}

void loadCreditsFromFile(credit**& arr, int& size) {
    FILE* file = fopen("credits.dat", "rb");
    if (!file) {
        size = 0;
        arr = nullptr;
        return;
    }

    fread(&size, sizeof(int), 1, file);
    arr = new credit * [size];

    for (int i = 0; i < size; i++) {
        int famLen, nameLen;
        char* fam, * name;
        double amount, rate;
        int cardNum;

        fread(&famLen, sizeof(int), 1, file);
        fam = new char[famLen];
        fread(fam, sizeof(char), famLen, file);

        fread(&nameLen, sizeof(int), 1, file);
        name = new char[nameLen];
        fread(name, sizeof(char), nameLen, file);

        fread(&amount, sizeof(double), 1, file);
        fread(&cardNum, sizeof(int), 1, file);
        fread(&rate, sizeof(double), 1, file);

        arr[i] = new credit(fam, name, amount, cardNum, rate);

        delete[] fam;
        delete[] name;
    }
    fclose(file);
}

void printTableHeader() {
    cout << "\n=========================================================================================" << endl;
    cout << "|  №  |     Тип      |        Клиент         |     Сумма (руб.)     |   Доп. информация   |" << endl;
    cout << "=========================================================================================" << endl;
}

void printDepositTable(deposit** arr, int size) {
    if (size == 0) {
        cout << "Нет данных по депозитам!" << endl;
        return;
    }

    printTableHeader();
    for (int i = 0; i < size; i++) {
        printf("| %3d | %-11s | %-21s | %-20.2f | %-19.2f%% |\n",
            i + 1, "ДЕПОЗИТ",
            (string(arr[i]->getClient().getFam()) + " " + string(arr[i]->getClient().getName())).c_str(),
            arr[i]->getAmount(),
            arr[i]->getInterestRate());
    }
    cout << "=========================================================================================" << endl;
}

void printCreditTable(credit** arr, int size) {
    if (size == 0) {
        cout << "Нет данных по кредитам!" << endl;
        return;
    }

    printTableHeader();
    for (int i = 0; i < size; i++) {
        printf("| %3d | %-11s | %-21s | %-20.2f | №%-18d |\n",
            i + 1, "КРЕДИТ",
            (string(arr[i]->getClient().getFam()) + " " + string(arr[i]->getClient().getName())).c_str(),
            arr[i]->getAmount(),
            arr[i]->getCardNumber());
    }
    cout << "=========================================================================================" << endl;
}

void printResultTable(common** arr, int size) {
    if (size == 0) {
        cout << "Результатов не найдено!" << endl;
        return;
    }

    cout << "\n========================= РЕЗУЛЬТАТЫ ПОИСКА =========================" << endl;
    cout << "==================================================================" << endl;
    cout << "|  №  |        Клиент         |     Тип     |     Сумма (руб.)    |" << endl;
    cout << "==================================================================" << endl;

    for (int i = 0; i < size; i++) {
        const char* type = (dynamic_cast<deposit*>(arr[i]) != nullptr) ? "ДЕПОЗИТ" : "КРЕДИТ";
        printf("| %3d | %-21s | %-11s | %-20.2f |\n",
            i + 1,
            (string(arr[i]->getClient().getFam()) + " " + string(arr[i]->getClient().getName())).c_str(),
            type,
            arr[i]->getAmount());
    }
    cout << "==================================================================" << endl;
}

int searchAndAdd(deposit** deposits, int depSize, credit** credits, int credSize,
    const char* fam, const char* name, double addAmount, common**& result, int& resSize) {

    man searchMan(fam, name);
    resSize = 0;

    common** temp = new common * [depSize + credSize];

    for (int i = 0; i < depSize; i++) {
        if (*deposits[i] == searchMan) {
            temp[resSize++] = deposits[i];
            deposits[i]->setAmount(deposits[i]->getAmount() + addAmount);
        }
    }

    for (int i = 0; i < credSize; i++) {
        if (*credits[i] == searchMan) {
            temp[resSize++] = credits[i];
            credits[i]->setAmount(credits[i]->getAmount() + addAmount);
        }
    }

    if (resSize > 0) {
        result = new common * [resSize];
        for (int i = 0; i < resSize; i++) {
            result[i] = temp[i];
        }
    }
    else {
        result = nullptr;
    }

    delete[] temp;
    return resSize;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    deposit** deposits = nullptr;
    credit** credits = nullptr;
    int depSize = 0, credSize = 0;

    loadDepositsFromFile(deposits, depSize);
    loadCreditsFromFile(credits, credSize);

    int choice;
    do {
        system("pause");
        system("cls");
        cout << "\n================================================================================" << endl;
        cout << "                        БАНКОВСКАЯ СИСТЕМА (Депозиты и Кредиты)" << endl;
        cout << "================================================================================" << endl;
        cout << "  1. Добавить новый депозит" << endl;
        cout << "  2. Добавить новый кредит" << endl;
        cout << "  3. Показать все депозиты (таблица)" << endl;
        cout << "  4. Показать все кредиты (таблица)" << endl;
        cout << "  5. Найти клиента по ФИО и добавить сумму" << endl;
        cout << "  6. Сохранить данные и выйти" << endl;
        cout << "================================================================================" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            deposit* newDep = new deposit();
            cin >> *newDep;

            deposit** newArr = new deposit * [depSize + 1];
            for (int i = 0; i < depSize; i++) {
                newArr[i] = deposits[i];
            }
            newArr[depSize] = newDep;

            delete[] deposits;
            deposits = newArr;
            depSize++;

            cout << "\nДепозит успешно добавлен!" << endl;
            break;
        }

        case 2: {
            credit* newCred = new credit();
            cin >> *newCred;

            credit** newArr = new credit * [credSize + 1];
            for (int i = 0; i < credSize; i++) {
                newArr[i] = credits[i];
            }
            newArr[credSize] = newCred;

            delete[] credits;
            credits = newArr;
            credSize++;

            cout << "\nКредит успешно добавлен!" << endl;
            break;
        }

        case 3:
            printDepositTable(deposits, depSize);
            break;

        case 4:
            printCreditTable(credits, credSize);
            break;

        case 5: {
            if (depSize == 0 && credSize == 0) {
                cout << "База данных пуста! Сначала добавьте записи." << endl;
                break;
            }

            char fam[100], name[100];
            double addAmount;

            cout << "\n=== ПОИСК КЛИЕНТА И ДОБАВЛЕНИЕ СУММЫ ===" << endl;
            cout << "Введите фамилию клиента: ";
            cin >> fam;
            cout << "Введите имя клиента: ";
            cin >> name;
            cout << "Введите сумму для добавления: ";
            cin >> addAmount;

            common** result = nullptr;
            int resSize = 0;

            int found = searchAndAdd(deposits, depSize, credits, credSize,
                fam, name, addAmount, result, resSize);

            if (found > 0) {
                cout << "\nНайдено " << found << " клиент(ов). После добавления суммы:" << endl;
                printResultTable(result, resSize);
            }
            else {
                cout << "\nКлиент с фамилией \"" << fam << "\" и именем \"" << name << "\" не найден!" << endl;
            }

            delete[] result;
            break;
        }

        case 6:
            saveDepositsToFile(deposits, depSize);
            saveCreditsToFile(credits, credSize);
            cout << "\nДанные сохранены. До свидания!" << endl;
            break;

        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
        }
    } while (choice != 6);

    for (int i = 0; i < depSize; i++) {
        delete deposits[i];
    }
    delete[] deposits;

    for (int i = 0; i < credSize; i++) {
        delete credits[i];
    }
    delete[] credits;

    return 0;
}