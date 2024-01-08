// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyBase.h"
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"
#include "EnemyBase.h"
#include "HttpModule.h"
#include "Http.h"
#include "JsonUtilities.h"
#include "Interfaces/IHttpResponse.h"
#include "SelectionPointer.h"

// Sets default values
APartyBase::APartyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponents"));
}

int APartyBase::CalculateTotalPartyMP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundActors);
		for (AActor* party : FoundActors)
		{
			APartyBase* PartyInstance = Cast<APartyBase>(party);
			if (PartyInstance)
			{
				TotalPartyMp += PartyInstance->MP;
			}
		}
	}
	return TotalPartyMp;
}
float APartyBase::CalculateTotalPartyHP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundActors);
		for (AActor* party : FoundActors)
		{
			APartyBase* PartyInstance = Cast<APartyBase>(party);
			if (PartyInstance)
			{
				TotalPartyHp += PartyInstance->HP;
			}
		}
	}
	return TotalPartyHp;
}
void APartyBase::AttackEnemy()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundEnemies);
		if (FoundEnemies.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, FoundEnemies.Num() - 1);
			AEnemyBase* RandomEnemy = Cast<AEnemyBase>(FoundEnemies[RandomIndex]);
			if (RandomEnemy)
			{
				if (HeavyAttackFlag == true)
				{
					HeavyAttack();
					RandomEnemy->HP -= 50;
					RemainingEnemyHP = RandomEnemy->HP;
					SendRemainingHp();
				}
				else if (LightAttackFlag == true)
				{
					LightAttack();
					RandomEnemy->HP -= 40;
					RemainingEnemyHP = RandomEnemy->HP;
					SendRemainingHp();
				}
			}
		}
	}
}

void APartyBase::HealPlayer()
{

}

void APartyBase::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// Handle the response if needed
		UE_LOG(LogTemp, Error, TEXT("HTTP request Success!"));
		FString ResponseString = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("GPT-3 API Response: %s"), *ResponseString);

		// Parse and use the response data
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		//if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		//{
		//	const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
		//	if (JsonObject->TryGetArrayField("choices", ChoicesArray))
		//	{
		//		for (const TSharedPtr<FJsonValue>& Choice : *ChoicesArray)
		//		{
		//			const TSharedPtr<FJsonObject> ChoiceObject = Choice->AsObject();

		//			if (ChoiceObject.IsValid())
		//			{
		//				FString TextResponse;
		//				if (ChoiceObject->HasField("message"))
		//				{
		//					const TSharedPtr<FJsonObject> MessageObject = ChoiceObject->GetObjectField("message");

		//					if (MessageObject.IsValid() && MessageObject->TryGetStringField("content", TextResponse))
		//					{
		//						// Extract any numeric value from the 'content' field
		//						FString RemainingHPString;
		//						if (ExtractNumericValueFromString(TextResponse, RemainingHPString))
		//						{
		//							RemainingHP = FCString::Atof(*RemainingHPString);
		//							UE_LOG(LogTemp, Warning, TEXT("Remaining HP: %.2f"), RemainingHP);
		//						}
		//						else
		//						{
		//							UE_LOG(LogTemp, Warning, TEXT("Failed to extract Remaining HP from 'content' field."));
		//						}
		//					}
		//					else
		//					{
		//						UE_LOG(LogTemp, Warning, TEXT("Failed to extract 'content' field from 'message'."));
		//					}
		//				}
		//				else
		//				{
		//					UE_LOG(LogTemp, Warning, TEXT("ChoiceObject does not have a 'message' field."));
		//				}
		//			}
		//			else
		//			{
		//				UE_LOG(LogTemp, Warning, TEXT("ChoiceObject is not valid"));
		//			}
		//		}
		//	}
		//}
		/*else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
		}*/
	}
	else
	{
		// Handle errors
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed!"));
	}
}
void APartyBase::SendRemainingHp()
{
	FString ChatGPTUrl = TEXT("https://api.openai.com/v1/chat/completions");
	FString ApiKey = TEXT("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");  // Replace with your actual API key

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ChatGPTUrl);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));
	HttpRequest->SetVerb(TEXT("POST"));

	// Create a JSON object with RemainingHP
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField(TEXT("RemainingHP"), RemainingEnemyHP);

	// Set the request payload
	FString JsonPayload = FString::Printf(TEXT("{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"system\", \"content\": \"You are a helpful assistant.\"}, {\"role\": \"system\", \"content\": \"Remaining HP: %.2f\"},{\"role\": \"system\", \"content\": \"Always state the hp.\"}]}"), RemainingEnemyHP);
	HttpRequest->SetContentAsString(JsonPayload);

	// Bind the callback function
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &APartyBase::OnHttpRequestComplete);

	// Process the request
	HttpRequest->ProcessRequest();
}
void APartyBase::HeavyAttack()
{
	UE_LOG(LogTemp, Error, TEXT("HeavyAttack."));
	HeavyAttackFlag = false;
}
void APartyBase::LightAttack()
{
	UE_LOG(LogTemp, Error, TEXT("LightAttack."));
	LightAttackFlag = false;
}
// Called when the game starts or when spawned
void APartyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APartyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APartyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

