// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "POPULCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Border.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundWave.h"
//#include "Containers/EnumAsByte.h"

#include "Engine.h"
#include "MainHUD.h"
#include "DefualtUserWidget.h"
#include "GameDataInstance.h"
#include "POPULGameMode.h"
#include "BaseActor.h"
#include "EnemyBaseCharacter.h"
#include "OKBoxUserWidget.h"
#include "PCAnimInstance.h"
#include "InventorySlotUserWidget.h"
#include "ItemDataSaveGame.h"

//////////////////////////////////////////////////////////////////////////
// APOPULCharacter

bool APOPULCharacter::bIsMoveLevel = false;
int APOPULCharacter::InvenMax = 0;

APOPULCharacter::APOPULCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//WeaponSocket
	//WeaponVisibility
	WeponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wepon"));
	WeponMesh->SetupAttachment((USceneComponent*)GetMesh(),FName(L"WeaponSocket"));
	WeponMesh->SetVisibility(true);
	m_bIsBattel = true;

	//GetMesh()->SetAnimInstanceClass(UPCAnimInstance::StaticClass());
	m_bIsActiveInventory = true;
	m_bIsJumStrat = false;
	OverlapActor = nullptr;
	m_bIsKeyDownQuitGame = false;
	AttackMove = false;
	m_bIstraceOn = false;
	m_bIsAdditionalOn = false;
	m_bIsAdditional_Input = false;
	m_bIsAttack = false;
	bIsCountVisibility = false;
	CountBoxUICameraRotation = FRotator(0.0f, 0.0f, 0.0f);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	static ConstructorHelpers::FObjectFinder<USoundWave> Blow_S(TEXT("/Game/Sound/SwordHitSound.SwordHitSound"));
	if (Blow_S.Succeeded())
	{
		BlowSound = Blow_S.Object;
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void APOPULCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APOPULCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APOPULCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APOPULCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APOPULCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APOPULCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APOPULCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APOPULCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APOPULCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APOPULCharacter::OnResetVR);

	PlayerInputComponent->BindAction("UIActive", IE_Pressed, this, &APOPULCharacter::UIActive);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &APOPULCharacter::OpenInventory);
	PlayerInputComponent->BindAction("InformationKey", IE_Pressed, this, &APOPULCharacter::OpenInformation);

	PlayerInputComponent->BindAction("ActionKey", IE_Pressed, this, &APOPULCharacter::ActionKey);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &APOPULCharacter::LeftClickFunction);
	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &APOPULCharacter::RightClickFunction);
	PlayerInputComponent->BindAction("WeaponVisibility", IE_Pressed, this, &APOPULCharacter::WeaponVisibilityKey);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APOPULCharacter::RollKey);
	PlayerInputComponent->BindAction("QuickSlot", IE_Pressed, this, &APOPULCharacter::QuickSlotKey);
	PlayerInputComponent->BindAction("EnterKey", IE_Pressed, this, &APOPULCharacter::EnterKey);
	PlayerInputComponent->BindAction("QuickSave", IE_Pressed, this, &APOPULCharacter::QuickSave);
	

	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &APOPULCharacter::KeyDownQuitGame);
	PlayerInputComponent->BindAction("TestKey", IE_Pressed, this, &APOPULCharacter::TestKey);

	
}


void APOPULCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APOPULCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APOPULCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APOPULCharacter::TurnAtRate(float Rate)
{
	
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APOPULCharacter::LookUpAtRate(float Rate)
{
	
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APOPULCharacter::MoveForward(float Value)
{
	if (!m_bIsMove || m_bIsDeath)
		return;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APOPULCharacter::MoveRight(float Value)
{
	if (!m_bIsMove || m_bIsDeath)
		return;
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APOPULCharacter::StopJumping()
{
	Super::StopJumping();
	m_bIsJumStrat = false;
	m_bIsJum = false;
}

void APOPULCharacter::Jump()
{
	//Super::Jump();
	m_bIsJumStrat = true;
	GetWorldTimerManager().SetTimer(m_JumDelayTimer,this,&APOPULCharacter::DelayJump, 0.135f);
}

void APOPULCharacter::DelayJump()
{
	Super::Jump();
	m_bIsJumStrat = false;
	m_bIsJum = true;
}

void APOPULCharacter::ZeroHP()
{
	if (nHP <= 0)
	{
		nHP = 0;
	}
	//사망함수

}

void APOPULCharacter::UIActive()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PController->bShowMouseCursor
		= !PController->bShowMouseCursor;
	if (PController->bShowMouseCursor == true)
		PController->SetInputMode(FInputModeGameAndUI());
	else
		PController->SetInputMode(FInputModeGameOnly());
}

void APOPULCharacter::InventoryActive(bool bIsActive)
{
	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();
	if (bIsActive)
		DefaultWidget->GetInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
	else
	{
		DefaultWidget->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
		APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PController->bShowMouseCursor = true;
		PController->SetInputMode(FInputModeGameAndUI());
	}
}

void APOPULCharacter::OpenInventory()
{
	m_bIsActiveInventory = !m_bIsActiveInventory;
	OnInventoryActive.Broadcast(m_bIsActiveInventory);
}

void APOPULCharacter::InformationActive(bool bIsActive)
{
	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();
	if (bIsActive)
		DefaultWidget->GetInformationBorder()->SetVisibility(ESlateVisibility::Hidden);
	else
	{
		DefaultWidget->GetInformationBorder()->SetVisibility(ESlateVisibility::Visible);
		DefaultWidget->UpdataInformationText();
		APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PController->bShowMouseCursor = true;
		PController->SetInputMode(FInputModeGameAndUI());
	}
}

void APOPULCharacter::OpenInformation()
{
	m_bIsActiveInmormation = !m_bIsActiveInmormation;
	OnInformationActive.Broadcast(m_bIsActiveInmormation);
}



void APOPULCharacter::AddItem( CItem* ItemData, UUserWidget* ItemWidget, int Actortype )
{
	m_MyInventory.AddItem(ItemData, ItemWidget, Actortype);
}

void APOPULCharacter::BeginPlay()
{
	Super::BeginPlay();

	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();
	if (DefaultWidget != nullptr)
	{
		DefaultWidget->BindCharacterSetting(this);
		DefaultWidget->SettingActionUI(false);

		OnHPChange.AddUObject(this, &APOPULCharacter::ZeroHP);
		OnInventoryActive.AddUObject(this, &APOPULCharacter::InventoryActive);
		OnInformationActive.AddUObject(this, &APOPULCharacter::InformationActive);

		
	}
	bIsActorType = ActorType::E_PC;
	UGameDataInstance* Data = Cast<UGameDataInstance>(GetGameInstance());
	

	if (Data != nullptr)
	{
		if (bIsMoveLevel == true)
		{
			// 캐릭터 가먼저 호출되어 위젯이 널포이트값이 된다 그래서 위젯에서 초기화할것
			//m_MyInventory.SetMaxInventoryItem(InvenMax);
			//Data->LoadPCDataOpenLevel(this);
			//APOPULCharacter::bIsMoveLevel = false;

		}
		else
		{
			APOPULCharacter::AddMoney(10000);
			m_Ability.nAttack = 25;
			m_Ability.nDefend = 20;
			m_Ability.nAttackSpeed = 1.0f;
			m_Ability.nSTR = 10; 
			m_Ability.nDEX = 8;
			m_OriginAbility = m_Ability;
			m_OriginAbility.nMaxHP = 1600;
			UpDataAbility();
			APOPULCharacter::AddHP(m_Ability.nMaxHP);
		}
	}
}

void APOPULCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 확인후 지울것 무적스위치

	if (m_MyInventory.GetItem() != nullptr)
		GEngine->AddOnScreenDebugMessage(0,0.1f,FColor::Red, FString::FromInt( m_MyInventory.GetItem()->GetQuickSlotSettingID()));
	else
		GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Red, TEXT("망겜"));
	//값확인하며 방향 정할것
	if (AttackMove)
	{
		FVector PCVector = GetActorLocation();
		SetActorLocation(PCVector);
		FVector PC = GetActorForwardVector();
		PCVector.X += (PC.X * 1.0f);
		PCVector.Y += (PC.Y * 1.0f);
		SetActorLocation(PCVector);
	}
	GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Red, FString::FromInt(m_OriginAbility.nDefend));

	if (m_bIstraceOn)
		APOPULCharacter::AttackTraceFunction();

	UPCAnimInstance* PCAnimInstance = Cast<UPCAnimInstance>(GetMesh()->GetAnimInstance());
	bool AnyMontage = PCAnimInstance->PlayingSkillAttackMontage();
	if (AnyMontage)
	{
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FRotator PCamerRoation = PlayerCameraManager->GetCameraRotation();
		FRotator PCRotation = GetActorRotation();
		PCRotation.Yaw = PCamerRoation.Yaw;
		SetActorRotation(PCRotation);
	}
	bool IsPlayingAnyMontage = PCAnimInstance->IsAnyMontagePlaying();
	if (!IsPlayingAnyMontage)
		m_bIsMove = true;
	if (bIsCountVisibility)
	{
		APlayerController* PController = Cast<APlayerController>(this->GetController());
		PController->SetControlRotation(CountBoxUICameraRotation);
	}





}

