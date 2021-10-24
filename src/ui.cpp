#include "ui.h"

void AddUIEntity(ComponentList *cList, int inX, int inY){
    int entityID = NUM_ENTITIES;
    NUM_ENTITIES += 1;

    cout << "EntityID: " << entityID << "\n";

    cList->num_position_comp += 1; 
    cList->position_comp[entityID].ID = entityID;
    cList->position_comp[entityID].x = inX;
    cList->position_comp[entityID].y = inY;

}

void ReadUIEntity(ComponentList *cList)
{
    for(int i = 0; i < cList->num_position_comp; i ++)
    {
        cout << "The entity: " << cList->position_comp[i].ID << " has x: " << cList->position_comp[i].x << "\n";
    }
}