// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget_AttributeSlot.h"
#include "Soul_Like_ACT.h"
#include "BPFL/BPFL_Utilities.h"
#include "Components/TextBlock.h"


void UWidget_AttributeSlot::Setup(FGameplayAttribute Attribute, UAbilitySystemComponent* ASC)
{
	_Attribute = Attribute;
	
	const auto AttributeName = FString::Printf(TEXT("%s :"), *Attribute.GetName());
	
	AttributeType->SetText(FText::FromString(AttributeName));

	if(ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Attribute)
			.AddUObject(this, &UWidget_AttributeSlot::UpdateAttributeValue);

		AttributeValue->SetText(FText::FromString(FString::SanitizeFloat(ASC->GetNumericAttribute(Attribute))));
	}
	else
	{
		AttributeValue->SetText(FText::FromString("-9999"));
	}
}

void UWidget_AttributeSlot::UpdateAttributeValue(const FOnAttributeChangeData& AttributeChangeData) const
{
	//Value to formatter
	const auto StringValue = UBPFL_Utilities::GetFormattedAttributeValue(_Attribute, AttributeChangeData.NewValue);
	AttributeValue->SetText(FText::FromString(StringValue));
}
