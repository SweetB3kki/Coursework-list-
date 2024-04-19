#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <stdbool.h>
#include <locale.h>

const int len = 15;

struct MatVal {
	int index;
	int factoryNum;
	int branchNum;
	char respPerson[len]; 
	float initialStock;
	float receivedStock;
	float disposedStock;
};

struct bidir_list {
	struct MatVal inf;
	struct bidir_list *nextl, *nextr;
};

bool isItInt(char *input) {
    if (*input == '\0') {
        return false;
    }
    for (; *input != '\0'; input++) {
        if (!isdigit(*input) && (*input != '-' || *input != '+')) {
            return false;
        }
    }
    return true;
}

int safeIntInput(const char *message) {
    char input[100];
    while (1) {
        printf("%s", message);
        scanf("%s", input);
        if (isItInt(input)) {
            return atoi(input);
        } else {
            printf("\n Input error. Enter a number\n");
        }
    }
}

bool isItFloat(char *input) {
    if (*input == '\0') {
        return false;
    }
    int pointCount = 0;
    for (; *input != '\0'; input++) {
        if (!isdigit(*input)) {
            if (*input == '.' && pointCount == 0) {
                pointCount++;
            } else {
                return false;
            }
        }
    }
    return true;
}

float safeFloatInput(const char *message) {
    char input[100];
    while (1) {
        printf("%s", message);
        scanf("%s", input);
        if (isItFloat(input)) {
            return atof(input);
        } else {
            printf("\n Input error. Enter a fractional number\n");
        }
    }
}

