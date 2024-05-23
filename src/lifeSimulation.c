//костыль 
typedef struct field
{
    int arr[10][10];
}field;


field makeNewField(){
    field newField;
    for(int raw = 0; raw < 10; raw++){
        for(int col = 0; col < 10; col++){
            newField.arr[raw][col] = 0;
        }
    }

    return newField;
}

// По  правилу B3/S23
field lifeSimulation(field f){
    field newField = makeNewField();
    for(int raw = 0; raw < 10; raw++){
        for(int col = 0; col < 10; col++){
            if((raw == 0) || (raw == 9) || (col == 0) || (col == 9)) newField.arr[raw][col] = 0;
            else{
                //находим сумму весов близлижащих клеток 
                int sum = f.arr[raw][col+1] + f.arr[raw][col-1] 
                    + f.arr[raw-1][col] + f.arr[raw+1][col] 
                    + f.arr[raw-1][col-1]+ f.arr[raw-1][col+1] 
                    + f.arr[raw+1][col-1] + f.arr[raw+1][col+1];
                // сверяем с правилом B3/S23
                if(sum == 3) newField.arr[raw][col] = 1;
                if(sum == 2) newField.arr[raw][col] = f.arr[raw][col];
                if((sum > 3) || (sum < 2)) newField.arr[raw][col] = 0;
            }
        }
    }
    return newField;
}

//изменяем значение выбранной клетки на противоположное
field changeCellWeight(field field, int raw, int col){
    if(field.arr[raw][col] == 1) field.arr[raw][col] = 0;
    else field.arr[raw][col] = 1;

    return field;
}