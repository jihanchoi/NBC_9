// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatUserWidget.h"
#include "ChatPlayerController.h"

void UChatUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_Chat->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_Chat->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}
void UChatUserWidget::NativeDestruct()
{
	if (EditableTextBox_Chat->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_Chat->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UChatUserWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AChatPlayerController* OwningChatPlayerController = Cast<AChatPlayerController>(OwningPlayerController);
			if (IsValid(OwningChatPlayerController) == true)
			{
				OwningChatPlayerController->SetChatMessageString(Text.ToString());
				EditableTextBox_Chat->SetText(FText());
			}
		}
	}
}