struct MatVal read_data();
struct bidir_list *create_list(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *add_sort(struct bidir_list *L, struct bidir_list *R, struct MatVal data);
struct bidir_list *load_data(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *free_data(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *del_elem(struct bidir_list *L, struct bidir_list *R, int num);
struct bidir_list *edit_elem(struct bidir_list *L, struct bidir_list *R);
int save_data(struct bidir_list *L, struct bidir_list *R);
void endBranchStock(struct bidir_list *L, struct bidir_list *R);
void endFactoryStock(struct bidir_list *L, struct bidir_list *R);
void endReport(struct bidir_list *L, struct bidir_list *R);
void print_list(struct bidir_list *L, struct bidir_list *R);
void delay() {
	puts("\n Press any key to continue...");
	getch();
}

int main() {
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
//	setlocale(LC_ALL,"rus");
	system("chcp 1251 > nul");
	system("cls");
	int num;
	char c;
	struct bidir_list *L, *R, *t;
	R = L = NULL;
	struct MatVal data;
	while(1) {
		system("cls");
	    puts("\n 1. Организация списка.");
	    puts(" 2. Вывести список.");
	    puts(" 3. Удалить элемент.");
	    puts(" 4. Редактировать элемент.");
	    puts(" 5. Стоимостная ведомость за мат. ценности по филиалу.");
	    puts(" 6. Стоимостная ведомость за мат. ценности по заводу.");
	    puts(" 7. Полная ведомость по всему заводу.");
	    puts(" 8. Сохранить в файл.");
	    puts(" 9. Загрузить из файла.");
	    puts(" 0. Выход.");
	    puts("\n Введите число от 1 до 9, 0 - для выхода");
		c = getch();
		switch(c) {
			case '1':
				L = create_list(L, R);
				t = L;
				for(; t != NULL; t = t->nextr) {
					R = t;
				}
				break;
			case '2':
				print_list(L, R);
				break;
			case '3':
				num = safeIntInput("\n Write Index of Element >> ");
				L = del_elem(L, R, num);
				if (L == NULL) {
					R = L;
				}
				t = L;
				for(; t != NULL; t = t->nextr) {
					R = t;
				}
				break;
			case '4':
				edit_elem(L, R);
				break;
			case '5':
				endBranchStock(L, R);
				break;
			case '6':
				endFactoryStock(L, R);
				break;
			case '7':
				endReport(L, R);
				break;
			case '8':
				save_data(L, R);
				break;
			case '9':
				R = load_data(L, R);
				t = R;
				for(; t != NULL; t = t->nextl) {
					L = t;
				}				
				break;
			case '-':
				while (1) {
					system("cls");
					puts("\n type 1");
					puts(" type 2");
					c = getch();
					switch (c) {
						case '1':
							puts("\n 1");
							break;
						case '2':
							puts(" 2");
							break;
					}
				}
			case '0':
				L = free_data(L, R);
				R = L;
				puts("\n List is free.");
				return 0;
		}
		delay();
	}
}

struct MatVal read_data() {
	struct MatVal data;
	char input[10]; 
	int c = 0;
	data.index = safeIntInput("\n Write Index Number >> ");
	data.factoryNum = safeIntInput("\n Write Factory Number >> ");
	data.branchNum = safeIntInput("\n Write Branch Number >> ");
	printf("\n Write Surname of Responsibility Person >> ");
	scanf("%s", &data.respPerson);
    data.initialStock = safeFloatInput("\n Write Initial Stock >> ");
	data.receivedStock = safeFloatInput("\n Write Received Stock >> ");
    data.disposedStock = safeFloatInput("\n Write Disposed Stock >> ");
	puts("\n Data recorded.");
	return data;
}

void endBranchStock(struct bidir_list *L, struct bidir_list *R) {
    struct bidir_list *t = L;
    int factoryNum;
	factoryNum = safeIntInput("\n Enter factory number >> ");
    if (R->inf.factoryNum < factoryNum) {
		printf("\n There is no such factory.");
		return;
	}
    puts("  _____________________________________");
	puts(" | Factory N | Branch N | Ending Stock |");
    while(t != NULL) {
	    if (t->inf.factoryNum == factoryNum) {
			puts(" |-----------|----------|--------------|");
	        float endStock = ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
	        printf(" | %-9d | %-8d | %-12.2f |\n", t->inf.factoryNum,
				t->inf.branchNum, endStock);
		}
		t = t->nextr;
	}
	puts(" |___________|__________|______________|");
}

void endFactoryStock(struct bidir_list *L, struct bidir_list *R) {
    int factoryNum;
    float endFactoryStock = 0;
    struct bidir_list *t = L;
	factoryNum = safeIntInput("\n Enter factory number >> ");
    if (R->inf.factoryNum < factoryNum) {
		printf("\n There is no such factory.");
		return;
	}
	puts("  __________________________");
	puts(" | Factory N | Ending Stock |");
    while (t != NULL) {
        if (t->inf.factoryNum == factoryNum) {
            endFactoryStock += ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
        }
        t = t->nextr;
    }
    puts(" |-----------|--------------|");
    printf(" | %-9d | %-12.2f |\n", factoryNum, endFactoryStock);
    puts(" |___________|______________|");
}

void endReport(struct bidir_list *L, struct bidir_list *R) {
    int factoryNum;
    float initialStockTotal = 0;
    float receivedStockTotal = 0;
    float disposedStockTotal = 0;
    float endStockTotal = 0;

    struct bidir_list *t = L;
    factoryNum = safeIntInput("\n Enter factory number >> ");
    if (R->inf.factoryNum < factoryNum) {
    	printf("\n There is no such factory.");
    	return;
	}
    while (t != NULL) {
        if (t->inf.factoryNum == factoryNum) {
            initialStockTotal += t->inf.initialStock;
            receivedStockTotal += t->inf.receivedStock;
            disposedStockTotal += t->inf.disposedStock;
            endStockTotal += ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
        }
        t = t->nextr;
    }
	puts("  ____________________________________________________________________________");
	puts(" | Factory N | Initial Stock | Received Stock | Disposed Stock | Ending Stock |");
	puts(" |-----------|---------------|----------------|----------------|--------------|");
	printf(" | %-9d | %-13.2f | %-14.2f | %-14.2f | %-12.2f |\n", factoryNum, initialStockTotal, 
		receivedStockTotal, disposedStockTotal, endStockTotal);
	puts(" |___________|_______________|________________|________________|______________|");
}

struct bidir_list *edit_elem(struct bidir_list *L, struct bidir_list *R) {
	if (L == NULL) {
		puts("\n List is empty.");
		return L;
	}
	struct bidir_list *t = L;
	char data[len];
	system("cls");
	int num = safeIntInput("\n Write Index of Element >> ");
	if ((R->inf.index < num) || (num == 0)) {
		puts("\n There is no such element.");
		return L;
	}
	while(t->inf.index != num) {
		t = t->nextr;
	}
	char c;
	while(1) {
		system("cls");
		puts("  _____________________________________________________________________________________________");
		puts(" | n.n | Factory N | Branch N | Respons. Person | Init Stock | Received Stock | Disposed Stock |");
		puts(" |-----|-----------|----------|-----------------|------------|----------------|----------------|");
		printf(" | %-3d | %-9d | %-8d | %-15s | %-10.2f | %-14.2f | %-14.2f |\n", 
			t->inf.index, t->inf.factoryNum, t->inf.branchNum, t->inf.respPerson, 
			t->inf.initialStock, t->inf.receivedStock, t->inf.disposedStock);
		puts(" |_____|___________|__________|_________________|____________|________________|________________|");
		puts("\n 1. Factory");
		puts(" 2. Branch.");
		puts(" 3. Responsibility Person.");
		puts(" 4. Initial Stock.");
		puts(" 5. Received Stock.");
		puts(" 6. Disposed Stock.");
		puts(" 7. All The Element.");
		puts(" 8. Delete Element.");
		puts(" 0. Exit.");
		puts("\n Choose key from 1 to 7, 0 - to exit");
		c = getch();
		switch(c) {
			case '1':
				t->inf.factoryNum = safeIntInput("\n Write Factory Number >> ");
				break;
			case '2':
				t->inf.branchNum = safeIntInput("\n Write Branch Number >> ");
				break;
			case '3':
				printf("\n Write Surname of Responsibility Person >> ");
				scanf("%s", &t->inf.respPerson);
				break;
			case '4':
				t->inf.initialStock = safeFloatInput("\n Write Initial Stock >> ");
				break;
			case '5':
				t->inf.receivedStock = safeFloatInput("\n Write Received Stock >> ");
				break;
			case '6':
				t->inf.disposedStock = safeFloatInput("\n Write Disposed Stock >> ");
				break;
			case '7':
				t->inf.factoryNum = safeIntInput("\n Write Factory Number >> ");
				t->inf.branchNum = safeIntInput("\n Write Branch Number >> ");
				printf("\n Write Surname of Responsibility Person >> ");
				scanf("%s", &t->inf.respPerson);
			    t->inf.initialStock = safeFloatInput("\n Write Initial Stock >> ");
				t->inf.receivedStock = safeFloatInput("\n Write Received Stock >> ");
			    t->inf.disposedStock = safeFloatInput("\n Write Disposed Stock >> ");
				break;
			case '8':
				del_elem(L, R, t->inf.index);
				return L;
			case '0':
				return L;
		}
		puts("\n Data recorded.");
		delay();
	}
}

struct bidir_list *create_list(struct bidir_list *L, struct bidir_list *R) {
	struct bidir_list *t;
	struct MatVal data;
	int f1 = 0;
	do {
		data = read_data();
		L = add_sort(L, R, data);
		t = L;
		if (L == NULL) {
			R = L;
		}
		for(; t != NULL; t = t->nextr) {
			R = t;
		}
		printf("\n Write 1 to continue, 0 - to exit -> ");
		scanf("%d", &f1);
		} while (f1);
	return L;
}

void print_list(struct bidir_list *L, struct bidir_list *R) {
	struct bidir_list *t;
	if(L == NULL) {
		puts("\n List is empty.");
		return;
	}
	puts("  _____________________________________________________________________________________________");
	puts(" | n.n | Factory N | Branch N | Respons. Person | Init Stock | Received Stock | Disposed Stock |");
	t = L;
	while(t != NULL) {
		puts(" |-----|-----------|----------|-----------------|------------|----------------|----------------|");
		printf(" | %-3d | %-9d | %-8d | %-15s | %-10.2f | %-14.2f | %-14.2f |\n", 
			t->inf.index, t->inf.factoryNum, t->inf.branchNum, t->inf.respPerson, 
			t->inf.initialStock, t->inf.receivedStock, t->inf.disposedStock);
		t = t->nextr;
	}
	puts(" |_____|___________|__________|_________________|____________|________________|________________|");
	return;
}

struct bidir_list *add_sort(struct bidir_list *L, struct bidir_list *R, struct MatVal data) {
	struct bidir_list *t1, *t2;
	t2 = (struct bidir_list *) malloc(sizeof(struct bidir_list));
	t2->inf = data;
	if(L == NULL) {
		t2->nextl = NULL;
		t2->nextr = L;
		L = R = t2;
		return L;
	}
	t1 = L;
	for(; t1 != NULL; t1 = t1->nextr) {
		if(t2->inf.index == t1->inf.index) {
			puts("\n There is already such an element");
			return L;
		}
		if((t2->inf.index < t1->inf.index) && (t1->nextl == NULL)) {
			t2->nextl = NULL;
			t2->nextr = L;
			L->nextl = t2;
			L = t2;
			return L;
		}
		if(t2->inf.index < t1->inf.index) {
			t2->nextl = t1->nextl;
			t2->nextr = t1;
			t1->nextl->nextr = t2;
			t1->nextl = t2;
			return L;
		}
		if((t2->inf.index > t1->inf.index) && (t1->nextr == NULL)) {
			t2->nextr = NULL;
			t2->nextl = R;
			R->nextr = t2;
			R = t2;
			return L;
		}
		if(t2->inf.index > t1->inf.index) {
			continue;
		}
	}
	return L;
}

struct bidir_list *del_elem(struct bidir_list *L, struct bidir_list *R, int num) {
	if (L == NULL) {
		puts("\n List is alredy empty.");
		return L;
	}
	struct bidir_list *t = L;
	while(t != NULL) {
		if (t->inf.index == num) {
			if (t == L) {
				if (t->nextr == NULL) {
					free(t);
					puts("\n Delete completed successfully.");
					return NULL;
				} else {
					L = t->nextr;
					L->nextl = NULL;
					free(t);
					puts("\n Delete completed successfully.");
					return L;
				}
			} else if (t == R) {
				R = t->nextl;
				R->nextr = NULL;
				free(t);
				puts("\n Delete completed successfully.");
				return L;
			} else {
				struct bidir_list *t1 = t->nextl;
				t1->nextr = t->nextr;
				t->nextr->nextl = t1;
				free(t);
				puts("\n Delete completed successfully.");
				return L;
			}
		}
		t = t->nextr;
	}
	puts("\n There is no such element.");
	return L;
}

struct bidir_list *load_data(struct bidir_list *L, struct bidir_list *R) {
	FILE *f = fopen("data.dat", "rb+");
	if (!f) {
		f = fopen("data.dat", "wb+");
		if (!f) {
			puts(" Can't open (create) file.\n");
			return R;
		}
	}
	struct bidir_list *t1 = (struct bidir_list *) malloc(sizeof(struct bidir_list));
	long len = 0;
	while(fread(&t1->inf, sizeof(t1->inf), 1, f)) {
		struct bidir_list *t2 = (struct bidir_list *) malloc(sizeof(struct bidir_list));
		t2->inf = t1->inf;
		if (L == NULL) {
			L = add_sort(L, R, t2->inf);
			if (R == NULL) {
				R = L; 
			}
			continue;
		}
		t2->nextr = NULL;
		t2->nextl = R;
		if (R == NULL) {
	        R = L = t2;
	    } else {
	        R->nextr = t2;
	        R = t2;
	    }
	    t1->nextr = (struct bidir_list *) malloc(sizeof(struct bidir_list));
		t1 = t1->nextr;
	}
	free(t1);
	fclose(f);
	puts("\n Data is upload.");
	return R;
}

int save_data(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n List is empty.");
		return 1;
	}
	FILE *f = fopen("data.dat", "wb+");
	if (!f) { 
		puts(" Can't open (create) file.\n");
		return 1;
	}
	struct bidir_list *t;
	long len = 0;
	t = L;
	while(t != NULL) {
		fwrite(&t->inf, sizeof(t->inf), 1, f);
		t = t->nextr;
	}
	fclose(f);
	puts("\n Data is recorded.");
	return 0;
}

struct bidir_list *free_data(struct bidir_list *L, struct bidir_list *R) {
	struct bidir_list *t;
	while(L != NULL) {
		if (L == NULL) {
			puts("\n List is alredy empty.");
			return L;
		}
		t = L;
		L = t->nextr;
		if (L != NULL) { 
			L->nextl = NULL;
		}
		free(t);
	}
	return L;
}