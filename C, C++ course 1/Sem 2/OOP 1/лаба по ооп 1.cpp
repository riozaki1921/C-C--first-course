#define _crt_secure_no_warnings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

typedef struct {
    char* last_name;
    char* first_name;
} fio;

typedef struct {
    int day;
    char month[10];
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
const char* filename = "deposits_data.txt";
char buffer[50];


char* save_from_buffer() {
    if (buffer == NULL)
        return ;

    size_t len = strlen(buffer);
    char* new_str = (char*)malloc(len + 1);

    if (new_str == NULL) {
        memset(buffer, 0, sizeof(buffer));
        return NULL;
    }
    
    strcpy(new_str, buffer);
    memset(buffer, 0, sizeof(buffer));
    return new_str;
}



int compare_dates(date date1, date date2) {
    if (date1.year != date2.year)
        return date1.year > date2.year;

    const char* months[] = { "январь", "февраль", "март", "апрель", "май", "июнь",
                           "июль", "август", "сентябрь", "октябрь", "ноябрь", "декабрь" };
    int month1_num = -1, month2_num = -1;

    for (int i = 0; i < 12; i++) {
        if (strcmp(date1.month, months[i]) == 0) month1_num = i;
        if (strcmp(date2.month, months[i]) == 0) month2_num = i;
    }

    if (month1_num == -1 || month2_num == -1) {
        printf("некорректное название месяца\n");
        return 0;
    }

    if (month1_num != month2_num)
        return month1_num > month2_num;

    return date1.day > date2.day;
}


void save_to_file() {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("ошибка при создании файла!\n");
        return;
    }

    fwrite(&deposit_count, sizeof(int), 1, file);

    for (int i = 0; i < deposit_count; i++) {
        fwrite(&deposits[i].account_number, sizeof(int), 1, file);

        size_t last_len = deposits[i].client_fio.last_name ?
            strlen(deposits[i].client_fio.last_name) + 1 : 0;
        fwrite(&last_len, sizeof(size_t), 1, file);
        if (last_len > 0) {
            fwrite(deposits[i].client_fio.last_name, 1, last_len, file);
        }

        size_t first_len = deposits[i].client_fio.first_name ?
            strlen(deposits[i].client_fio.first_name) + 1 : 0;
        fwrite(&first_len, sizeof(size_t), 1, file);
        if (first_len > 0) {
            fwrite(deposits[i].client_fio.first_name, 1, first_len, file);
        }

        fwrite(&deposits[i].opening_date.day, sizeof(int), 1, file);
        fwrite(deposits[i].opening_date.month, 1, 10, file);
        fwrite(&deposits[i].opening_date.year, sizeof(int), 1, file);

        fwrite(&deposits[i].balance, sizeof(double), 1, file);
    }

    fclose(file);
    printf("данные сохранены в файл.\n");
}


void load_from_file() {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("файл данных не найден. будет создан новый.\n");
        return;
    }

    fread(&deposit_count, sizeof(int), 1, file);

    if (deposit_count > 0) {
        deposits = (deposit*)malloc(deposit_count * sizeof(deposit));
        if (deposits == NULL) {
            printf("ошибка выделения памяти!\n");
            fclose(file);
            return;
        }

        for (int i = 0; i < deposit_count; i++) {
            fread(&deposits[i].account_number, sizeof(int), 1, file);

            size_t last_len;
            fread(&last_len, sizeof(size_t), 1, file);
            if (last_len > 0) {
                deposits[i].client_fio.last_name = (char*)malloc(last_len);
                fread(deposits[i].client_fio.last_name, 1, last_len, file);
            }
            else {
                deposits[i].client_fio.last_name = NULL;
            }

            size_t first_len;
            fread(&first_len, sizeof(size_t), 1, file);
            if (first_len > 0) {
                deposits[i].client_fio.first_name = (char*)malloc(first_len);
                fread(deposits[i].client_fio.first_name, 1, first_len, file);
            }
            else {
                deposits[i].client_fio.first_name = NULL;
            }

            fread(&deposits[i].opening_date.day, sizeof(int), 1, file);
            fread(deposits[i].opening_date.month, 1, 10, file);
            fread(&deposits[i].opening_date.year, sizeof(int), 1, file);

            fread(&deposits[i].balance, sizeof(double), 1, file);
        }
    }

    fclose(file);
    printf("данные загружены из файла. записей: %d\n", deposit_count);
}

void add_deposit() {
    printf("========================================\n");
    printf("         добавление нового счета       \n");
    printf("========================================\n");

    deposits = (deposit*)realloc(deposits, (deposit_count + 1) * sizeof(deposit));
    if (deposits == NULL) {
        printf("ошибка выделения памяти!\n");
        return;
    }
    deposits[deposit_count].client_fio.last_name = NULL;
    deposits[deposit_count].client_fio.first_name = NULL;
    printf("номер счета: ");
    scanf("%d", &deposits[deposit_count].account_number);
    
    getchar();

    printf("фамилия клиента: ");
    gets_s(buffer, 50);
    deposits[deposit_count].client_fio.last_name = save_from_buffer();

    printf("имя клиента: ");
    gets_s(buffer, 50);
    deposits[deposit_count].client_fio.first_name = save_from_buffer();

    printf("дата открытия счета:\n");
    printf("  день: ");
    scanf("%d", &deposits[deposit_count].opening_date.day);
    getchar();

    printf("  месяц: ");
    gets_s(deposits[deposit_count].opening_date.month, 20);

    printf("  год: ");
    scanf("%d", &deposits[deposit_count].opening_date.year);
    getchar();

    printf("сумма на счете: ");
    scanf("%lf", &deposits[deposit_count].balance);
    getchar();

    deposit_count++;
    printf("\nсчет успешно добавлен! всего счетов: %d\n", deposit_count);
    system("pause");
}

