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
            printf("\n ������ �����. ������� �����.\n");
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
            printf("\n ������ �����. ������� ������� �����.\n");
        }
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
void delay() {
	puts("\n ������� ����� �������, ����� ����������...");
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
	    puts("\n 1. ����������� ������.");
	    puts(" 2. ������� ������.");
	    puts(" 3. ����� ��������.");
	    puts(" 4. ���������� ������.");
	    puts(" 5. ����������� ��������� �� ���. �������� �� �������.");
	    puts(" 6. ����������� ��������� �� ���. �������� �� ������.");
	    puts(" 7. ������ ��������� �� ����� ������.");
	    puts(" 8. ��������� � ����.");
	    puts(" 9. ��������� �� �����.");
	    puts(" 0. �����.");
	    puts("\n ������� ����� �� 1 �� 9, 0 - ��� ������");
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
//				num = safeIntInput("\n ������� ������ ��������� >> ");
//				L = del_elem(L, R, num);
//				if (L == NULL) {
//					R = L;
//				}
//				t = L;
//				for(; t != NULL; t = t->nextr) {
//					R = t;
//				}
			    L = search_elem(L, R);
			    t = L;
				for(; t != NULL; t = t->nextr) {
					R = t;
				}
				break;
			case '4':
				sort_list(L, R);
				t = L;
				for(; t != NULL; t = t->nextr) {
					R = t;
				}
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
			case '0':
				L = free_data(L, R);
				R = L;
				puts("\n ������ ������.");
				return 0;
		}
		delay();
	}
}

struct MatVal read_data() {
	struct MatVal data;
	char input[10]; 
	int c = 0;
	data.index = safeIntInput("\n ������� ������ �������� >> ");
	data.factoryNum = safeIntInput("\n ������� ����� ������ >> ");
	data.branchNum = safeIntInput("\n ������� ����� ������� >> ");
	printf("\n ������� ������� �������������� ���� >> ");
	scanf("%s", &data.respPerson);
    data.initialStock = safeFloatInput("\n ������� ��������� �������� >> ");
	data.receivedStock = safeFloatInput("\n ������� ���������� �������� >> ");
    data.disposedStock = safeFloatInput("\n ������� �������� �������� >> ");
	puts("\n ������ ��������.");
	return data;
}

struct bidir_list *search_elem(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
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
	
	while(1) {
	    system("cls");
		puts("\n 1. ������");
		puts(" 2. �����");
		puts(" 3. ������.");
		puts(" 4. ������������� ����.");
		puts(" 5. ��������� ��������.");
		puts(" 6. ���������� ��������.");
		puts(" 7. �������� ��������.");
		puts(" 0. �����.");
		puts("\n ������� ����� �� 1 �� 7, 0 - ����� �����");
		printf("\n �������� ���� ��� ������ >> ");
		field = getch();
		if(field == '0') {
			return NULL;
		}
		
		if (field == '0' || (field >= '1' && field <= '7')) {
        	break;
    	}
	}
	puts("");
    printf("\n ������� �������� ��� ������ >> ");
    scanf("%s", value);
	
    while (t != NULL) {
        if ((field == '4') && strstr(t->inf.respPerson, value) != NULL) {
            foundL = add_sort(foundL, foundR, t->inf);
            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
            	foundR = t2;
			}
        } else if (field == '1') {
            intValue = t->inf.index;
            sprintf(stringValue, "%d", intValue);
            if (strstr(stringValue, value) != NULL) {
	            foundL = add_sort(foundL, foundR, t->inf);
	            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
	            	foundR = t2;
				}
            }
        } else if (field == '2') {
            intValue = t->inf.factoryNum;
            sprintf(stringValue, "%d", intValue);
            if (strstr(stringValue, value) != NULL) {
                foundL = add_sort(foundL, foundR, t->inf);
	            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
	            	foundR = t2;
				}
            }
        } else if (field == '3') {
            intValue = t->inf.branchNum;
            sprintf(stringValue, "%d", intValue);
            if (strstr(stringValue, value) != NULL) {
	            foundL = add_sort(foundL, foundR, t->inf);
	            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
	            	foundR = t2;
				}
            }
        } else if (field == '5') {
            floatValue = t->inf.initialStock;
            sprintf(stringValue, "%.2f", floatValue);
            if (strstr(stringValue, value) != NULL) {
	            foundL = add_sort(foundL, foundR, t->inf);
	            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
	            	foundR = t2;
				}
            }
        } else if (field == '6') {
            floatValue = t->inf.receivedStock;
            sprintf(stringValue, "%.2f", floatValue);
            if (strstr(stringValue, value) != NULL) {
	            foundL = add_sort(foundL, foundR, t->inf);
	            for(t2 = foundL; t2 != NULL; t2 = t2->nextr) {
	            	foundR = t2;
				}
            }
        } else if (field == '7') {
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

        selectedIndex = safeIntInput("\n �������� ������ �������� ��� ������ (0 - ����� ���������) >> ");;
		if(selectedIndex == 0) {
			return NULL;
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
            puts("\n �������� �������� (E - �������������, D - �������) >> ");
            c = getch();

            switch (c) {
                case 'E':
                case 'e':
                case '�':
                case '�':
                    L = edit_elem(L, R, selectedElementData);
                    puts("\n edit_elem.");
                    break;
                case 'D':
                case 'd':
                case '�':
                case '�':
                    L = del_elem(L, R, selectedElementData.index);
                    puts("\n del_elem.");
                    break;
                default:
                    puts("\n �������� �����.");
            }
        } else {
            puts("\n ������� � ��������� �������� �� ������.");
        }
    } else {
        puts("\n ������� �� ����������.");
    }
    
    return L;
}

