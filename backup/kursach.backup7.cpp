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

bool isItFilename(char *input) {
	if(*input == '\0') {
		return false;
	}
	for(; *input != '\0'; input++) {
		if((*input == '\\') || (*input == '/') || (*input == '|') || (*input == ':') || (*input == '*')
			|| (*input == '?') || (*input == '"') || (*input == '<') || (*input == '>')) {
				return false;	
			}
	}
	return true;
}

bool isItInt(char *input) {
    if(*input == '\0') {
        return false;
    }
    for(; *input != '\0'; input++) {
        if(!isdigit(*input) && (*input != '-' || *input != '+')) {
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
        if(isItInt(input)) {
            return atoi(input);
        } else {
            printf("\n Ошибка ввода. Введите число.\n");
        }
    }
}

bool isItFloat(char *input) {
    if(*input == '\0') {
        return false;
    }
    int pointCount = 0;
    for(; *input != '\0'; input++) {
        if(!isdigit(*input)) {
            if(*input == '.' && pointCount == 0) {
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
        if(isItFloat(input)) {
            return atof(input);
        } else {
            printf("\n Ошибка ввода. Введите дробное число.\n");
        }
    }
}

void delay() {
	puts("\n Нажмите любую клавишу, чтобы продолжить...");
	getch();
}

void GoToXY(short x, short y) {
    printf("\033[%d;%dH", y + 1, x + 1); // для перемещения курсора
}

void ConsoleCursorVisible(int show, short size) {
    if (show) {
        printf("\e[?25h"); // для отображения курсора
    } else {
        printf("\e[?25l"); // для скрытия курсора
    }
}

struct MatVal read_data();
struct bidir_list *sort_list(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *create_list(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *add_sort(struct bidir_list *L, struct bidir_list *R, struct MatVal data);
struct bidir_list *load_data(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *free_data(struct bidir_list *L, struct bidir_list *R);
struct bidir_list *del_elem(struct bidir_list *L, struct bidir_list *R, int num);
struct bidir_list *edit_elem(struct bidir_list *L, struct bidir_list *R, struct MatVal selectedElementData);
struct bidir_list *search_elem(struct bidir_list *L, struct bidir_list *R);
int save_data(struct bidir_list *L, struct bidir_list *R);
void swap(struct MarVal *a, struct MatVal *b);
void endBranchStock(struct bidir_list *L, struct bidir_list *R);
void endFactoryStock(struct bidir_list *L, struct bidir_list *R);
void endReport(struct bidir_list *L, struct bidir_list *R);
void print_list(struct bidir_list *L, struct bidir_list *R);

int main() {
	system("chcp 1251 > nul");
	system("cls");
	SetConsoleTitle("Курсовая работа");

	char Menu[][60] = {
	    "Организация списка.", 
	    "Вывести список.", 
	    "Поиск элемента.", 
	    "Сортировка списка.", 
	    "Стоимостная ведомость за мат. ценности по филиалу.", 
	    "Стоимостная ведомость за мат. ценности по заводу.",
	    "Полная ведомость по всему заводу.", 
	    "Сохранить в файл.", 
	    "Загрузить из файла",
	    "Выход."
	};

	struct bidir_list *L, *R, *t;
	R = L = NULL;
	struct MatVal data;
	
	char ch;
	int active_menu = 0;
	int menu_size = sizeof(Menu) / sizeof(Menu[0]);
	ConsoleCursorVisible(0, 100);
	
    while (1) {
		system("cls");
	    int x = 30, y = 10;
	    GoToXY(x, y);
	
	    for (int i = 0; i < menu_size; i++) {
	        if (i == active_menu) {
	            printf("\033[1;32m"); // для зеленого цвета текста
	        } else {
	            printf("\033[0;29m"); // для сброса цвета текста
	        }
	        GoToXY(x, y++);
	        printf("%s\n", Menu[i]);
	    }
	    
        ch = getch();
		switch (ch) {
		    case 27: //ESC
		    	system("cls");
				L = free_data(L, R);
				R = L;
				printf(" Список очищен.");
				return 0;
		    case 72: // UP
		        if (active_menu > 0) {
		            active_menu--;
		        }
		        break;
		    case 80: // DOWN
		        if (active_menu < sizeof(Menu) / sizeof(Menu[0]) - 1) {
		            active_menu++;
		        }
		        break;
		    case 13: // ENTER
		    	system("cls");
		    	printf("\033[0;29m"); 
				switch(active_menu) {
					case 0:
						L = create_list(L, R);
						t = L;
						for(; t != NULL; t = t->nextr) {
							R = t;
						}
						break;
					case 1:
						print_list(L, R);
						break;
					case 2:
					    L = search_elem(L, R);
					    t = L;
						for(; t != NULL; t = t->nextr) {
							R = t;
						}
						break;
					case 3:
						sort_list(L, R);
						t = L;
						for(; t != NULL; t = t->nextr) {
							R = t;
						}
						break;
					case 4:
						endBranchStock(L, R);
						break;
					case 5:
						endFactoryStock(L, R);
						break;
					case 6:
						endReport(L, R);
						break;
					case 7:
						save_data(L, R);
						break;
					case 8:
						R = load_data(L, R);
						t = R;
						for(; t != NULL; t = t->nextl) {
							L = t;
						}				
						break;
					case 9:
						L = free_data(L, R);
						R = L;
						printf(" Список очищен.");
						return 0;
				}
				delay();
		        break;
		}
    }
	return 0;
}

struct MatVal read_data() {
	struct MatVal data;
	char input[10]; 
	int c = 0;
	data.index = safeIntInput("\n Введите индекс элемента >> ");
	data.factoryNum = safeIntInput("\n Введите номер завода >> ");
	data.branchNum = safeIntInput("\n Введите номер филиала >> ");
	printf("\n Введите фамилию ответственного лица >> ");
	scanf("%s", &data.respPerson);
    data.initialStock = safeFloatInput("\n Введите начальные ценности >> ");
	data.receivedStock = safeFloatInput("\n Введите полученные ценности >> ");
    data.disposedStock = safeFloatInput("\n Введите выбывшие ценности >> ");
	puts("\n Данные записаны.");
	return data;
}

struct bidir_list *search_elem(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return NULL;
	}
	
	char field;
    char value[50];
    struct bidir_list *foundL = NULL;
	struct bidir_list *foundR = NULL;
    struct bidir_list *t = L;
    struct bidir_list *t2 = foundL;
    struct bidir_list *selectedNode = NULL;
    struct MatVal selectedElementData;
	char stringValue[30];
	int intValue = 0;
	float floatValue = 0.0;
	int selectedIndex;
	
	char Menu[][30] = {
	    "Индекс.", 
	    "Завод.", 
	    "Филиал.", 
	    "Ответственное лицо.", 
	    "Начальные ценности.", 
	    "Полученные ценности.",
	    "Выбывшие ценности.",
	    "Выход."
	};
	
	char ch;
	int active_menu = 0;
	int menu_size = sizeof(Menu) / sizeof(Menu[0]);
	
    while (1) {
		system("cls");
	    int x = 30, y = 10;
	    GoToXY(x, y);
	
	    for (int i = 0; i < menu_size; i++) {
	        if (i == active_menu) {
	            printf("\033[1;32m"); // для зеленого цвета текста
	        } else {
	            printf("\033[0;29m"); // для сброса цвета текста
	        }
	        GoToXY(x, y++);
	        printf("%s\n", Menu[i]);
	    }
	    
        ch = getch();
		switch (ch) {
		    case 27: //ESC
				return NULL;
		    case 72: // UP
		        if (active_menu > 0) {
		            active_menu--;
		        }
		        break;
		    case 80: // DOWN
		        if (active_menu < sizeof(Menu) / sizeof(Menu[0]) - 1) {
		            active_menu++;
		        }
		        break;
		    case 13: // ENTER
		    	system("cls");
		    	printf("\033[0;29m"); 
				if(active_menu == 7) {
					return L;
				}
			    printf("\n Введите значение для поиска >> ");
			    scanf("%s", value);
				
			    while (t != NULL) {
			        if ((active_menu == 3) && strstr(t->inf.respPerson, value) != NULL) {
			            foundL = add_sort(foundL, foundR, t->inf);
			            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
			            	foundR = t2;
						}
			        } else if (active_menu == 0) {
			            intValue = t->inf.index;
			            sprintf(stringValue, "%d", intValue);
			            if (strstr(stringValue, value) != NULL) {
				            foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        } else if (active_menu == 1) {
			            intValue = t->inf.factoryNum;
			            sprintf(stringValue, "%d", intValue);
			            if (strstr(stringValue, value) != NULL) {
			                foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        } else if (active_menu == 2) {
			            intValue = t->inf.branchNum;
			            sprintf(stringValue, "%d", intValue);
			            if (strstr(stringValue, value) != NULL) {
				            foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        } else if (active_menu == 4) {
			            floatValue = t->inf.initialStock;
			            sprintf(stringValue, "%.2f", floatValue);
			            if (strstr(stringValue, value) != NULL) {
				            foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        } else if (active_menu == 5) {
			            floatValue = t->inf.receivedStock;
			            sprintf(stringValue, "%.2f", floatValue);
			            if (strstr(stringValue, value) != NULL) {
				            foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        } else if (active_menu == 6) {
			            floatValue = t->inf.disposedStock;
			            sprintf(stringValue, "%.2f", floatValue);
			            if (strstr(stringValue, value) != NULL) {
				            foundL = add_sort(foundL, foundR, t->inf);
				            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
				            	foundR = t2;
							}
			            }
			        }
			
			        t = t->nextr;
			    }
				if (foundL != NULL) {
			        print_list(foundL, foundR);
			
			        selectedIndex = safeIntInput("\n Введите индекс элемента для работы (0 - чтобы вернуться) >> ");;
					if(selectedIndex == 0) {
						return L;
					}
				    selectedNode = foundL;
				
				    while (selectedNode != NULL) {
				        if (selectedNode->inf.index == selectedIndex) {
				        	selectedElementData = selectedNode->inf;
				            break;
				        }
				        selectedNode = selectedNode->nextr;
				    }
			
			        if (selectedNode != NULL) {
			            char c;
			            puts("\n Выберите действие (E - редактировать, D - удалить) >> ");
			            c = getch();
			
			            switch (c) {
			                case 'E':
			                case 'e':
			                case 'У':
			                case 'у':
			                    L = edit_elem(L, R, selectedElementData);
			                    break;
			                case 'D':
			                case 'd':
			                case 'В':
			                case 'в':
			                    L = del_elem(L, R, selectedElementData.index);
			                    break;
			                default:
			                    puts("\n Неверный выбор.");
			            }
			        } else {
			            puts("\n Элемент с указанным индексом не найден.");
			        }
			    } else {
			        puts("\n Элемент не существует.");
			    }
				return L;
			}
		}
	return L;
}

void endBranchStock(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return;
	}
    struct bidir_list *t = L;
    int factoryNum;
	factoryNum = safeIntInput("\n Введите номер завода >> ");
    if(R->inf.factoryNum < factoryNum) {
		printf("\n Такого завода нет.");
		return;
	}
	
	FILE *f = fopen("endbranchstock.txt", "ab+");
	if(!f) {
		f = fopen("endbranchstock.txt", "wb+");
	} else if (!f) {
        puts(" Не получается открыть (создать) файл.\n");
        return;
    }
	
    puts("  _____________________________________");
	puts(" | Завод N | Филиал N | Сумм. Ценности |");
	fprintf(f, "  _____________________________________\n");
    fprintf(f, " | Завод N | Филиал N | Сумм. Ценности |\n");
    while(t != NULL) {
	    if(t->inf.factoryNum == factoryNum) {
			puts(" |---------|----------|----------------|");
			fprintf(f, " |---------|----------|----------------|\n");
	        float endStock = ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
	        printf(" | %-7d | %-8d | %-14.2f |\n", t->inf.factoryNum,
				t->inf.branchNum, endStock);
			fprintf(f, " | %-7d | %-8d | %-14.2f |\n", t->inf.factoryNum,
				t->inf.branchNum, endStock);
		}
		t = t->nextr;
	}
	puts(" |_________|__________|________________|");
	fprintf(f, " |_________|__________|________________|\n");

	fclose(f);
	puts("\n Данные загружены в текстовый файл.");
	return;
}

void endFactoryStock(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return;
	}
    int factoryNum;
    float endFactoryStock = 0;
    struct bidir_list *t = L;
	factoryNum = safeIntInput("\n Введите номер завода >> ");
    if(R->inf.factoryNum < factoryNum) {
		printf("\n Такого завода нет.");
		return;
	}
	
	
	FILE *f = fopen("endfactorystock.txt", "ab+");
	if(!f) {
		f = fopen("endfactorystock.txt", "wb+");	
	} else if (!f) {
        puts(" Не получается открыть (создать) файл.\n");
        return;
    }
	
	puts("  __________________________");
	puts(" | Завод N | Сумм. Ценности |");
	fprintf(f, "  __________________________\n");
	fprintf(f, " | Завод N | Сумм. Ценности |\n");
    while (t != NULL) {
        if(t->inf.factoryNum == factoryNum) {
            endFactoryStock += ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
        }
        t = t->nextr;
    }
    puts(" |---------|----------------|");
    fprintf(f, " |---------|----------------|\n");
    printf(" | %-7d | %-14.2f |\n", factoryNum, endFactoryStock);
    fprintf(f, " | %-7d | %-14.2f |\n", factoryNum, endFactoryStock);
    puts(" |_________|________________|");
    fprintf(f, " |_________|________________|\n");
    
	fclose(f);
	puts("\n Данные загружены в текстовый файл.");
	return;
}

void endReport(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return;
	}
    int factoryNum;
    float initialStockTotal = 0;
    float receivedStockTotal = 0;
    float disposedStockTotal = 0;
    float endStockTotal = 0;

    struct bidir_list *t = L;
    factoryNum = safeIntInput("\n Введите номер завода >> ");
    if(R->inf.factoryNum < factoryNum) {
    	printf("\n Такого завода нет.");
    	return;
	}
	
	FILE *f = fopen("endstockreport.txt", "ab+");
	if(!f) {
		f = fopen("endstockreport.txt", "wb+");
	} else if (!f) {
        puts(" Не получается открыть (создать) файл.\n");
        return;
    }
	
    while (t != NULL) {
        if(t->inf.factoryNum == factoryNum) {
            initialStockTotal += t->inf.initialStock;
            receivedStockTotal += t->inf.receivedStock;
            disposedStockTotal += t->inf.disposedStock;
            endStockTotal += ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
        }
        t = t->nextr;
    }
	puts("  ____________________________________________________________________________");
	puts(" | Завод N | Нач. Ценности | Получ. Ценности | Выб. Ценности | Сумм. Ценности |");
	puts(" |---------|---------------|-----------------|---------------|----------------|");
	printf(" | %-7d | %-13.2f | %-15.2f | %-13.2f | %-14.2f |\n", factoryNum, initialStockTotal, 
		receivedStockTotal, disposedStockTotal, endStockTotal);
	puts(" |_________|_______________|_________________|_______________|________________|");
	
	fprintf(f, "  ____________________________________________________________________________\n'");
	fprintf(f, " | Завод N | Нач. Ценности | Получ. Ценности | Выб. Ценности | Сумм. Ценности |\n");
	fprintf(f, " |---------|---------------|-----------------|---------------|----------------|\n");
	fprintf(f, " | %-7d | %-13.2f | %-15.2f | %-13.2f | %-14.2f |\n", factoryNum, initialStockTotal, 
		receivedStockTotal, disposedStockTotal, endStockTotal);	
	fprintf(f, " |_________|_______________|_________________|_______________|________________|\n");
	
	fclose(f);
	puts("\n Данные загружены в текстовый файл.");
	return;	
}

struct bidir_list *edit_elem(struct bidir_list *L, struct bidir_list *R, struct MatVal selectedElementData) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return L;
	}
	struct MatVal t = selectedElementData;
	struct bidir_list *t1 = L;
	for(; t1 != NULL; t1 = t1->nextr) {
		if(t.index == t1->inf.index) {
			break;
		}
	}

	char Menu[][40] = { 
	    "Завод.", 
	    "Филиал.", 
	    "Ответственное лицо.", 
	    "Начальные ценности.", 
	    "Полученные ценности.",
	    "Выбывшие ценности.",
	    "Редактировать элемент целиком.",
	    "Удалить элемент.",
	    "Выход."
	};
	
	char ch;
	int active_menu = 0;
	int menu_size = sizeof(Menu) / sizeof(Menu[0]);
	
    while (1) {
		system("cls");
	    int x = 30, y = 10;
	    printf("\033[0;29m"); 
	    GoToXY(0,0);
		puts("  _________________________________________________________________________________________________");
		puts(" | n.n | Завод N | Филиал N | Ответственное Лицо | Нач. Ценности | Получ. Ценности | Выб. Ценности |");
		puts(" |-----|---------|----------|--------------------|---------------|-----------------|---------------|");
		printf(" | %-3d | %-7d | %-8d | %-18s | %-13.2f | %-15.2f | %-13.2f |\n", 
			t.index, t.factoryNum, t.branchNum, t.respPerson, 
			t.initialStock, t.receivedStock, t.disposedStock);
		puts(" |_____|_________|__________|____________________|_______________|_________________|_______________|");
		
		GoToXY(x, y);
		
	    for (int i = 0; i < menu_size; i++) {
	        if (i == active_menu) {
	            printf("\033[1;32m"); // для зеленого цвета текста
	        } else {
	            printf("\033[0;29m"); // для сброса цвета текста
	        }
	        GoToXY(x,y++);
	        printf("%s\n", Menu[i]);
	    }
	    
        ch = getch();
		switch (ch) {
		    case 27: //ESC
				return NULL;
		    case 72: // UP
		        if (active_menu > 0) {
		            active_menu--;
		        }
		        break;
		    case 80: // DOWN
		        if (active_menu < sizeof(Menu) / sizeof(Menu[0]) - 1) {
		            active_menu++;
		        }
		        break;
		    case 13: // ENTER
		    	printf("\033[0;29m"); 
				switch(active_menu) {
					case 0:
						t.factoryNum = safeIntInput("\n Введите номер завода >> ");
						break;
					case 1:
						t.branchNum = safeIntInput("\n Введите номер филиала >> ");
						break;
					case 2:
						printf("\n Введите фамилию ответственного лица >> ");
						scanf("%s", &t.respPerson);
						break;
					case 3:
						t.initialStock = safeFloatInput("\n Введите начальные ценности >> ");
						break;
					case 4:
						t.receivedStock = safeFloatInput("\n Введите полученные ценности >> ");
						break;
					case 5:
						t.disposedStock = safeFloatInput("\n Введите выбывшие ценности >> ");
						break;
					case 6:
						t.index = safeIntInput("\n Введите индекс элемента >> ");
						t.factoryNum = safeIntInput("\n Введите номер завода >> ");
						t.branchNum = safeIntInput("\n Введите номер филиала >> ");
						printf("\n Введите фамилию ответственного лица >> ");
						scanf("%s", &t.respPerson);
					    t.initialStock = safeFloatInput("\n Введите начальные ценности >> ");
						t.receivedStock = safeFloatInput("\n Введите полученные ценности >> ");
					    t.disposedStock = safeFloatInput("\n Введите выбывшие ценности >> ");
						break;
					case 7:
						del_elem(L, R, t.index);
						return L;
					case 8:
						return L;
				}
				t1->inf = t;
				puts("\n Данные записаны.");
				delay();
			}
		}
	return L;
}

void swap(struct MatVal *a, struct MatVal *b) {
	struct MatVal temp = *a;
	*a = *b;
	*b = temp;
}

struct bidir_list *sort_list(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return L;
	}
	
	char c;
	
	while(1) {
		system("cls");
		puts("\n 1. Относительно чего проводить сортировку?");
		puts("\n 1. Индекс");
		puts(" 2. Завод");
		puts(" 3. Филиал.");
		puts(" 4. Ответственное лицо.");
		puts(" 5. Начальные ценности.");
		puts(" 6. Полученные ценности.");
		puts(" 7. Выбывшие ценности.");
		puts(" 0. Выход.");
		puts("\n Введите число от 1 до 7, 0 - чтобы выйти");
		c = getch();

		if(c == '0') {
			return NULL;
		}
		
		if (c == '0' || (c  >= '1' && c  <= '7')) {
			break;
		}
	}
    	
	struct bidir_list *t1, *t2, *min;
	
	for(t1 = L; t1 != NULL; t1 = t1->nextr) {
		min = t1;
	
		for(t2 = t1->nextr; t2 != NULL; t2 = t2->nextr) {
			if(c == '1') {
				if(t2->inf.index < min->inf.index) {
					min = t2;
				}
			} else if (c == '2') {
				if(t2->inf.factoryNum < min->inf.factoryNum) {
					min = t2;
				}
			} else if (c == '3') {
				if(t2->inf.branchNum < min->inf.branchNum) {
					min = t2;
				}
			} else if (c == '4') {
				if(strcmp(t2->inf.respPerson, min->inf.respPerson) < 0) {
					min = t2;
				}
			} else if (c == '5') {
				if(t2->inf.initialStock < min->inf.initialStock) {
					min = t2;
				}
			} else if (c == '6') {
				if(t2->inf.receivedStock < min->inf.receivedStock) {
					min = t2;
				}
			} else if (c == '7') {
				if(t2->inf.disposedStock < min->inf.disposedStock) {
					min = t2;
				}
			}

		}
		
		if(min != t1) {
			swap(&t1->inf, &min->inf);
		}
	}
	return L;
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
		printf("\n Введите 1 чтобы продолжить, 0 - для выхода -> ");
		scanf("%d", &f1);
		} while (f1);
	return L;
}


void print_list(struct bidir_list *L, struct bidir_list *R) {
	if (L == NULL) {
		puts("\n Список пуст.");
		return;
	}
	
    struct bidir_list *t;
    int pageSize = 10; 
    int currentPage = 1; 
    int totalElements = 0;
	for(t = L; t != NULL; t = t->nextr) {
		totalElements++;
	}
    int startIndex = 0; 
    int endIndex = startIndex + pageSize - 1;
    char key;
    
    while (1) {
        system("cls");
        puts("  _________________________________________________________________________________________________");
        puts(" | n.n | Завод N | Филиал N | Ответственное Лицо | Нач. Ценности | Получ. Ценности | Выб. Ценности |");
        t = L;
        int index = 1;
        
        while (t != NULL) {
            if (index >= startIndex + 1 && index <= endIndex + 1) {
                puts(" |-----|---------|----------|--------------------|---------------|-----------------|---------------|");
                printf(" | %-3d | %-7d | %-8d | %-18s | %-13.2f | %-15.2f | %-13.2f |\n",
                    t->inf.index, t->inf.factoryNum, t->inf.branchNum, t->inf.respPerson,
                    t->inf.initialStock, t->inf.receivedStock, t->inf.disposedStock);
            }
            t = t->nextr;
            index++;
        }
        puts(" |_____|_________|__________|____________________|_______________|_________________|_______________|");

        printf(" Страница %d из %d (ниже - v, выше - ^, выход - ESC)\n", currentPage, (totalElements + pageSize) / pageSize);

        key = getch();

        switch (key) {
			case 80:
                if (endIndex < totalElements - 1) {
                    startIndex += pageSize;
                    endIndex = startIndex + pageSize - 1;
                    currentPage++;
                }
                break;
            case 72:
                if (startIndex > 0) {
                    startIndex -= pageSize;
                    endIndex = startIndex + pageSize - 1;
                    currentPage--;
                }
                break;
			case 27:
            	return;
        }
    }
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
			puts("\n Такой элемент уже существует.");
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
		puts("\n Список пуст.");
		return L;
	}
	
    struct bidir_list *t = L;

    while (t != NULL) {
        if (t->inf.index == num) {
            if (t == L) {
                L = t->nextr;
                if (L != NULL) {
                    L->nextl = NULL;
                }
            } else if (t == R) {
                R = t->nextl;
                if (R != NULL) {
                    R->nextr = NULL;
                }
            } else {
                struct bidir_list *t1 = t->nextl;
                t1->nextr = t->nextr;
                t->nextr->nextl = t1;
            }

            free(t);
            puts("\n Удаление прошло успешно.");
            return L;
        }

        t = t->nextr;
    }
	puts("\n Такого элемента нет.");
	return L;
}

