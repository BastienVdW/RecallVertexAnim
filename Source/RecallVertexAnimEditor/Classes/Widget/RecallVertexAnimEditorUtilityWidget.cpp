// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "RecallVertexAnimEditorUtilityWidget.h"

#include "AnimToTextureBPLibrary.h"
#include "EditorFilterLibrary.h"
#include "EditorUtilityLibrary.h"
#include "EditorUtilityWidgetComponents.h"
#include "IAssetTools.h"
#include "Animation/AnimationSettings.h"
#include "Components/DetailsView.h"
#include "Components/SinglePropertyView.h"
#include "Data/VertexAnim/RecallAnimToTexturePreset.h"
#include "Kismet/BlueprintPathsLibrary.h"
#include "Materials/MaterialInstanceConstant.h"

#define LOCTEXT_NAMESPACE "URecallVertexAnimEditorUtilityWidget"

URecallVertexAnimEditorUtilityWidget::URecallVertexAnimEditorUtilityWidget()
	: Super()
{
	VirtualAnimationTextureFormat = NSLOCTEXT("URecallVertexAnimEditorUtilityWidget", "VirtualAnimationTextureFormat", "VAT_{Name}_{VATTexName}");
	DataAssetFormat = NSLOCTEXT("URecallVertexAnimEditorUtilityWidget", "DataAssetFormat", "DA_{Name}");
}

void URecallVertexAnimEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SinglePropertyView_PresetAsset)
	{
		SinglePropertyView_PresetAsset->SetPropertyName(TEXT("PresetAsset"));
		SinglePropertyView_PresetAsset->SetObject(this);
	}

	if (BTN_CreateSTMeshFrSel)
	{
		BTN_CreateSTMeshFrSel->OnClicked.AddDynamic(this, &ThisClass::CreateStaticMesh);
	}

	if (BTN_CreateTexFrSel)
	{
		BTN_CreateTexFrSel->OnClicked.AddDynamic(this, &ThisClass::CreateAllVertexAnimationTextures);
	}

	if (BTN_CreateDataAssetFrSel)
	{
		BTN_CreateDataAssetFrSel->OnClicked.AddDynamic(this, &ThisClass::CreateDataAsset);
	}

	if (BTN_SetDataParams)
	{
		BTN_SetDataParams->OnClicked.AddDynamic(this, &ThisClass::SetDataAssetParameters);
	}

	if (BTN_BAKE_DATA)
	{
		BTN_BAKE_DATA->OnClicked.AddDynamic(this, &ThisClass::BakeData);
	}
}

void URecallVertexAnimEditorUtilityWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (BTN_CreateSTMeshFrSel)
	{
		BTN_CreateSTMeshFrSel->OnClicked.RemoveDynamic(this, &ThisClass::CreateStaticMesh);
	}

	if (BTN_CreateTexFrSel)
	{
		BTN_CreateTexFrSel->OnClicked.RemoveDynamic(this, &ThisClass::CreateAllVertexAnimationTextures);
	}

	if (BTN_CreateDataAssetFrSel)
	{
		BTN_CreateDataAssetFrSel->OnClicked.RemoveDynamic(this, &ThisClass::CreateDataAsset);
	}

	if (BTN_SetDataParams)
	{
		BTN_SetDataParams->OnClicked.RemoveDynamic(this, &ThisClass::SetDataAssetParameters);
	}

	if (BTN_BAKE_DATA)
	{
		BTN_BAKE_DATA->OnClicked.RemoveDynamic(this, &ThisClass::BakeData);
	}
}

void URecallVertexAnimEditorUtilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URecallVertexAnimEditorUtilityWidget::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == TEXT("PresetAsset"))
	{
		if (DetailsView_PresetAsset)
		{
			DetailsView_PresetAsset->SetObject(PresetAsset);
		}
	}
}