void endBranchStock(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return;
	}
    struct bidir_list *t = L;
    int factoryNum;
	factoryNum = safeIntInput("\n ������� ����� ������ >> ");
    if(R->inf.factoryNum < factoryNum) {
		printf("\n ������ ������ ���.");
		return;
	}
    puts("  _____________________________________");
	puts(" | ����� N | ������ N | ����. �������� |");
    while(t != NULL) {
	    if(t->inf.factoryNum == factoryNum) {
			puts(" |---------|----------|----------------|");
	        float endStock = ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
	        printf(" | %-7d | %-8d | %-14.2f |\n", t->inf.factoryNum,
				t->inf.branchNum, endStock);
		}
		t = t->nextr;
	}
	puts(" |_________|__________|________________|");
}

void endFactoryStock(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return;
	}
    int factoryNum;
    float endFactoryStock = 0;
    struct bidir_list *t = L;
	factoryNum = safeIntInput("\n ������� ����� ������ >> ");
    if(R->inf.factoryNum < factoryNum) {
		printf("\n ������ ������ ���.");
		return;
	}
	puts("  __________________________");
	puts(" | ����� N | ����. �������� |");
    while (t != NULL) {
        if(t->inf.factoryNum == factoryNum) {
            endFactoryStock += ((t->inf.receivedStock + t->inf.initialStock) - t->inf.disposedStock);
        }
        t = t->nextr;
    }
    puts(" |---------|----------------|");
    printf(" | %-7d | %-14.2f |\n", factoryNum, endFactoryStock);
    puts(" |_________|________________|");
}

void endReport(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return;
	}
    int factoryNum;
    float initialStockTotal = 0;
    float receivedStockTotal = 0;
    float disposedStockTotal = 0;
    float endStockTotal = 0;

    struct bidir_list *t = L;
    factoryNum = safeIntInput("\n ������� ����� ������ >> ");
    if(R->inf.factoryNum < factoryNum) {
    	printf("\n ������ ������ ���.");
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
	puts(" | ����� N | ���. �������� | �����. �������� | ���. �������� | ����. �������� |");
	puts(" |---------|---------------|-----------------|---------------|----------------|");
	printf(" | %-7d | %-13.2f | %-15.2f | %-13.2f | %-14.2f |\n", factoryNum, initialStockTotal, 
		receivedStockTotal, disposedStockTotal, endStockTotal);
	puts(" |_________|_______________|_________________|_______________|________________|");
}