float  APOPULCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (m_Ability.nHP <= 0)
	{
		if (!m_bIsDeath)
		{
			m_bIsDeath = true;
			GetWorldTimerManager().SetTimer(m_DeathTimerHandle, this, &APOPULCharacter::Deathtimer, 5.0f);
		}
	}
	OnHPChange.Broadcast();
	return Damage;
}

void APOPULCharacter::Deathtimer()
{
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
		GameMode->OpenLevel(FString(L"HouseMap"));
	
}

bool APOPULCharacter::Destroy(bool bNetForce, bool bShouldModifyLevel)
{
	bool pendingkillpednding = Super::Destroy();
	if (!pendingkillpednding)
		m_MyInventory.Delete();
	return pendingkillpednding;
}

void APOPULCharacter::AddMoney(int Value)
{
	m_MyInventory.AddMoney(Value);
	OnMoneyChange.Broadcast();
}

void APOPULCharacter::SetMaxHP(int Value)
{
	m_Ability.nMaxHP = Value;
	OnHPChange.Broadcast();
}

void APOPULCharacter::AddHP(int Value)
{
	m_Ability.nHP += Value;
	if (m_Ability.nHP >= m_Ability.nMaxHP)
		m_Ability.nHP = m_Ability.nMaxHP;
	OnHPChange.Broadcast();
}

bool APOPULCharacter::IsMaxHP()
{
	if (m_Ability.nHP == m_Ability.nMaxHP)
		return true;
	return false;
}

void APOPULCharacter::SetOverlapActor(AActor* Actor)
{
	
	if (Actor != nullptr && this != nullptr)
		OverlapActor = Actor;
	else
		OverlapActor = nullptr;
}

int APOPULCharacter::OverlapActorTypeFind()
{
	
	if (OverlapActor == nullptr)
		return ActorType::E_NONE;
	AEnemyBaseCharacter* ECharacter = Cast<AEnemyBaseCharacter>(OverlapActor);
	if (ECharacter != nullptr)
		return ECharacter->GetbIsActorType();
	ABaseActor* BaseActor = Cast<ABaseActor>(OverlapActor);
	if (BaseActor != nullptr)
		return BaseActor->GetbIsActorType();
	return ActorType::E_NONE;
}

bool APOPULCharacter::bIsOverlapActorEnemyBaseCharacter()
{
	int Type = APOPULCharacter::OverlapActorTypeFind();
	switch (Type)
	{
	case ActorType::E_NONE:
		return false;
	case ActorType::E_Enemy:
	case ActorType::E_NPC:
	case ActorType::E_BOX:
		return true;
	case ActorType::E_DestructionObject:
	case ActorType::E_NonDestructionObject:
	case ActorType::E_CreateObject:
		return false;
	}
	return false;
}

void APOPULCharacter::LoadAbility(Ability& Abil, Ability& OriginAbil)
{
	m_Ability = Abil;
	m_OriginAbility = OriginAbil;
}

void APOPULCharacter::LoadInventory(CInventory& Inventory)
{
	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();

	m_MyInventory.SetMoney(Inventory.GetMoney());
	OnMoneyChange.Broadcast();
	for (int i = 0; i < m_MyInventory.GetMax(); i++)
	{
		if (Inventory.GetItem()[i].GetItemInformation()->nItemID != 0)
		{
			if (Inventory.GetItem()[i].GetItemIcon() == nullptr)
				Inventory.GetItem()[i].SetItem(ATextureManager::GetTextrues()[Inventory.GetItem()[i].GetItemIconID()]);
			m_MyInventory.AddItem_Insert(&Inventory.GetItem()[i], i, DefaultWidget);
			m_MyInventory.GetItem()[i].SetQuickSlotSettingID(Inventory.GetItem()[i].GetQuickSlotSettingID());
		}
	}
}

