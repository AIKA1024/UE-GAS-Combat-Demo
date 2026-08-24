#include "Widgets/ActorWidget.h"


AActor* UActorWidget::GetOwnerActor() const
{
	return OwnerActor.Get();
}

void UActorWidget::SetOwnerActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
}

void UActorWidget::InitializeFromOwner()
{
	
}
