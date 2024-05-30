#include <src/lifeSimulation.h>
#include <stdio.h>

void isEqual(int expected, int result, int number)
{
    if(expected == result) printf("test%d: OK\n", number);
    else printf("test%d: FAIL\n", number);
}

//сравниваем посимвольно 2 поля
int isEqualField(field expected, field result)
{
    int flag = 1;
    for(int raw = 0; raw < 10; raw++)
    {
        for (int col = 0; col < 10; col++)
        {
            if(expected.arr[raw][col] != result.arr[raw][col])
            {
                flag = 0;
                break;
            }
        }
        if(!flag) break;
    }
    return flag;
}

void testChangeCellWeight(){
    //первый тест
    printf("changeCellWeight tests:\n");
    field result = makeNewField();
    int expected = 1;
    result = changeCellWeight(result, 1, 1);
    isEqual(expected, result.arr[1][1], 1);

    //второй тест
    expected = 0;
    result = changeCellWeight(result, 1 ,1);
    isEqual(expected, result.arr[1][1], 2);
}

void testLifeSimulation(){
    printf("lifeSimulation tests:\n");
    //проверяем случай, кода у клетки нет соседей
    field expected = makeNewField();
    field result = makeNewField();
    result = changeCellWeight(result, 2 , 2);
    result = lifeSimulation(result);
    isEqual(1,isEqualField(expected,result),1);

    //проверяем случай, когда у клетки один сосед
    expected = makeNewField();
    result = makeNewField();
    result = changeCellWeight(result, 2,2);
    result = changeCellWeight(result, 2,3);
    result = lifeSimulation(result);
    isEqual(1,isEqualField(expected,result),2);

    //проверяем случай, когда у клетки 2 соседа 
    //заодно проверяем простейший пульсар
    result = makeNewField();
    result = changeCellWeight(result, 2,2);
    result = changeCellWeight(result, 2,3);
    result = changeCellWeight(result,2,4);
    expected = result;
    result = lifeSimulation(result);
    result = lifeSimulation(result);
    isEqual(1,isEqualField(expected,result),3);
    
    //проверяем статичную фигуру
    //не знаю зачем...
    result = makeNewField();
    result = changeCellWeight(result, 2,2);
    result = changeCellWeight(result, 2,3);
    result = changeCellWeight(result,3,2);
    expected = result;
    expected= changeCellWeight(expected, 3,3);
    //клетки создают квадрат
    result = lifeSimulation(result);
    result = lifeSimulation(result);
    isEqual(1,isEqualField(expected,result),4);
}

int main(int argc, char const *argv[])
{
    testChangeCellWeight();
    testLifeSimulation();
    return 0;
}
