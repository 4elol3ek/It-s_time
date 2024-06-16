#define HEIGHT 10
#define LENGTH 10
typedef struct field
{
    int arr[HEIGHT][LENGTH];
} field;
field makeNewField();
field lifeSimulation(field field);
field changeCellWeight(field field, int raw, int col);
int lifeManage();