void APOPULCharacter::LoadEquipment(CEquipment& Equipment)
{
	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();
	TArray<UInventorySlotUserWidget*> EquipmentSlot = DefaultWidget->GetEquipmentSlotWidget();
	UGameDataInstance* Data = Cast<UGameDataInstance>(GetGameInstance());

	m_Equipment = Equipment;
	if (EquipmentSlot.GetData()[EquipmentItemKinds::E_Weapone]->GetItemData() != nullptr)
	{
		CItem* Item = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone);
	
			Item->SetItem(ATextureManager::GetTextrues()[Item->GetItemIconID()]);
		EquipmentSlot.GetData()[EquipmentItemKinds::E_Weapone]->SetSlotItemData(m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone));
	}
	else
	{
		if (Data->GetLoadData())
		{
			CItem* Item = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone);

				Item->SetItem(ATextureManager::GetTextrues()[Item->GetItemIconID()]);
			EquipmentSlot.GetData()[EquipmentItemKinds::E_Weapone]->SetSlotItemData(m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone));
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("널값"));
	}
	if (EquipmentSlot.GetData()[EquipmentItemKinds::E_Armor]->GetItemData() != nullptr)
	{
		CItem* Item = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Armor);
		
			Item->SetItem(ATextureManager::GetTextrues()[Item->GetItemIconID()]);
		EquipmentSlot.GetData()[EquipmentItemKinds::E_Armor]->SetSlotItemData(m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Armor));
	}
	else
	{
		if (Data->GetLoadData())
		{
			CItem* Item = m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Armor);
		
				Item->SetItem(ATextureManager::GetTextrues()[Item->GetItemIconID()]);
			EquipmentSlot.GetData()[EquipmentItemKinds::E_Armor]->SetSlotItemData(m_Equipment.GetEquipmentItem(EquipmentItemKinds::E_Weapone));
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("널값"));
	}
	UpDataAbility();
	DefaultWidget->UpDataWidget();
}

void APOPULCharacter::PlayerLocationToRootBoneLocation()
{
	FVector rootBoneVector;
	FVector PlayerVector;
	rootBoneVector = GetMesh()->GetBoneLocation(FName(L"root"));
	PlayerVector = GetActorLocation();
	rootBoneVector.Z = PlayerVector.Z;
	this->SetActorLocation(rootBoneVector);
}

void APOPULCharacter::AttackTraceFunction()
{
	if (!m_bIsBattel)
		return;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsToIgnore.Add(this);
	FVector StartLocation = WeponMesh->GetSocketLocation(FName(L"Damage_Start"));
	FVector EndLocation = WeponMesh->GetSocketLocation(FName(L"Damage_End"));
	FVector RadiusLocation = WeponMesh->GetSocketLocation(FName(L"Radius_S"));
	float Radius = UKismetMathLibrary::GetPointDistanceToSegment(RadiusLocation, StartLocation, EndLocation);
	bool IsOK = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, Radius, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHits,true);

	if (IsOK)
	{
		int Max = OutHits.Num();
		for (int i = 0; i < Max; i++)
		{
			AActor* HitActor = OutHits.GetData()[i].GetActor();
			if (HitActor == nullptr)
				continue;
			AEnemyBaseCharacter* HitEnemy = Cast<AEnemyBaseCharacter>(HitActor);
			if (HitEnemy == nullptr)
				continue;
			if (HitEnemy->GetbIsHit())
				continue;
			// 죽은 몬스터한테 타격음 안나게할려면 주석해제
			if (HitEnemy->GetAbility()->nHP <= 0)
				continue;
			int nRnad = rand() % 50 + 75;
			float fRand = (float)((float)nRnad * 0.01f);
			float fDamage = (float)((float)m_Ability.nAttack * fRand);
			UGameplayStatics::ApplyDamage(HitActor, fDamage,GetController(),this,UDamageType::StaticClass());
			HitEnemy->SetbIsHit(true);
			if (BlowSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlowSound, GetActorLocation());
			}
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::FromInt((int)((int)fDamage * 100)));
		}
	}
}

