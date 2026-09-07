#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <windows.h>
#include <cstdio>
using namespace std;

const char* filename = "lab2_data.txt";

class bank {
private:
    char* fam;
    char* name;
    double sum;
    static int count;
public:
    bank(const char* last, const char* first, double bal);
    bank();
    bank(const bank& other);
    void define();

    char* getFam() { return fam; }
    char* getName() { return name; }
    double getSum() { return sum; }
    static int getCount() { return count; }

    void findClientsBySum(bank* clients, int clientCount, double minSum);

    void display();
    friend void load_from_file(bank*& deposits, int& clientCount);
    friend void findClient(bank* clients, int count, const char* fam, const char* name);
    bank& operator=(const bank& other);
    ~bank();
};

int bank::count = 0;

bank::bank() {
    fam = new char[60];
    strcpy(fam, " ");
    name = new char[60];
    strcpy(name, " ");
    sum = 0;
    count++;
    cout << "Конструктор по умолчанию. count=" << count << endl;
}

bank::bank(const char* last, const char* first, double bal) {
    fam = new char[strlen(last) + 1];
    strcpy(fam, last);
    name = new char[strlen(first) + 1];
    strcpy(name, first);
    sum = bal;
    count++;
    cout << "Конструктор с параметрами. count=" << count << endl;
}

bank::bank(const bank& other) {
    fam = new char[strlen(other.fam) + 1];
    strcpy(fam, other.fam);
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
    sum = other.sum;
    count++;
    cout << "Конструктор копирования. count=" << count << endl;
}

bank& bank::operator=(const bank& other) {
    cout << "Оператор присваивания" << endl;
    if (this != &other) {
        delete[] fam;
        delete[] name;

        fam = new char[strlen(other.fam) + 1];
        strcpy(fam, other.fam);
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        sum = other.sum;
    }
    return *this;
}

void bank::define() {
    delete[] fam;
    delete[] name;
    char buffer[100];

    cout << "Введите фамилию клиента: ";
    cin >> buffer;
    fam = new char[strlen(buffer) + 1];
    strcpy(fam, buffer);

    cout << "Введите имя клиента: ";
    cin >> buffer;
    name = new char[strlen(buffer) + 1];
    strcpy(name, buffer);

    cout << "Введите сумму на счете: ";
    cin >> sum;
}

bank::~bank() {
    delete[] fam;
    delete[] name;
    count--;
    cout << "Деструктор. count=" << count << endl;
}

void save_to_file(bank* deposits, int clientCount) {
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
        size_t last_len = strlen(deposits[i].getFam());
        fwrite(&last_len, sizeof(size_t), 1, file);
        fwrite(deposits[i].getFam(), 1, last_len, file);

        size_t first_len = strlen(deposits[i].getName());
        fwrite(&first_len, sizeof(size_t), 1, file);
        fwrite(deposits[i].getName(), 1, first_len, file);

        double balance = deposits[i].getSum();
        fwrite(&balance, sizeof(double), 1, file);
    }

    fclose(file);
    cout << "Данные сохранены в файл. Записей: " << clientCount << endl;
}

void bank::display() {
    cout << "Клиент: " << fam << " " << name
        << ", Сумма: " << sum << endl;
}

void bank::findClientsBySum(bank* clients, int clientCount, double minSum) {
    cout << "\nКлиенты с суммой больше " << minSum << ":\n";
    int found = 0;

    for (int i = 0; i < clientCount; i++) {
        if (clients[i].sum > minSum) {
            clients[i].display();
            found++;
        }
    }

    if (found == 0)
        cout << "Таких клиентов не найдено.\n";
    else
        cout << "Найдено: " << found << endl;
}

void findClient(bank* clients, int clientCount, const char* searchFam, const char* searchName) {
    cout << "\nПоиск клиента: " << searchFam << " " << searchName << endl;
    int found = 0;

    for (int i = 0; i < clientCount; i++) {
        if (strcmp(clients[i].fam, searchFam) == 0 &&
            strcmp(clients[i].name, searchName) == 0) {
            cout << "Найден: ";
            clients[i].display();
            found++;
        }
    }

    if (found == 0) {
        cout << "Клиент не найден.\n";
    }
}

