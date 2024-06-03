#include "Patterns/Command_CollectItem.h"
#include "Managers/LuredToAdventureGameMode.h"

void UCommand_CollectItem::Init(UClass *Item, int32 Points, FVector Location)
{
    PickedItem = Item;
    EarnedPoints = Points;
    SpawnPoint = Location;
}
void UCommand_CollectItem::Undo()
{
    Super::Undo();

    if (!PickedItem) return;
    GetWorld()->SpawnActor(PickedItem,&SpawnPoint);
    GetWorld()->GetAuthGameMode<ALuredToAdventureGameMode>()->UpdateScore(-EarnedPoints);
}
