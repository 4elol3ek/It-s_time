#include <src/lifeSimulation.h>
#include <stdio.h>

void isEqual(int expected, int result, int number){
    if(expected == result) printf("test%d: OK\n", number);
    else printf("test%d: FAIL\n", number);
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

int main(int argc, char const *argv[])
{
    testChangeCellWeight();
    return 0;
}