void find_by_client(bank* deposits, int clientCount) {
    if (deposits == NULL || clientCount == 0) {
        cout << "Нет данных для поиска!\n";
        return;
    }

    char searchFam[100], searchName[100];
    cout << "Введите фамилию для поиска: ";
    cin >> searchFam;
    cout << "Введите имя для поиска: ";
    cin >> searchName;

    findClient(deposits, clientCount, searchFam, searchName);
}

void load_from_file(bank*& deposits, int& clientCount) {
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

    deposits = new bank[countq];
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

        deposits[i] = bank(last_name, first_name, balance);

        delete[] last_name;
        delete[] first_name;

        cout << "Запись " << (i + 1) << " загружена: ";
        deposits[i].display();
    }

    fclose(file);
    cout << "Всего загружено: " << clientCount << " записей\n";
}

void add_new_account(bank*& deposits, int& clientCount) {
    bank* new_deposits = new bank[clientCount + 1];

    for (int i = 0; i < clientCount; i++)
        new_deposits[i] = deposits[i];

    cout << "\nВвод данных для нового клиента:\n";
    new_deposits[clientCount].define();

    delete[] deposits;

    deposits = new_deposits;
    clientCount++;

    cout << "Новый счет добавлен! Всего записей: " << clientCount << endl;
}

int main() {
    setlocale(LC_ALL, "russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int choice;
    bank* deposits = NULL;
    int clientCount = 0;

    load_from_file(deposits, clientCount);

    if (clientCount == 0) {
        cout << "База данных пуста. Хотите ввести данные с клавиатуры? (1 - да, 0 - нет): ";
        int init_choice;
        cin >> init_choice;

        if (init_choice == 1) {
            int initial_count;
            cout << "Введите количество начальных счетов: ";
            cin >> initial_count;

            if (initial_count > 0) {
                deposits = new bank[initial_count];
                clientCount = initial_count;

                for (int i = 0; i < initial_count; i++) {
                    cout << "\nВвод данных для клиента " << i + 1 << ":\n";
                    deposits[i].define();
                }
            }
        }
    }

    do {
        system("cls");
        printf("========================================\n");
        printf("   БАНКОВСКАЯ СИСТЕМА - ДЕПОЗИТЫ       \n");
        printf("========================================\n");
        printf("Текущее количество записей: %d\n", clientCount);
        printf("Добавить новый счет .................. 1\n");
        printf("Распечатать базу счетов .............. 2\n");
        printf("Поиск счетов по сумме ................ 3\n");
        printf("Поиск счетов по клиенту .............. 4\n");
        printf("Выход из программы ................... 5\n");
        printf("........................................\n");
        printf("Введите номер функции: ");
        cin >> choice;

        switch (choice) {
        case 1:
            add_new_account(deposits, clientCount);
            system("pause");
            break;

        case 2:
            if (deposits == NULL || clientCount == 0) {
                cout << "База данных пуста!\n";
            }
            else {
                cout << "\n=== СПИСОК ВСЕХ КЛИЕНТОВ ===\n";
                for (int i = 0; i < clientCount; i++) {
                    cout << i + 1 << ". ";
                    deposits[i].display();
                }
            }
            system("pause");
            break;

        case 3: { 
            if (deposits == NULL || clientCount == 0) {
                cout << "Нет данных для поиска!\n";
            }
            else {
                double minSum;
                cout << "Введите минимальную сумму: ";
                cin >> minSum;

                bank temp;
                temp.findClientsBySum(deposits, clientCount, minSum);
            }
            system("pause");
            break;
        }

        case 4:
            find_by_client(deposits, clientCount);
            system("pause");
            break;

        case 5:
            printf("Сохранение данных...\n");
            if (clientCount > 0) {
                save_to_file(deposits, clientCount);
            }
            printf("Завершение программы...\n");
            break;

        default:
            printf("Неверный выбор! Попробуйте снова.\n");
            system("pause");
        }
    } while (choice != 5);

    delete[] deposits;
    deposits = NULL;

    return 0;
}