void URecallVertexAnimEditorUtilityWidget::CreateStaticMesh()
{
	FString SkeletalMeshName;
	USkeletalMesh* SkeletalMesh = GetSelectedSkeletalMesh(SkeletalMeshName);
	if (!SkeletalMesh || SkeletalMeshName.IsEmpty())
	{
		return;
	}

	const FString PackageName = FString::Printf(TEXT("%s/SM_%s"), *AssetDirectoryPath, *SkeletalMeshName);
	
	UStaticMesh* StaticMesh = UAnimToTextureBPLibrary::ConvertSkeletalMeshToStaticMesh(SkeletalMesh, PackageName);

	if (PresetAsset)
	{
		PresetAsset->Preset.StaticMesh = StaticMesh;
		PresetAsset->Modify();
	}
}

USkeletalMesh* URecallVertexAnimEditorUtilityWidget::GetSelectedSkeletalMesh(FString& OutSkeletalMeshName)
{
	USkeletalMesh* SelectedSkeletalMesh = nullptr;
	
	if (PresetAsset && PresetAsset->Preset.SourceSkeletalMesh)
	{
		SelectedSkeletalMesh = PresetAsset->Preset.SourceSkeletalMesh;
	}
	else
	{
		const TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
		const TArray<UObject*> SelectedSkeletalMeshAssets = UEditorFilterLibrary::ByClass(SelectedAssets, USkeletalMesh::StaticClass());
	
		if (SelectedSkeletalMeshAssets.Num() == 0)
		{
			return nullptr;
		}

		SelectedSkeletalMesh = CastChecked<USkeletalMesh>(SelectedSkeletalMeshAssets[0]);
	
		if (PresetAsset)
		{
			PresetAsset->Preset.SourceSkeletalMesh = SelectedSkeletalMesh;
			PresetAsset->Modify();
		}
	}

	if (SelectedSkeletalMesh)
	{
		FString Extension;
		UBlueprintPathsLibrary::Split(SelectedSkeletalMesh->GetPathName(),
			AssetDirectoryPath, OutSkeletalMeshName, Extension);

	}

	return SelectedSkeletalMesh;
}

UTexture2D* URecallVertexAnimEditorUtilityWidget::CreateVertexAnimationTexture(FString Filename, FString TextureTypeName)
{
	if (AssetDirectoryPath.IsEmpty())
	{
		return nullptr;
	}
	
	IAssetTools& AssetTools = IAssetTools::Get();

	FFormatNamedArguments Args;
	Args.Add("Name", FText::FromString(Filename));
	Args.Add("VATTexName", FText::FromString(TextureTypeName));
		
	const FText TextureName = FText::Format(VirtualAnimationTextureFormat, Args);

	UObject* NewAsset = AssetTools.CreateAsset(TextureName.ToString(), AssetDirectoryPath, UTexture2D::StaticClass(), nullptr);
	UTexture2D* NewTexture = Cast<UTexture2D>(NewAsset);
	if (!NewTexture)
	{
		return nullptr;
	}

	NewTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	NewTexture->CompressionSettings = TextureCompressionSettings::TC_HDR;
	NewTexture->SRGB = false;
	NewTexture->Modify();

	return NewTexture;
}

void URecallVertexAnimEditorUtilityWidget::CreateAllVertexAnimationTextures()
{
	if (!PresetAsset || !PresetAsset->Preset.SourceSkeletalMesh)
	{
		return;	
	}
	
	FString Path, Filename, Extension;
	UBlueprintPathsLibrary::Split(PresetAsset->Preset.SourceSkeletalMesh->GetPathName(),
		Path, Filename, Extension);
	if (Filename.IsEmpty())
	{
		return;
	}

	PresetAsset->Preset.BonePositionTexture = CreateVertexAnimationTexture(Filename, TEXT("BPT"));
	PresetAsset->Preset.BoneRotationTexture = CreateVertexAnimationTexture(Filename, TEXT("BRT"));
	PresetAsset->Preset.BoneWeightTexture = CreateVertexAnimationTexture(Filename, TEXT("BWT"));
	PresetAsset->Modify();
}