void print_deposits() {
    printf("========================================\n");
    printf("     информация о счетах в банке       \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("нет данных о счетах.\n");
        system("pause");
        return;
    }

    printf("номер     клиент              дата открытия     сумма\n");
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
    printf("общая сумма на всех счетах: %.2lf\n", total_balance);
    printf("количество счетов: %d\n", deposit_count);
    printf("========================================\n");
    system("pause");
}

void find_by_balance() {
    printf("========================================\n");
    printf("     поиск счетов по сумме             \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("нет данных о счетах.\n");
        system("pause");
        return;
    }

    double min_balance;
    printf("введите минимальную сумму для поиска: ");
    scanf("%lf", &min_balance);
    getchar();

    printf("\nсчета с суммой больше %.2lf:\n", min_balance);
    printf("номер     клиент              дата открытия     сумма\n");
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
        printf("счетов с суммой больше %.2lf не найдено.\n", min_balance);
    }
    else {
        printf("найдено %d клиентов\n", found);
    }

    printf("========================================\n");
    system("pause");
}

void find_by_client() {
    printf("========================================\n");
    printf("     поиск счетов по клиенту           \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("нет данных о счетах.\n");
        system("pause");
        return;
    }

    char search_last_name[50];
    char search_first_name[50];

    printf("введите фамилию клиента: ");
    gets_s(search_last_name, 50);

    printf("введите имя клиента: ");
    gets_s(search_first_name, 50);

    printf("\nсчета клиента %s %s:\n", search_last_name, search_first_name);
    printf("номер     клиент              дата открытия     сумма\n");
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
        printf("счетов для клиента %s %s не найдено.\n", search_last_name, search_first_name);
    }

    printf("========================================\n");
    system("pause");
}



void find_by_opening_date() {
    printf("========================================\n");
    printf("  поиск счетов, открытых после даты    \n");
    printf("========================================\n");

    if (deposit_count == 0) {
        printf("нет данных о счетах.\n");
        system("pause");
        return;
    }

    date search_date;
    printf("введите дату для поиска:\n");
    printf("  день: ");
    scanf("%d", &search_date.day);
    getchar();

    printf("  месяц: ");
    gets_s(search_date.month, 20);

    printf("  год: ");
    scanf("%d", &search_date.year);
    getchar();

    printf("\nсчета, открытые после %2d %s %d:\n",
        search_date.day, search_date.month, search_date.year);
    printf("номер     клиент              дата открытия     сумма\n");
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
        printf("счетов, открытых после указанной даты, не найдено.\n");
    }
    else {
        printf("найдено %d счетов\n", found);
    }

    printf("========================================\n");
    system("pause");
}

void free_memory() {
    for (int i=0; i < deposit_count; i++) {
        free(deposits[i].client_fio.last_name);
        deposits[i].client_fio.last_name = NULL;
        free(deposits[i].client_fio.first_name);
        deposits[i].client_fio.first_name = NULL;
    }

    free(deposits);
    deposits = NULL;
    deposit_count = 0;
}

int main() {
    setlocale(LC_ALL, "russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int choice;

    printf("========================================\n");
    printf("   банковская система - депозиты       \n");
    printf("========================================\n");

    load_from_file();

    if (deposit_count == 0) {
        printf("база данных пуста. хотите ввести данные с клавиатуры? (1 - да, 0 - нет): ");
        int init_choice;
        scanf("%d", &init_choice);
        getchar();

        if (init_choice == 1) {
            int initial_count;
            printf("введите количество начальных счетов: ");
            scanf("%d", &initial_count);
            getchar();

            if (initial_count > 0) {
                deposits = (deposit*)malloc(initial_count * sizeof(deposit));
                if (deposits == NULL) {
                    printf("ошибка выделения памяти!\n");
                    return 1;
                }

                for (int i = 0; i < initial_count; i++) {
                    deposits[i].client_fio.last_name = NULL;
                    deposits[i].client_fio.first_name = NULL;

                    printf("\n=== счет #%d ===\n", i + 1);

                    printf("номер счета: ");
                    scanf("%d", &deposits[i].account_number);
                    getchar();

                    printf("фамилия клиента: ");
                    gets_s(buffer, 50);
                    deposits[i].client_fio.last_name = save_from_buffer();

                    printf("имя клиента: ");
                    gets_s(buffer, 50);
                    deposits[i].client_fio.first_name = save_from_buffer();

                    printf("дата открытия счета:\n");
                    printf("  день: ");
                    scanf("%d", &deposits[i].opening_date.day);
                    getchar();

                    printf("  месяц: ");
                    gets_s(deposits[i].opening_date.month, 20);

                    printf("  год: ");
                    scanf("%d", &deposits[i].opening_date.year);
                    getchar();

                    printf("сумма на счете: ");
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
        printf("   банковская система - депозиты       \n");
        printf("========================================\n");
        printf("добавить новый счет .................. 1\n");
        printf("распечатать базу счетов .............. 2\n");
        printf("поиск счетов по сумме ................ 3\n");
        printf("поиск счетов по клиенту .............. 4\n");
        printf("поиск счетов по дате открытия ........ 5\n");
        printf("выход из программы .................. 6\n");
        printf("........................................\n");
        printf("введите номер функции: ");
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
            printf("сохранение данных...\n");
            save_to_file();
            printf("завершение программы...\n");
            break;
        default:
            printf("неверный выбор! попробуйте снова.\n");
            system("pause");
        }
    } while (choice != 6);

    free_memory();

    return 0;
}