struct bidir_list *edit_elem(struct bidir_list *L, struct bidir_list *R, struct MatVal selectedElementData) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return L;
	}
	struct MatVal t = selectedElementData;
	struct bidir_list *t1 = L;
	for(; t1 != NULL; t1 = t1->nextr) {
		if(t.index == t1->inf.index) {
			break;
		}
	}
//	char data[len];
//	system("cls");
//	int num = safeIntInput("\n ������� ������ �������� >> ");
//	if ((R->inf.index < num) || (num == 0)) {
//		puts("\n ������ �������� ���.");
//		return L;
//	}
//	while(t->inf.index != num) {
//		t = t->nextr;
//	}
	char c;
	while(1) {
		system("cls");
		puts("  _________________________________________________________________________________________________");
		puts(" | n.n | ����� N | ������ N | ������������� ���� | ���. �������� | �����. �������� | ���. �������� |");
		puts(" |-----|---------|----------|--------------------|---------------|-----------------|---------------|");
		printf(" | %-3d | %-7d | %-8d | %-18s | %-13.2f | %-15.2f | %-13.2f |\n", 
			t.index, t.factoryNum, t.branchNum, t.respPerson, 
			t.initialStock, t.receivedStock, t.disposedStock);
		puts(" |_____|_________|__________|____________________|_______________|_________________|_______________|");
		puts("\n 1. �����");
		puts(" 2. ������.");
		puts(" 3. ������������� ����.");
		puts(" 4. ��������� ��������.");
		puts(" 5. ���������� ��������.");
		puts(" 6. �������� ��������.");
		puts(" 7. ������������� ������� �������.");
		puts(" 8. ������� �������.");
		puts(" 0. �����.");
		puts("\n ������� ����� �� 1 �� 7, 0 - ����� �����");
		c = getch();
		switch(c) {
			case '1':
				t.factoryNum = safeIntInput("\n ������� ����� ������ >> ");
				break;
			case '2':
				t.branchNum = safeIntInput("\n ������� ����� ������� >> ");
				break;
			case '3':
				printf("\n ������� ������� �������������� ���� >> ");
				scanf("%s", &t.respPerson);
				break;
			case '4':
				t.initialStock = safeFloatInput("\n ������� ��������� �������� >> ");
				break;
			case '5':
				t.receivedStock = safeFloatInput("\n ������� ���������� �������� >> ");
				break;
			case '6':
				t.disposedStock = safeFloatInput("\n ������� �������� �������� >> ");
				break;
			case '7':
				t.index = safeIntInput("\n ������� ������ �������� >> ");
				t.factoryNum = safeIntInput("\n ������� ����� ������ >> ");
				t.branchNum = safeIntInput("\n ������� ����� ������� >> ");
				printf("\n ������� ������� �������������� ���� >> ");
				scanf("%s", &t.respPerson);
			    t.initialStock = safeFloatInput("\n ������� ��������� �������� >> ");
				t.receivedStock = safeFloatInput("\n ������� ���������� �������� >> ");
			    t.disposedStock = safeFloatInput("\n ������� �������� �������� >> ");
				break;
			case '8':
				del_elem(L, R, t.index);
				return L;
			case '0':
				return L;
		}
		t1->inf = t;
		puts("\n ������ ��������.");
		delay();
	}
}

void swap(struct MatVal *a, struct MatVal *b) {
	struct MatVal temp = *a;
	*a = *b;
	*b = temp;
}

