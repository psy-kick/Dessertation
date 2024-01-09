// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PartyBase.h"
#include "HttpModule.h"
#include "Http.h"
#include "JsonUtilities.h"
#include "Interfaces/IHttpResponse.h"
#include <Kismet/GameplayStatics.h>

AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

int AEnemyBase::CalculateTotalEnemyMP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundActors);
		for (AActor* enemy : FoundActors)
		{
			AEnemyBase* EnemyInstance = Cast<AEnemyBase>(enemy);
			if (EnemyInstance)
			{
				TotalEnemyMp += EnemyInstance->MP;
			}
		}
	}
	return TotalEnemyMp;
}

float AEnemyBase::CalculateTotalEnemyHP()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, AEnemyBase::StaticClass(), FoundActors);
		for (AActor* party : FoundActors)
		{
			AEnemyBase* EnemyInstance = Cast<AEnemyBase>(party);
			if (EnemyInstance)
			{
				TotalEnemyHp += EnemyInstance->HP;
			}
		}
	}
	return TotalEnemyHp;
}

APartyBase* AEnemyBase::GetSelectedParty()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::GetAllActorsOfClass(World, APartyBase::StaticClass(), FoundPartyActors);
		if (FoundPartyActors.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, FoundPartyActors.Num() - 1);
			SlectedParty = Cast<APartyBase>(FoundPartyActors[RandomIndex]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World is not valid."));
	}
	return SlectedParty;
}

void AEnemyBase::EnemyAttack()
{
	if (GetSelectedParty())
	{
		RemainingPartyHP = GetSelectedParty()->HP;
		SendRemainingHp();
		Attacking = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetSelectedParty() is not valid."));
	}
}

void AEnemyBase::HeavyAttack()
{
	if (Attacking == true)
	{
		UE_LOG(LogTemp, Error, TEXT("HeavyEnemyAttack."));
		GetSelectedParty()->HP -= 50;
		RemainingPartyHP = GetSelectedParty()->HP;
		Attacking = false;
	}
}

void AEnemyBase::LightAttack()
{
	if (Attacking == true)
	{
		UE_LOG(LogTemp, Error, TEXT("LightEnemyAttack."));
		GetSelectedParty()->HP -= 40;
		RemainingPartyHP = GetSelectedParty()->HP;
		Attacking = false;
	}
}

void AEnemyBase::SendRemainingHp()
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
	JsonObject->SetNumberField(TEXT("RemainingHP"), RemainingPartyHP);

	// Set the request payload
	FString JsonPayload = FString::Printf(TEXT("{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"system\", \"content\": \"You are a helpful assistant.\"}, {\"role\": \"system\", \"content\": \"Remaining HP: %.2f\"},{\"role\": \"system\", \"content\": \"Always state the hp.\"}, {\"role\": \"user\", \"content\": \"Alaways do a heavy attack and state you did an heavy attack.\"}]}"), RemainingPartyHP);
	HttpRequest->SetContentAsString(JsonPayload);

	// Bind the callback function
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AEnemyBase::OnHttpRequestComplete);

	// Process the request
	HttpRequest->ProcessRequest();
}

void AEnemyBase::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
			if (JsonObject->TryGetArrayField("choices", ChoicesArray))
			{
				for (const TSharedPtr<FJsonValue>& Choice : *ChoicesArray)
				{
					const TSharedPtr<FJsonObject> ChoiceObject = Choice->AsObject();

					if (ChoiceObject.IsValid())
					{
						FString TextResponse;
						if (ChoiceObject->HasField("message"))
						{
							const TSharedPtr<FJsonObject> MessageObject = ChoiceObject->GetObjectField("message");

							if (MessageObject.IsValid() && MessageObject->TryGetStringField("content", TextResponse))
							{
								if (TextResponse.Contains("light attack"))
								{
									Attacking = true;
									LightAttack();
								}
								else if (TextResponse.Contains("heavy attack"))
								{
									Attacking = true;
									HeavyAttack();
								}
								else
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed To Decide."));
								}
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Failed to extract 'content' field from 'message'."));
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("ChoiceObject does not have a 'message' field."));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("ChoiceObject is not valid"));
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
		}
	}
	else
	{
		// Handle errors
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed!"));
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
