#include"lifeSimulation.h"
#include<stdio.h>

void printField(field field){
    for(int raw = 0; raw<10; raw++){
        for(int col = 0; col < 10; col++){
            printf("%d|", field.arr[raw][col]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(){
    field field = makeNewField(); 
    printField(field);
    char answer = 'c';
    printf("Choose an action\nh-help\nc-continue\ns-set cell weight\nr-restart\ne-exit\n");
    while(answer != 'e'){
        scanf("%c",&answer);
        if(answer == 'h'){
            printf("Choose an action\nh-help\nc-м\ns-set cell weight\nr-restart\ne-exit\n");
            continue;
        }
        if(answer == 'c'){
            field = lifeSimulation(field);
            printField(field);
            continue;
        }
        if(answer == 's'){
            int x;
            int y;
            printf("Enter the coordinate:");
            scanf("%d %d", &y, &x);
            field = changeCellWeight(field,x,y);
            printField(field);
            continue;
        }
        if(answer == 'r'){
            field = makeNewField();
            printField(field);
            continue;
        }
        if(answer == 'e') break;
        printf("ERROR unexpected symbol, try again\n");
    }


    return 0;
}