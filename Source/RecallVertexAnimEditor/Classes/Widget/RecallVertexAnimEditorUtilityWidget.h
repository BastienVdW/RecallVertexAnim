// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "EditorUtilityWidget.h"

#include "RecallVertexAnimEditorUtilityWidget.generated.h"

class UDetailsView;
class USinglePropertyView;

/**
 * Widget to display the countdown at the start of the race.
 */
UCLASS(Abstract)
class RECALLVERTEXANIMEDITOR_API URecallVertexAnimEditorUtilityWidget :
	public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	URecallVertexAnimEditorUtilityWidget();

	// UUserWidget implementation Begin
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// UUserWidget implementation End

	// UObject implementation Begin
public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// UObject implementation End

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=VertexAnim)
	TObjectPtr<class URecallAnimToTexturePresetAsset> PresetAsset;

	UPROPERTY(EditAnywhere, Category=VertexAnim)
	FText VirtualAnimationTextureFormat;
	
	UPROPERTY(EditAnywhere, Category=VertexAnim)
	FText DataAssetFormat;
	
	UPROPERTY(EditAnywhere, Category=VertexAnim)
	int32 LightmapIndex = 2;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USinglePropertyView> SinglePropertyView_PresetAsset;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDetailsView> DetailsView_PresetAsset;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UEditorUtilityButton> BTN_CreateSTMeshFrSel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UEditorUtilityButton> BTN_CreateTexFrSel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UEditorUtilityButton> BTN_CreateDataAssetFrSel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UEditorUtilityButton> BTN_SetDataParams;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<class UEditorUtilityButton> BTN_BAKE_DATA;
	
	UPROPERTY(Transient)
	FString AssetDirectoryPath;
	
	UFUNCTION(BlueprintCallable)
	USkeletalMesh* GetSelectedSkeletalMesh(FString& OutSkeletalMeshName);
	
	UFUNCTION(BlueprintCallable)
	UTexture2D* CreateVertexAnimationTexture(FString Filename, FString TextureTypeName);
	
	UFUNCTION(BlueprintCallable)
	void CreateAllVertexAnimationTextures();
	
	UFUNCTION(BlueprintCallable)
	void CreateDataAsset();
	
	UFUNCTION(BlueprintCallable)
	void SetDataAssetParameters();
	
	UFUNCTION(BlueprintCallable)
	void BakeData();

	UFUNCTION()
	void CreateStaticMesh();
};