void APOPULCharacter::FindeObject()
{
	FVector Start = this->GetActorLocation();
	// 무시 엑터
	TArray<AActor*> arryActor;
	TArray<FHitResult> arryHit;
	FHitResult Hit;
	// 인식이 안될때에는 box콜리전 값조정해보기
	bool IsOk = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, Start, FVector(80.0f,80.0f,100.0f),FRotator(0.0f, 0.0f,0.0f),ETraceTypeQuery::TraceTypeQuery1,false, arryActor,EDrawDebugTrace::Type::None, arryHit, false);
	if (IsOk)
	{
		for (int i = 0; i < arryHit.Max(); i++)
		{
			ABaseActor* Object = Cast<ABaseActor>(arryHit.GetData()[i].GetActor());

			if (Object != nullptr)
			{
				if (Object->GetbIsActorType() == ActorType::E_DestructionObject
					|| Object->GetbIsActorType() == ActorType::E_NonDestructionObject)
				{
					
					Object->BoxComponent->OnComponentBeginOverlap.Broadcast(nullptr,this,nullptr,1,true, Hit);
					
					return;
				}
			}
		}
	}
}

void APOPULCharacter::ActionKey()
{
	if (OverlapActor == nullptr)
		return;
	AMainHUD* MyHUD = Cast<AMainHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	UDefualtUserWidget* DefaultWidget = (UDefualtUserWidget*)MyHUD->__GetDefualtWidget();
	CInventory* Inven = nullptr;
	ABaseActor* BaseActor = Cast<ABaseActor>(OverlapActor);
	int Max = 0;
	if (BaseActor != nullptr)
	{
		switch (BaseActor->GetbIsActorType())
		{
		case ActorType::E_DestructionObject:
		case ActorType::E_NonDestructionObject:
			Inven = BaseActor->GetInventory();
			if (Inven == nullptr)
				return;
			Max = BaseActor->GetInventory()->GetMax();
			for (int i = 0; i < Max; i++)
			{
				if (Inven->GetItem()[i].GetItemInformation()->nItemID == 0
					|| Inven->GetItem()[i].GetItemInformation() == nullptr)
					continue;
				m_MyInventory.AddItem(&Inven->GetItem()[i], DefaultWidget);
				Inven->DeleteItemfromInventoyNumber(i, Inven->GetItem()[i].GetItemInformation()->nCount);

			}
			if (BaseActor->GetbIsActorType() == ActorType::E_DestructionObject)
				BaseActor->Destroy();

			DefaultWidget->SettingActionUI(false);
			APOPULCharacter::FindeObject();
			return;
		case ActorType::E_CreateObject:
			DefaultWidget->GetCreateBorder()->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}

	AEnemyBaseCharacter* EnemyActor = Cast<AEnemyBaseCharacter>(OverlapActor);
	if (EnemyActor != nullptr)
	{
		
		UTextBlock* Text = Cast<UTextBlock>(DefaultWidget->GetEnemyInventoryHeaderBorder()->GetChildAt(0));
		CInventory* TEst = EnemyActor->GetInventory();
		CInventory TEst2 = *EnemyActor->GetInventory();
		TEst2.GetItem();
		TEst->GetItem();
		switch (EnemyActor->GetbIsActorType())
		{
		case ActorType::E_Enemy:
			if (EnemyActor->GetAbility()->nHP > 0)
			{
				DefaultWidget->SettingActionUI(false);
				break;
			}
		case ActorType::E_BOX:
			DefaultWidget->GetEnemyInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
			Text->SetText(FText::FromString(EnemyActor->GetNaem()));
			break;
		case ActorType::E_NPC:
			DefaultWidget->VisibilityDialog(ESlateVisibility::Visible, (DailogName)EnemyActor->GetDialogNumber());
			Text->SetText(FText::FromString(L"NPC"));
			break;

		}
	}
}

void APOPULCharacter::LeftClickFunction()
{
	m_bIsleftClicked = true;
	if (m_bIsJum || !m_bIsActiveInventory || bIsCountVisibility)
		return;
	if (m_bIsBattel && m_bIsJumStrat == false )
	{
		UPCAnimInstance* PCAnimInstance = Cast<UPCAnimInstance>(GetMesh()->GetAnimInstance());
		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FRotator PCamerRoation = PlayerCameraManager->GetCameraRotation();
		FRotator PCRotation = GetActorRotation();
		bool AnyMontage = PCAnimInstance->IsAnyMontagePlaying();
		if (!AnyMontage)
		{
			PCRotation.Yaw = PCamerRoation.Yaw;
			SetActorRotation(PCRotation);
			PCAnimInstance->PlayAttackMontage(m_Ability.nAttackSpeed);
		}
		if (m_bIsAdditionalOn)
		{
			m_bIsAdditional_Input = true;
			if (m_bIsRolling)
			{
				PCRotation.Yaw = PCamerRoation.Yaw;
				SetActorRotation(PCRotation);
				PCAnimInstance->PlayAttackMontage(m_Ability.nAttackSpeed);
			}
		}
	}
}

void APOPULCharacter::RightClickFunction()
{
	m_bIsleftClicked = false;
	if (m_bIsJum || !m_bIsActiveInventory || bIsCountVisibility)
		return;
	if (m_bIsBattel && m_bIsJumStrat == false)
	{
		UPCAnimInstance* PCAnimInstance = Cast<UPCAnimInstance>(GetMesh()->GetAnimInstance());
		bool AnyMontage = PCAnimInstance->IsAnyMontagePlaying();
		if (!AnyMontage)
		{
			APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			FRotator PCamerRoation = PlayerCameraManager->GetCameraRotation();
			FRotator PCRotation = GetActorRotation();
			PCRotation.Yaw = PCamerRoation.Yaw;
			SetActorRotation(PCRotation);
			PCAnimInstance->PlaySkillAttackMontage(m_Ability.nAttackSpeed);
		}
	}
}

void APOPULCharacter::KeyDownQuitGame()
{
	APlayerController* PCController = Cast<APlayerController>(GetController());
	AMainHUD* MainHUD = Cast<AMainHUD>(PCController->GetHUD());
	UDefualtUserWidget* DefaultUserWidget = Cast<UDefualtUserWidget>(MainHUD->__GetDefualtWidget());
	DefaultUserWidget->QuitUIAndGame();
}

void APOPULCharacter::QuitGame()
{
	APlayerController* PCController = Cast<APlayerController>(GetController());
	AMainHUD* MainHUD = Cast<AMainHUD>(PCController->GetHUD());
	UDefaultUserWidget* DefaultUserWidget = Cast<UDefualtUserWidget>(MainHUD->__GetDefualtWidget());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(MainHUD->__GetOkBoxWidget());
	DefaultUserWidget->SetQuickSlotSet(false);
	if (OKBox->GetOKBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
	{
		OKBox->NoButtonClickedFunction();
	}
	else if (OKBox->GetCountBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
	{
		OKBox->CountNoButtonClickedFunction();
	}
	else
	{
		OKBox->GetOKBoxBorder()->SetVisibility(ESlateVisibility::Visible);
		OKBox->OKTextSetText(FText::FromString(L"종료 하시겠습니까?"));
		m_bIsKeyDownQuitGame = true;
		PCController->bShowMouseCursor = true;
		if (PCController->bShowMouseCursor == true)
			PCController->SetInputMode(FInputModeGameAndUI());
	}

}

void APOPULCharacter::WeaponVisibilityKey()
{
	static bool Visivility = true;
	if (WeponMesh->IsVisible())
	{
		WeponMesh->SetVisibility(false);
		m_bIsBattel = false;
	}
	else
	{
		WeponMesh->SetVisibility(true);
		m_bIsBattel = true;
	}
}

void APOPULCharacter::SetWeaponVisibility(bool BOOL)
{
	WeponMesh->SetVisibility(BOOL);
}

void APOPULCharacter::QuickSave()
{
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SaveGameData();
	UKismetSystemLibrary::PrintString(GetWorld(), (FString)(L"저장하였습니다."));
}

void APOPULCharacter::EnterKey()
{
	APlayerController* PCController = Cast<APlayerController>(GetController());
	AMainHUD* MainHUD = Cast<AMainHUD>(PCController->GetHUD());
	UOKBoxUserWidget* OKBox = Cast<UOKBoxUserWidget>(MainHUD->__GetOkBoxWidget());
	UDefualtUserWidget* DefaultUserWidget = Cast<UDefualtUserWidget>(MainHUD->__GetDefualtWidget());
	if(OKBox->GetOKBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
		OKBox->YesButtonClickedFunction();
	if(OKBox->GetCountBoxBorder()->GetVisibility() == ESlateVisibility::Visible)
		OKBox->CountYesButtonClickedFunction();
}

#define WKEYDOWN 0
#define SKEYDOWN 1
#define AKEYDOWN 2
#define DKEYDOWN 3
void APOPULCharacter::RollKey()
{
	if (m_bIsJum)
		return;
	m_bIsMove = true;
	APlayerController* PCController = Cast<APlayerController>(GetController());
	UPCAnimInstance* PCAnimInstance = Cast<UPCAnimInstance>(GetMesh()->GetAnimInstance());
	bool KeyDown[4] = {false,false,false,false};
	int i = 0,Count = 0;
	KeyDown[WKEYDOWN] = PCController->IsInputKeyDown(EKeys::W);
	KeyDown[SKEYDOWN] = PCController->IsInputKeyDown(EKeys::S);
	KeyDown[AKEYDOWN] = PCController->IsInputKeyDown(EKeys::A);
	KeyDown[DKEYDOWN] = PCController->IsInputKeyDown(EKeys::D);

	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FRotator PCamerRoation = PlayerCameraManager->GetCameraRotation();
	FRotator PCRotation = GetActorRotation();
	PCRotation.Yaw = PCamerRoation.Yaw;
	SetActorRotation(PCRotation);
	AttackMove = false;
	m_bIstraceOn = false;
	m_bIsRolling = true;

	for (i = 0; i < 4; i++)
	{
		if (KeyDown[i])
			Count++;
	}
	switch (Count)
	{
	case 0:
		PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_BACKWARD, m_Ability.nAttackSpeed);
		return;
	case 1:
		if (KeyDown[WKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_FORWARD, m_Ability.nAttackSpeed);
		else if (KeyDown[SKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_BACKWARD, m_Ability.nAttackSpeed);
		else if (KeyDown[AKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_LEFT, m_Ability.nAttackSpeed);
		else if (KeyDown[DKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_RIAHT, m_Ability.nAttackSpeed);
		return;
	case 2:
	case 3:
	case 4:
		if (KeyDown[WKEYDOWN] && KeyDown[AKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_FORWARDLEFT, m_Ability.nAttackSpeed);
		else if (KeyDown[WKEYDOWN] && KeyDown[DKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_FORWARDRIAHT, m_Ability.nAttackSpeed);
		else if (KeyDown[SKEYDOWN] && KeyDown[AKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_BACKWARDLEFT, m_Ability.nAttackSpeed);
		else if (KeyDown[SKEYDOWN] && KeyDown[DKEYDOWN])
			PCAnimInstance->PlayRollMontage(EROLLDIRECTION::E_BACKWARDRIAHT, m_Ability.nAttackSpeed);
		return;
	}
}

#define KEY_1 0
#define KEY_2 1
#define KEY_3 2
#define KEY_4 3
void APOPULCharacter::QuickSlotKey()
{
	APlayerController* PCController = Cast<APlayerController>(GetController());
	AMainHUD* HUD = Cast<AMainHUD>(PCController->GetHUD());
	UDefaultUserWidget* DefaultWidget = Cast<UDefaultUserWidget>(HUD->__GetDefualtWidget());
	APOPULGameMode* GameMode = Cast<APOPULGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	bool KeyDown[4] = { false,false,false,false };
	KeyDown[KEY_1] = PCController->IsInputKeyDown(EKeys::One);
	KeyDown[KEY_2] = PCController->IsInputKeyDown(EKeys::Two);
	KeyDown[KEY_3] = PCController->IsInputKeyDown(EKeys::Three);
	KeyDown[KEY_4] = PCController->IsInputKeyDown(EKeys::Four);
	for (int i = 0; i < 4; i++)
	{
		UInventorySlotUserWidget* QuickSlot = DefaultWidget->GetQuickSlot().GetData()[i];
		if (KeyDown[i])
		{
			if (QuickSlot != nullptr)
			{
				CItem* Item = QuickSlot->GetItemData();
				if (Item != nullptr)
				{
					GameMode->UsingItem_Ex(Item, this);
					break;
	
				}
			}
		}
		
	}


}

void APOPULCharacter::SetCountBoxUICameraRotation()
{
	APlayerController* PController = Cast<APlayerController>(GetController());
	CountBoxUICameraRotation = PController->GetControlRotation();
}

void APOPULCharacter::TestKey()
{
	TArray<AActor*> EActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemyBaseCharacter::StaticClass(), EActor);
	for (int i = 0 ;i < EActor.Num(); i++)
	{
		AEnemyBaseCharacter* EnemyCharacter = Cast<AEnemyBaseCharacter>(EActor.GetData()[i]);
		if (EnemyCharacter != nullptr)
		{
			EnemyCharacter->GetAbility()->nDefend += 10;
		}
	}

}