struct bidir_list *sort_list(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return L;
	}
	
	struct bidir_list *t1, *t2, *min;
	
	for(t1 = L; t1 != NULL; t1 = t1->nextr) {
		min = t1;
	
		for(t2 = t1->nextr; t2 != NULL; t2 = t2->nextr) {
			if(strcmp(t2->inf.respPerson, min->inf.respPerson) < 0) {
				min = t2;
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
		printf("\n ������� 1 ����� ����������, 0 - ��� ������ -> ");
		scanf("%d", &f1);
		} while (f1);
	return L;
}


void print_list(struct bidir_list *L, struct bidir_list *R) {
	if (L == NULL) {
		puts("\n ������ ����.");
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
        puts(" | n.n | ����� N | ������ N | ������������� ���� | ���. �������� | �����. �������� | ���. �������� |");
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

        printf(" �������� %d �� %d (���� - S, ���� - W, ����� - Q)\n", currentPage, (totalElements + pageSize) / pageSize);

        key = getch();

        switch (key) {
        	case '�':
        	case '�':
            case 's':
            case 'S':
                if (endIndex < totalElements - 1) {
                    startIndex += pageSize;
                    endIndex = startIndex + pageSize - 1;
                    currentPage++;
                }
                break;
            case '�':
            case '�':
            case 'w':
            case 'W':
                if (startIndex > 0) {
                    startIndex -= pageSize;
                    endIndex = startIndex + pageSize - 1;
                    currentPage--;
                }
                break;
            case '�':
            case '�':
            case 'q':
            case 'Q':
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
			puts("\n ����� ������� ��� ����������.");
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
		puts("\n ������ ����.");
		return L;
	}
	printf(" %d", num);
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
            puts("\n �������� ������ �������.");
            return L;
        }

        t = t->nextr;
    }
	puts("\n ������ �������� ���.");
	return L;
}

struct bidir_list *load_data(struct bidir_list *L, struct bidir_list *R) {
	int d;
	char filename[30];
	char input[30];
	while(1) {
		d = safeIntInput("\n ������� 1, ����� ��������� ������ �� ���������� �����, 0 - �� ��������� >> ");
		system("cls");
        
		if(d == 0) {
			while(1) {
				printf("\n ������� ��� ����� >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n ������ �����. ��� ����� �� ������ ��������� ��������� �������: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".dat");
			
			FILE *f = fopen(filename, "rb+");
			if (!f) {
				puts("\n �� ������� ����� ����.");
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
			puts("\n ������ ��������� �� ��������� ����.");
			return R;
		
		} else if (d == 1) {
			while(1) {
				printf("\n ������� ��� ����� >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n ������ �����. ��� ����� �� ������ ��������� ��������� �������: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".txt");
			
			FILE *f = fopen(filename, "rb+");
		    if (!f) {
		        puts(" �� ������� ����� ����.\n");
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
		    puts("\n ������ ��������� �� ���������� ����.");
		    return R;
		} else {
			puts("\n ������ �����, ���������� ��� ���.");
		}
	}
}

int save_data(struct bidir_list *L, struct bidir_list *R) {
	if(L == NULL) {
		puts("\n ������ ����.");
		return 1;
	}
	
	int d;
	char filename[30];
	char input[30];
	while(1) {
		d = safeIntInput("\n ������� 1, ����� ��������� ������ � ��������� ����, 0 - � �������� >> ");
		system("cls");
		
		if(d == 0) {
			while(1) {
				printf("\n ������� ��� ����� >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n ������ �����. ��� ����� �� ������ ��������� ��������� �������: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".dat");
			
			FILE *f = fopen(filename, "wb+");
			if (!f) { 
				puts("\n �� ���������� ������� (�������) ����.");
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
			puts("\n ������ ��������� � �������� �����.");
			return 0;
		} else if (d == 1) {
			while(1) {
				printf("\n ������� ��� ����� >> ");
				scanf("%s", input);
				if (isItFilename(input)) {
					strcpy(filename, input);
					break;
				} else {
					printf("\n ������ �����. ��� ����� �� ������ ��������� ��������� �������: \\/:*?\"<>|\n");
				}
			}
			
			strcat(filename, ".txt");
			
			FILE *f = fopen(filename, "wb+");
		    if (!f) {
		        puts(" �� ���������� ������� (�������) ����.\n");
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
	    	puts("\n ������ ��������� � ��������� �����.");
	    	return 0;
		} else {
			puts("\n ������ �����, ���������� ��� ���.");
		}
	}
}

struct bidir_list *free_data(struct bidir_list *L, struct bidir_list *R) {
	struct bidir_list *t;
	while(L != NULL) {
		if (L == NULL) {
			puts("\n ������ ��� ����.");
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