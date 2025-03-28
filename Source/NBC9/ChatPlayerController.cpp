// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatPlayerController.h"
#include "ChatUserWidget.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "NBC9.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ChatGameModeBase.h"
#include "Net/UnrealNetwork.h"

void AChatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsLocalController() == false)
	{
		return;
	}

	if (IsValid(ChatUserWidgetClass) == true)
	{
		ChatUserWidgetInstance = CreateWidget<UChatUserWidget>(this, ChatUserWidgetClass);
		if (IsValid(ChatUserWidgetInstance) == true)
		{
			ChatUserWidgetInstance->AddToViewport();
		}
	}
}

void AChatPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Green, 10.0f);
	ChatFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AChatPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	//PrintChatMessageString(InChatMessageString);
	if (IsLocalController() == true)
	{
		FString CombinedMessageString = GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

		ServerRPCPrintChatMessageString(CombinedMessageString);
	}
}

void AChatPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AChatPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AChatGameModeBase* ChatGM = Cast<AChatGameModeBase>(GM);
		if (IsValid(ChatGM) == true)
		{
			ChatGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

AChatPlayerController::AChatPlayerController()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void AChatPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString AChatPlayerController::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount+1) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}