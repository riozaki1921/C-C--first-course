#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

typedef struct {
    char last_name[50];
    char first_name[50];
} fio;

typedef struct {
    int day;
    char month[20];
    int year;
} date;

typedef struct {
    int account_number;
    fio client_fio;
    date opening_date;
    double balance;
} deposit;

deposit* deposits = NULL;
int deposit_count = 0;
const char* FILENAME = "deposits.dat";


int compare_dates(date date1, date date2) {
    if (date1.year != date2.year)
        return date1.year > date2.year;

    const char* months[] = { "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
                           "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь" };
    int month1_num = -1, month2_num = -1;

    for (int i = 0; i < 12; i++) {
        if (strcmp(date1.month, months[i]) == 0) month1_num = i;
        if (strcmp(date2.month, months[i]) == 0) month2_num = i;
    }

    if (month1_num == -1 || month2_num == -1) {
        printf("Некорректное название месяца\n");
        return 0;
    }

    if (month1_num != month2_num)
        return month1_num > month2_num;

    return date1.day > date2.day;
}


void load_from_file() {
    FILE* file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("Файл данных не найден. Будет создан новый.\n");
        return;
    }

    size_t items_read = fread(&deposit_count, sizeof(int), 1, file);

    if (items_read != 1) {
        printf("Ошибка чтения количества записей!\n");
        deposit_count = 0;
        return;
    }

    if (deposit_count > 0) {
        deposits = (deposit*)malloc(deposit_count * sizeof(deposit));
        if (deposits == NULL) {
            printf("Ошибка выделения памяти!\n");
            fclose(file);
            return;
        }
        fread(deposits, sizeof(deposit), deposit_count, file);
    }

    fclose(file);
    printf("Данные загружены из файла. Записей: %d\n", deposit_count);
}


void save_to_file() {
    FILE* file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Ошибка при создании файла!\n");
        return;
    }

    fwrite(&deposit_count, sizeof(int), 1, file);
    if (deposit_count > 0) {
        fwrite(deposits, sizeof(deposit), deposit_count, file);
    }

    fclose(file);
    printf("Данные сохранены в файл.\n");
}

