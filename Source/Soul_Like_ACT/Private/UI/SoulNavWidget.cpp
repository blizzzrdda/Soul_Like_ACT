// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SoulNavWidget.h"
#include "UINavPCComponent.h"
#include "UINavPCReceiver.h"
#include "UINavWidgetComponent.h"

UWidget* USoulNavWidget::GoToOnViewportWidget(UUINavWidget* NewWidget, bool bRemoveParent, bool bDestroyParent)
{
	if (NewWidget == nullptr) return nullptr;
	APlayerController* PC = Cast<APlayerController>(UINavPC->GetOwner());
	NewWidget->ParentWidget = this;
	NewWidget->bParentRemoved = bRemoveParent;
	NewWidget->bShouldDestroyParent = bDestroyParent;
	NewWidget->WidgetComp = WidgetComp;

	//Warning: remember to call this when the widget was create already
	UINavPC->SetActiveWidget(NewWidget);
	
	if (WidgetComp != nullptr)
	{
		WidgetComp->SetWidget(NewWidget);
	}
	else
	{
		NewWidget->SetUserFocus(PC);
		if (UINavPC->GetInputMode() == EInputMode::UI)
		{
			NewWidget->SetKeyboardFocus();
		}
	}
	CleanSetup();
	return NewWidget;
}

void USoulNavWidget::ReturnToParent(bool bRemoveAllParents, int ZOrder)
{
	if(bRemoveWhenReturn)
	{
		Super::ReturnToParent(bRemoveAllParents, ZOrder);
	}
	else
	{
		if (ParentWidget == nullptr)
		{
			if (bAllowRemoveIfRoot)
			{
				IUINavPCReceiver::Execute_OnRootWidgetRemoved(UINavPC->GetOwner());
				UINavPC->SetActiveWidget(nullptr);

				// if (WidgetComp != nullptr) WidgetComp->SetWidget(nullptr);
				// else RemoveFromParent();
			}
			return;
		}

		if (WidgetComp != nullptr)
		{
			if (bRemoveAllParents)
			{
				WidgetComp->SetWidget(nullptr);
			}
			else
			{
				if (bParentRemoved)
				{
					ParentWidget->ReturnedFromWidget = this;
				}
				else
				{
					UINavPC->SetActiveWidget(ParentWidget);
					ParentWidget->ReconfigureSetup();
				}
				WidgetComp->SetWidget(ParentWidget);
			}
		}
		else
		{
			if (bRemoveAllParents)
			{
				IUINavPCReceiver::Execute_OnRootWidgetRemoved(UINavPC->GetOwner());
				UINavPC->SetActiveWidget(nullptr);
				ParentWidget->RemoveAllParents();
				// RemoveFromParent();
				// Destruct();
			}
			else
			{
				//If parent was removed, add it to viewport
				if (bParentRemoved)
				{
					ParentWidget->ReturnedFromWidget = this;
					if (!bUsingSplitScreen) ParentWidget->AddToViewport(ZOrder);
					else ParentWidget->AddToPlayerScreen(ZOrder);
				}
				else
				{
					UINavPC->SetActiveWidget(ParentWidget);
					ParentWidget->ReconfigureSetup();
				}
				// RemoveFromParent();
			}
		}
	}
}
