#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    int n;
    int chance_rebra;
    int chance_petli;

    printf("Введите количество вершин графа: ");
    scanf("%d", &n);

    printf("Введите вероятность ребра между вершинами (0-100%%): ");
    scanf("%d", &chance_rebra);
    if (chance_rebra > 100) {
        printf("Максимальная вероятность 100%%. Выставлено 100%%.\n");
        chance_rebra = 100;
    }
    else if (chance_rebra < 0) {
        printf("Минимальная вероятность 0%%. Выставлено 0%%.\n");
        chance_rebra = 0;
    }

    printf("Введите вероятность петли у вершины (0-100%%): ");
    scanf("%d", &chance_petli);
    if (chance_petli > 100) {
        printf("Максимальная вероятность 100%%. Выставлено 100%%.\n");
        chance_petli = 100;
    }
    else if (chance_petli < 0) {
        printf("Минимальная вероятность 0%%. Выставлено 0%%.\n");
        chance_petli = 0;
    }

    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                matrix[i][j] = (rand() % 100 < chance_petli) ? 1 : 0;
            }
            else {
                matrix[i][j] = (rand() % 100 < chance_rebra) ? 1 : 0;
                matrix[j][i] = matrix[i][j];
            }
        }
    }

    printf("\nМатрица смежности:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    int rebra = 0;
    int petli = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                if (matrix[i][j] == 1) {
                    rebra++;
                    petli++;
                }
            }
            else {
                if (matrix[i][j] == 1) {
                    rebra++;
                }
            }
        }
    }

    printf("\nСтатистика графа:\n");
    printf("Всего ребер: %d\n", rebra);
    if (petli > 0) {
        printf("Из них петель: %d\n", petli);
    }

    if (rebra % 10 == 1 && rebra % 100 != 11) {
        printf("Размер графа: %d ребро\n", rebra);
    }
    else if (rebra % 10 >= 2 && rebra % 10 <= 4 && (rebra % 100 < 10 || rebra % 100 > 20)) {
        printf("Размер графа: %d ребра\n", rebra);
    }
    else {
        printf("Размер графа: %d рёбер\n", rebra);
    }

    printf("\nАнализ вершин по матрице смежности:\n");

    int* list = (int*)malloc(n * sizeof(int));
    int* izol = (int*)malloc(n * sizeof(int));
    int* dom = (int*)malloc(n * sizeof(int));
    int list_count = 0, izol_count = 0, dom_count = 0;

    for (int i = 0; i < n; i++) {
        int has_connections = 0;

        for (int j = 0; j < n; j++) {
            if (i != j && matrix[i][j] == 1) {
                has_connections = 1;
                break;
            }
        }

        int has_loop = (matrix[i][i] == 1);
        int stepen = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                if (i == j) stepen += 2;
                else stepen += 1;
            }
        }

        if (stepen == 1 && !has_loop) {
            list[list_count] = i + 1;
            list_count++;
        }

        if (!has_connections) {
            izol[izol_count] = i + 1;
            izol_count++;
        }

        int all_connected = 1;
        for (int j = 0; j < n; j++) {
            if (i != j && matrix[i][j] != 1) {
                all_connected = 0;
                break;
            }
        }
        if (all_connected) {
            dom[dom_count] = i + 1;
            dom_count++;
        }
    }

    printf("Концевые вершины (%d): ", list_count);
    if (list_count == 0) printf("нет");
    else for (int i = 0; i < list_count; i++) printf("%d ", list[i]);

    printf("\nИзолированные вершины (%d): ", izol_count);
    if (izol_count == 0) printf("нет");
    else for (int i = 0; i < izol_count; i++) printf("%d ", izol[i]);

    printf("\nДоминирующие вершины (%d): ", dom_count);
    if (izol_count > 0) printf("не могут быть при наличии изолированных");
    else if (dom_count == 0) printf("нет");
    else for (int i = 0; i < dom_count; i++) printf("%d ", dom[i]);

    int vsego_reber = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (matrix[i][j] == 1) {
                vsego_reber++;
            }
        }
    }

    int** incidency = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        incidency[i] = (int*)malloc(vsego_reber * sizeof(int));
        for (int j = 0; j < vsego_reber; j++) {
            incidency[i][j] = 0;
        }
    }

    int index_rebra = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (matrix[i][j] == 1) {
                if (i == j) {
                    incidency[i][index_rebra] = 2;
                }
                else {
                    incidency[i][index_rebra] = 1;
                    incidency[j][index_rebra] = 1;
                }
                index_rebra++;
            }
        }
    }

    printf("\n\nМатрица инцидентности:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < vsego_reber; j++) {
            printf("%d ", incidency[i][j]);
        }
        printf("\n");
    }

    printf("\nАнализ вершин по матрице инцидентности:\n");

    int* list_inc = (int*)malloc(n * sizeof(int));
    int* izol_inc = (int*)malloc(n * sizeof(int));
    int* dom_inc = (int*)malloc(n * sizeof(int));
    int list_count_inc = 0, izol_count_inc = 0, dom_count_inc = 0;

    for (int i = 0; i < n; i++) {
        int has_connections_inc = 0;

        for (int j = 0; j < vsego_reber; j++) {
            if (incidency[i][j] == 1) {
                has_connections_inc = 1;
                break;
            }
        }

        int stepen = 0;
        for (int j = 0; j < vsego_reber; j++) {
            if (incidency[i][j] == 1) stepen += 1;
            else if (incidency[i][j] == 2) stepen += 2;
        }

        if (stepen == 1 && incidency[i][i] != 2) {
            list_inc[list_count_inc] = i + 1;
            list_count_inc++;
        }

        if (!has_connections_inc) {
            izol_inc[izol_count_inc] = i + 1;
            izol_count_inc++;
        }

        int all_connected_inc = 1;
        for (int k = 0; k < n; k++) {
            if (i != k) {
                int connected = 0;
                for (int j = 0; j < vsego_reber; j++) {
                    if (incidency[i][j] == 1 && incidency[k][j] == 1) {
                        connected = 1;
                        break;
                    }
                }
                if (!connected) {
                    all_connected_inc = 0;
                    break;
                }
            }
        }

        if (all_connected_inc) {
            dom_inc[dom_count_inc] = i + 1;
            dom_count_inc++;
        }
    }

    printf("Концевые вершины (%d): ", list_count_inc);
    if (list_count_inc == 0) printf("нет");
    else for (int i = 0; i < list_count_inc; i++) printf("%d ", list_inc[i]);

    printf("\nИзолированные вершины (%d): ", izol_count_inc);
    if (izol_count_inc == 0) printf("нет");
    else for (int i = 0; i < izol_count_inc; i++) printf("%d ", izol_inc[i]);

    printf("\nДоминирующие вершины (%d): ", dom_count_inc);
    if (izol_count_inc > 0) printf("не могут быть при наличии изолированных");
    else if (dom_count_inc == 0) printf("нет");
    else for (int i = 0; i < dom_count_inc; i++) printf("%d ", dom_inc[i]);

    printf("\n");

    free(list);
    free(izol);
    free(dom);
    free(list_inc);
    free(izol_inc);
    free(dom_inc);
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
        free(incidency[i]);
    }
    free(matrix);
    free(incidency);

    return 0;
}