struct bidir_list *load_data(struct bidir_list *L, struct bidir_list *R) {
	int d;
	char filename[30];
	char input[30];
	while(1) {
		d = safeIntInput("\n Введите 1, чтобы загрузить данные из текстового файла, 0 - из бинарного >> ");
		system("cls");
        
		if(d == 0) {
			while(1) {
				printf("\n Введите имя файла >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n Ошибка ввода. Имя файла не должно соедржать следующие символы: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".dat");
			
			FILE *f = fopen(filename, "rb+");
			if (!f) {
				puts("\n Не удалось найти файл.");
				return R;
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
			puts("\n Данные загружены из бинарного файл.");
			return R;
		
		} else if (d == 1) {
			while(1) {
				printf("\n Введите имя файла >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n Ошибка ввода. Имя файла не должно соедржать следующие символы: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".txt");
			
			FILE *f = fopen(filename, "rb+");
		    if (!f) {
		        puts(" Не удалось найти файл.\n");
		        return R;
		    }
		    
		    struct bidir_list *t1 = (struct bidir_list *)malloc(sizeof(struct bidir_list)); 
		    
		    while (!feof(f)) {
		        struct bidir_list *t2 = (struct bidir_list *)malloc(sizeof(struct bidir_list));
		        if (fscanf(f, "%d %d %d %s %f %f %f\n", &t1->inf.index, &t1->inf.factoryNum, &t1->inf.branchNum, &t1->inf.respPerson, 
					&t1->inf.initialStock, &t1->inf.receivedStock, &t1->inf.disposedStock) == 7) {
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
			}
			free(t1);
		    fclose(f);
		    puts("\n Данные загружены из текстового файл.");
		    return R;
		} else {
			puts("\n Ошибка ввода, попробуйте ещё раз.");
		}
	}
}

int save_data(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n Список пуст.");
		return 1;
	}
	
	int d;
	char filename[30];
	char input[30];
	while(1) {
		d = safeIntInput("\n Введите 1, чтобы сохранить данные в текстовый файл, 0 - в бинарный >> ");
		system("cls");
		
		if(d == 0) {
			while(1) {
				printf("\n Введите имя файла >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n Ошибка ввода. Имя файла не должно соедржать следующие символы: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".dat");
			
			FILE *f = fopen(filename, "wb+");
			if (!f) { 
				puts("\n Не получается открыть (создать) файл.");
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
			puts("\n Данные загружены в бинарный файла.");
			return 0;
		} else if (d == 1) {
			while(1) {
				printf("\n Введите имя файла >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n Ошибка ввода. Имя файла не должно соедржать следующие символы: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".txt");
			
			FILE *f = fopen(filename, "wb+");
		    if (!f) {
		        puts(" Не получается открыть (создать) файл.\n");
		        return 1;
		    }
		    struct bidir_list *t;
		    t = L;
		    
		    while (t != NULL) {
		        fprintf(f, "%d %d %d %s %.2f %.2f %.2f\n", t->inf.index, t->inf.factoryNum, t->inf.branchNum, t->inf.respPerson, 
					t->inf.initialStock, t->inf.receivedStock, t->inf.disposedStock);
		        t = t->nextr;
	    	}
	    	fclose(f);
	    	puts("\n Данные загружены в текстовый файл.");
	    	return 0;
		} else {
			puts("\n Ошибка ввода, попробуйте ещё раз.");
		}
	}
}

struct bidir_list *free_data(struct bidir_list *L, struct bidir_list *R) {
	struct bidir_list *t;
	while(L != NULL) {
		if (L == NULL) {
			puts("\n Список уже пуст.");
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