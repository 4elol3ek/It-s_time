#include <stdio.h>
#ifndef HEIGHT
#define HEIGHT 10
#endif
#ifndef LENGTH
#define LENGTH 10
#endif
// костыль
typedef struct field
{
    int arr[HEIGHT][LENGTH];
} field;

field makeNewField()
{
    field newField;
    for (int raw = 0; raw < HEIGHT; raw++)
    {
        for (int col = 0; col < LENGTH; col++)
        {
            newField.arr[raw][col] = 0;
        }
    }

    return newField;
}

// По  правилу B3/S23
field lifeSimulation(field f)
{
    field newField = makeNewField();
    for (int raw = 0; raw < HEIGHT; raw++)
    {
        for (int col = 0; col < LENGTH; col++)
        {
            if ((raw == 0) || (raw == HEIGHT - 1) || (col == 0) || (col == LENGTH - 1))
                newField.arr[raw][col] = 0;
            else
            {
                // находим сумму весов близлижащих клеток
                int sum = f.arr[raw][col + 1] + f.arr[raw][col - 1] + f.arr[raw - 1][col] + f.arr[raw + 1][col] + f.arr[raw - 1][col - 1] + f.arr[raw - 1][col + 1] + f.arr[raw + 1][col - 1] + f.arr[raw + 1][col + 1];
                // сверяем с правилом B3/S23
                if (sum == 3)
                    newField.arr[raw][col] = 1;
                if (sum == 2)
                    newField.arr[raw][col] = f.arr[raw][col];
                if ((sum > 3) || (sum < 2))
                    newField.arr[raw][col] = 0;
            }
        }
    }
    return newField;
}

// изменяем значение выбранной клетки на противоположное
field changeCellWeight(field field, int raw, int col)
{
    if (raw <= HEIGHT && raw >= 0 && col <= LENGTH && col >= 0)
    {
        if (field.arr[col][raw] == 1)
            field.arr[col][raw] = 0;
        else
            field.arr[col][raw] = 1;
    }
    return field;
}

void printField(field field)
{
    for (int raw = 0; raw < HEIGHT; raw++)
    {
        printf("|");
        for (int col = 0; col < LENGTH; col++)
        {
            printf("%d|", field.arr[raw][col]);
        }
        printf("\n");
    }
    printf("\n");
}

int lifeManage()
{
    field field = makeNewField();
    // printField(field);
    // char answer = 'c';
    // printf("Choose an action\nh-help\nc-continue\ns-set cell weight\nr-restart\ne-exit\n");
    // while (answer != 'e')
    // {
    //     scanf("%c", &answer);
    //     if (answer == 'h')
    //     {
    //         printf("Choose an action\nh-help\nc-м\ns-set cell weight\nr-restart\ne-exit\n");
    //         continue;
    //     }
    //     if (answer == 'c')
    //     {
    //         field = lifeSimulation(field);
    //         // printField(field);
    //         continue;
    //     }
    //     if (answer == 's')
    //     {
    //         int x;
    //         int y;
    //         printf("Enter the coordinate:");
    //         scanf("%d %d", &y, &x);
    //         field = changeCellWeight(field, x, y);
    //         printField(field);
    //         continue;
    //     }
    //     if (answer == 'r')
    //     {
    //         field = makeNewField();
    //         printField(field);
    //         continue;
    //     }
    //     if (answer == 'e')
    //         break;
    //     printf("ERROR unexpected symbol, try again\n");
    // }

    return 0;
}