void URecallVertexAnimEditorUtilityWidget::CreateDataAsset()
{
	if (!PresetAsset || !PresetAsset->Preset.SourceSkeletalMesh)
	{
		return;
	}

	USkeletalMesh* SkeletalMesh = PresetAsset->Preset.SourceSkeletalMesh;
	
	FString Filename, Extension;
	UBlueprintPathsLibrary::Split(SkeletalMesh->GetPathName(),
		AssetDirectoryPath, Filename, Extension);
	
	IAssetTools& AssetTools = IAssetTools::Get();

	FFormatNamedArguments Args;
	Args.Add("Name", FText::FromString(Filename));
		
	const FText DataAssetName = FText::Format(DataAssetFormat, Args);

	UObject* NewAsset = AssetTools.CreateAsset(DataAssetName.ToString(), AssetDirectoryPath, UAnimToTextureDataAsset::StaticClass(), nullptr);
	UAnimToTextureDataAsset* AnimToTextureDataAsset = Cast<UAnimToTextureDataAsset>(NewAsset);
	if (!AnimToTextureDataAsset)
	{
		return;
	}

	AnimToTextureDataAsset->MaxHeight = 4096;
	AnimToTextureDataAsset->MaxWidth = 4096;
	AnimToTextureDataAsset->Precision = EAnimToTexturePrecision::SixteenBits;
	AnimToTextureDataAsset->bAutoPlay = false;
	AnimToTextureDataAsset->NumBoneInfluences = EAnimToTextureNumBoneInfluences::Four;
	AnimToTextureDataAsset->SampleRate = static_cast<float>(UAnimationSettings::Get()->GetDefaultFrameRate().Numerator);
	AnimToTextureDataAsset->Modify();

	PresetAsset->Preset.AnimToTextureDataAsset = AnimToTextureDataAsset;
}

void URecallVertexAnimEditorUtilityWidget::SetDataAssetParameters()
{
	if (!PresetAsset || !PresetAsset->Preset.AnimToTextureDataAsset)
	{
		return;
	}
	
	UAnimToTextureDataAsset* AnimToTextureDataAsset = PresetAsset->Preset.AnimToTextureDataAsset;

	AnimToTextureDataAsset->AnimSequences.Empty(PresetAsset->Preset.Animations.Num());

	for (const TObjectPtr<UAnimSequence>& AnimSequence : PresetAsset->Preset.Animations)
	{
		FAnimToTextureAnimSequenceInfo& AnimSequenceInfo = AnimToTextureDataAsset->AnimSequences.AddDefaulted_GetRef();
		AnimSequenceInfo.AnimSequence = AnimSequence;
	}

	AnimToTextureDataAsset->SkeletalMesh = PresetAsset->Preset.SourceSkeletalMesh;
	AnimToTextureDataAsset->StaticMesh = PresetAsset->Preset.StaticMesh;
	AnimToTextureDataAsset->Mode = EAnimToTextureMode::Bone;
	AnimToTextureDataAsset->BonePositionTexture = PresetAsset->Preset.BonePositionTexture;
	AnimToTextureDataAsset->BoneRotationTexture = PresetAsset->Preset.BoneRotationTexture;
	AnimToTextureDataAsset->BoneWeightTexture = PresetAsset->Preset.BoneWeightTexture;
	AnimToTextureDataAsset->Modify();
}

void URecallVertexAnimEditorUtilityWidget::BakeData()
{
	if (!PresetAsset || !PresetAsset->Preset.AnimToTextureDataAsset || !PresetAsset->Preset.StaticMesh)
	{
		return;	
	}

	UStaticMesh* StaticMesh = PresetAsset->Preset.StaticMesh;
	UAnimToTextureDataAsset* AnimToTextureDataAsset = PresetAsset->Preset.AnimToTextureDataAsset;

	UAnimToTextureBPLibrary::SetLightMapIndex(StaticMesh, AnimToTextureDataAsset->StaticLODIndex, LightmapIndex, true);
	if (!UAnimToTextureBPLibrary::AnimationToTexture(AnimToTextureDataAsset))
	{
		return;
	}

	for (const FStaticMaterial& StaticMaterial: StaticMesh->GetStaticMaterials())
	{
		if (UMaterialInstanceConstant* MaterialInstanceConstant = Cast<UMaterialInstanceConstant>(StaticMaterial.MaterialInterface))
		{
			UAnimToTextureBPLibrary::UpdateMaterialInstanceFromDataAsset(
				AnimToTextureDataAsset, MaterialInstanceConstant);
		}
	}
}

#undef LOCTEXT_NAMESPACE
