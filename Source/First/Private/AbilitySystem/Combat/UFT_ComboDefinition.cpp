#include "AbilitySystem/Combat/UFT_ComboDefinition.h"

const FFT_ComboNode* UFT_ComboDefinition::FindNode(const FGameplayTag& NodeTag) const
{
	return Nodes.FindByPredicate([&NodeTag](const FFT_ComboNode& Node)
	{
		return Node.NodeTag == NodeTag;
	});
}
