#include "ui.h"

void InitUIEntity(){
    NUM_UI_ENTITIES = 0;
}
void AddUIEntity(int input_x, int input_y){
    NUM_UI_ENTITIES += 1;

    cout << "num entiteis: " << NUM_UI_ENTITIES << "\n";
    UI_POS_Array[NUM_UI_ENTITIES].x = input_x; 
    UI_POS_Array[NUM_UI_ENTITIES].y = input_y; 

}