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

	// ���κ��丮���� Ŭ���޴���
	void SetOKEnemyItemClick() { m_bIsEnemyItemCilck = true; }
	// �÷��̾� �κ��丮���� Ŭ���޴���
	void SetOKPCItemClick() { m_bIsPCItemCilck = true; }
	void SetCountdata();
	// �ݾ� Ȱ��ȭ ��Ȱ��ȭ
	void SetCountWonVisible(ESlateVisibility InVisibility);
	//OKText ����
	void OKTextSetText(const FText& text);
	//MessageText ����
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

	// Yse ��ư ���ε� �Լ�
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

	// ī��Ʈ �ڽ�
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

	// �޼��� �ڽ�
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
