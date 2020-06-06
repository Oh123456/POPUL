// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OKBoxUserWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE(FOnOKBoxDelegate);

UCLASS()
class POPUL_API UOKBoxUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// 적인벤토리에서 클릭햇는지
	void SetOKEnemyItemClick() { m_bIsEnemyItemCilck = true; }
	// 플레이어 인벤토리에서 클릭햇는지
	void SetOKPCItemClick() { m_bIsPCItemCilck = true; }
	void SetCountdata();
	// 금액 활성화 비활성화
	void SetCountWonVisible(ESlateVisibility InVisibility);
	//OKText 세팅
	void OKTextSetText(const FText& text);
	//MessageText 세팅
	void SetMessageText(const FText& text);
	void SetMessageBox(const FText& text);

	const bool GetbIsOK() { return m_bIsOK; }

	int GetCountTotalMoney() const { return m_Count_TotalMoney; }
	int GetCountEditableText_Count() const { return m_CountEditableText_Count; }
	bool GetEnemyItemClick() const { return m_bIsEnemyItemCilck; }
	bool GetPCItemClick() const { return m_bIsPCItemCilck; }
	class UBorder* GetOKBoxBorder() { return OKBoxBorder; }
	class UBorder* GetCountBoxBorder() { return CountBoxBorder; }
	class UTextBlock* GetTotalGoldText() { return TotalGoldText; }
	class UEditableTextBox* GetCountEditableText() { return CountEditableText; }

	// Yse 버튼 바인드 함수
	UFUNCTION()
		void YesButtonClickedFunction();
	UFUNCTION()
		void NoButtonClickedFunction();

	UFUNCTION()
		void CountYesButtonClickedFunction();
	UFUNCTION()
		void CountNoButtonClickedFunction();

	void Tick(FGeometry MyGeometry, float InDeltaTime);

public:
	FOnOKBoxDelegate OnOkBoxInven;
	FOnOKBoxDelegate OnCreateBox;
protected:
	virtual void NativeConstruct() override;
private:

	UFUNCTION()
		void CountEditText(const FText& Text);
	UFUNCTION()
		void CountEditTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
		void MessageButtonClickedFunction();
private:
	UPROPERTY()
		class UButton* YesButton;
	UPROPERTY()
		class UButton* NoButton;
	UPROPERTY()
		class UBorder* OKBoxBorder;
	UPROPERTY()
		class UTextBlock* OKText;

	// 카운트 박스
	UPROPERTY()
		class UButton* CountYesButton;
	UPROPERTY()
		class UButton* CountNoButton;
	UPROPERTY()
		class UBorder* CountBoxBorder;
	UPROPERTY()
		class UTextBlock* TotalGoldText;
	UPROPERTY()
		class UTextBlock* WonText;
	UPROPERTY()
		class UEditableTextBox* CountEditableText;

	// 메세지 박스
	UPROPERTY()
		class UButton* MessageButton;
	UPROPERTY()
		class UBorder* MessageBorder;
	UPROPERTY()
		class UTextBlock* MessageText;

	int m_Count_TotalMoney;
	int m_CountEditableText_Count;
	int m_CountEditableText_CountBuffer;
	bool m_bIsOK;
	bool m_bIsEnemyItemCilck;
	bool m_bIsPCItemCilck;
};
