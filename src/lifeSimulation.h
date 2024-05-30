typedef struct field
{
    int arr[10][10];
} field;
field makeNewField();
field lifeSimulation(field field);
field changeCellWeight(field field, int raw, int col);
int lifeManage();