void add_deposit() {
    printf("========================================\n");
    printf("         ДОБАВЛЕНИЕ НОВОГО СЧЕТА       \n");
    printf("========================================\n");

    deposits = (deposit*)realloc(deposits, (deposit_count + 1) * sizeof(deposit));
    if (deposits == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    printf("Номер счета: ");
    scanf("%d", &deposits[deposit_count].account_number);
    getchar();

    printf("Фамилия клиента: ");
    gets_s(deposits[deposit_count].client_fio.last_name, 50);

    printf("Имя клиента: ");
    gets_s(deposits[deposit_count].client_fio.first_name, 50);

    printf("Дата открытия счета:\n");
    printf("  День: ");
    scanf("%d", &deposits[deposit_count].opening_date.day);
    getchar();

    printf("  Месяц (например, Январь): ");
    gets_s(deposits[deposit_count].opening_date.month, 20);

    printf("  Год: ");
    scanf("%d", &deposits[deposit_count].opening_date.year);
    getchar();

    printf("Сумма на счете: ");
    scanf("%lf", &deposits[deposit_count].balance);
    getchar();

    deposit_count++;
    printf("\nСчет успешно добавлен! Всего счетов: %d\n", deposit_count);
    system("pause");
}

void print_deposits() {
    printf("========================================\n");
    printf("     ИНФОРМАЦИЯ О СЧЕТАХ В БАНКЕ       \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("Нет данных о счетах.\n");
        system("pause");
        return;
    }

    printf("Номер     Клиент              Дата открытия     Сумма\n");
    printf("------    ------------------  --------------    --------------\n");

    double total_balance = 0;
    for (int i = 0; i < deposit_count; i++) {
        printf("%-9d %-11s %-9s   %02d %s %d   %-15.2lf\n",
            deposits[i].account_number,
            deposits[i].client_fio.last_name,
            deposits[i].client_fio.first_name,
            deposits[i].opening_date.day,
            deposits[i].opening_date.month,
            deposits[i].opening_date.year,
            deposits[i].balance);
        total_balance += deposits[i].balance;
    }

    printf("------    ------------------  --------------    --------------\n");
    printf("Общая сумма на всех счетах: %.2lf\n", total_balance);
    printf("Количество счетов: %d\n", deposit_count);
    printf("========================================\n");
    system("pause");
}

void find_by_balance() {
    printf("========================================\n");
    printf("     ПОИСК СЧЕТОВ ПО СУММЕ             \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("Нет данных о счетах.\n");
        system("pause");
        return;
    }

    double min_balance;
    printf("Введите минимальную сумму для поиска: ");
    scanf("%lf", &min_balance);
    getchar();

    printf("\nСчета с суммой больше %.2lf:\n", min_balance);
    printf("Номер     Клиент              Дата открытия     Сумма\n");
    printf("------    ------------------  --------------    -----\n");

    int found = 0;
    for (int i = 0; i < deposit_count; i++) {
        if (deposits[i].balance > min_balance) {
            printf("%-9d %-11s %-9s %2d %s %d %-10.2lf\n",
                deposits[i].account_number,
                deposits[i].client_fio.last_name,
                deposits[i].client_fio.first_name,
                deposits[i].opening_date.day,
                deposits[i].opening_date.month,
                deposits[i].opening_date.year,
                deposits[i].balance);
            found++;
        }
    }

    if (!found) {
        printf("Счетов с суммой больше %.2lf не найдено.\n", min_balance);
    }
    else {
        printf("Найдено %d клиентов\n", found);
    }

    printf("========================================\n");
    system("pause");
}

void find_by_client() {
    printf("========================================\n");
    printf("     ПОИСК СЧЕТОВ ПО КЛИЕНТУ           \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("Нет данных о счетах.\n");
        system("pause");
        return;
    }

    char search_last_name[50];
    char search_first_name[50];

    printf("Введите фамилию клиента: ");
    gets_s(search_last_name, 50);

    printf("Введите имя клиента: ");
    gets_s(search_first_name, 50);

    printf("\nСчета клиента %s %s:\n", search_last_name, search_first_name);
    printf("Номер     Клиент              Дата открытия     Сумма\n");
    printf("------    ------------------  --------------    -----\n");

    int found = 0;
    for (int i = 0; i < deposit_count; i++) {
        if (strcmp(deposits[i].client_fio.last_name, search_last_name) == 0 &&
            strcmp(deposits[i].client_fio.first_name, search_first_name) == 0) {
            printf("%-9d %-11s %-9s %2d %s %d %-10.2lf\n",
                deposits[i].account_number,
                deposits[i].client_fio.last_name,
                deposits[i].client_fio.first_name,
                deposits[i].opening_date.day,
                deposits[i].opening_date.month,
                deposits[i].opening_date.year,
                deposits[i].balance);
            found = 1;
        }
    }

    if (!found) {
        printf("Счетов для клиента %s %s не найдено.\n", search_last_name, search_first_name);
    }

    printf("========================================\n");
    system("pause");
}



void find_by_opening_date() {
    printf("========================================\n");
    printf("  ПОИСК СЧЕТОВ, ОТКРЫТЫХ ПОСЛЕ ДАТЫ    \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("Нет данных о счетах.\n");
        system("pause");
        return;
    }

    date search_date;
    printf("Введите дату для поиска:\n");
    printf("  День: ");
    scanf("%d", &search_date.day);
    getchar();

    printf("  Месяц: ");
    gets_s(search_date.month, 20);

    printf("  Год: ");
    scanf("%d", &search_date.year);
    getchar();

    printf("\nСчета, открытые после %2d %s %d:\n",
        search_date.day, search_date.month, search_date.year);
    printf("Номер     Клиент              Дата открытия     Сумма\n");
    printf("------    ------------------  --------------    -----\n");

    int found = 0;
    for (int i = 0; i < deposit_count; i++) {
        if (compare_dates(deposits[i].opening_date, search_date)) {
            printf("%-9d %-11s %-9s %2d %s %d %-10.2lf\n",
                deposits[i].account_number,
                deposits[i].client_fio.last_name,
                deposits[i].client_fio.first_name,
                deposits[i].opening_date.day,
                deposits[i].opening_date.month,
                deposits[i].opening_date.year,
                deposits[i].balance);
            found++;
        }
    }

    if (!found) {
        printf("Счетов, открытых после указанной даты, не найдено.\n");
    }
    else {
        printf("Найдено %d счетов\n", found);
    }

    printf("========================================\n");
    system("pause");
}

void free_memory() {
    free(deposits);
    deposits = NULL;
    deposit_count = 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int choice;

    printf("========================================\n");
    printf("   БАНКОВСКАЯ СИСТЕМА - ДЕПОЗИТЫ       \n");
    printf("========================================\n");

    load_from_file();

    if (deposit_count == 0) {
        printf("База данных пуста. Хотите ввести данные с клавиатуры? (1 - да, 0 - нет): ");
        int init_choice;
        scanf("%d", &init_choice);
        getchar();

        if (init_choice == 1) {
            int initial_count;
            printf("Введите количество начальных счетов: ");
            scanf("%d", &initial_count);
            getchar();

            if (initial_count > 0) {
                deposits = (deposit*)malloc(initial_count * sizeof(deposit));
                if (deposits == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    return 1;
                }

                for (int i = 0; i < initial_count; i++) {
                    printf("\n=== Счет #%d ===\n", i + 1);

                    printf("Номер счета: ");
                    scanf("%d", &deposits[i].account_number);
                    getchar();

                    printf("Фамилия клиента: ");
                    gets_s(deposits[i].client_fio.last_name, 50);

                    printf("Имя клиента: ");
                    gets_s(deposits[i].client_fio.first_name, 50);

                    printf("Дата открытия счета:\n");
                    printf("  День: ");
                    scanf("%d", &deposits[i].opening_date.day);
                    getchar();

                    printf("  Месяц: ");
                    gets_s(deposits[i].opening_date.month, 20);

                    printf("  Год: ");
                    scanf("%d", &deposits[i].opening_date.year);
                    getchar();

                    printf("Сумма на счете: ");
                    scanf("%lf", &deposits[i].balance);
                    getchar();
                }

                deposit_count = initial_count;
            }
        }
    }

    do {
        system("cls");
        printf("========================================\n");
        printf("   БАНКОВСКАЯ СИСТЕМА - ДЕПОЗИТЫ       \n");
        printf("========================================\n");
        printf("Добавить новый счет .................. 1\n");
        printf("Распечатать базу счетов .............. 2\n");
        printf("Поиск счетов по сумме ................ 3\n");
        printf("Поиск счетов по клиенту .............. 4\n");
        printf("Поиск счетов по дате открытия ........ 5\n");
        printf("Выход из программы .................. 6\n");
        printf("........................................\n");
        printf("Введите номер функции: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            add_deposit();
            break;
        case 2:
            print_deposits();
            break;
        case 3:
            find_by_balance();
            break;
        case 4:
            find_by_client();
            break;
        case 5:
            find_by_opening_date();
            break;
        case 6:
            printf("Сохранение данных...\n");
            save_to_file();
            printf("Завершение программы...\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
            system("pause");
        }
    } while (choice != 6);

    free_memory();

    return 0;
}
