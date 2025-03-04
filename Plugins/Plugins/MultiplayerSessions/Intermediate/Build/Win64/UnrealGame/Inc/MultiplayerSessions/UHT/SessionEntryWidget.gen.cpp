// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSessions/Public/SessionEntryWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSessionEntryWidget() {}

// Begin Cross Module References
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_UMenu_NoRegister();
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_USessionEntryWidget();
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_USessionEntryWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessions();
// End Cross Module References

// Begin Class USessionEntryWidget Function OnJoinSessionButtonClicked
struct Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USessionEntryWidget, nullptr, "OnJoinSessionButtonClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(USessionEntryWidget::execOnJoinSessionButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnJoinSessionButtonClicked();
	P_NATIVE_END;
}
// End Class USessionEntryWidget Function OnJoinSessionButtonClicked

// Begin Class USessionEntryWidget Function SessionSetup
struct Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics
{
	struct SessionEntryWidget_eventSessionSetup_Parms
	{
		FString GetSessionID;
		FString GetSessionUser;
		FString GetMaxPlayers;
		int32 SessionIndex;
		FString GetMapType;
		UMenu* Menu;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Menu_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetSessionID;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetSessionUser;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetMaxPlayers;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SessionIndex;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetMapType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Menu;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetSessionID = { "GetSessionID", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, GetSessionID), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetSessionUser = { "GetSessionUser", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, GetSessionUser), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetMaxPlayers = { "GetMaxPlayers", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, GetMaxPlayers), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_SessionIndex = { "SessionIndex", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, SessionIndex), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetMapType = { "GetMapType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, GetMapType), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_Menu = { "Menu", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(SessionEntryWidget_eventSessionSetup_Parms, Menu), Z_Construct_UClass_UMenu_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Menu_MetaData), NewProp_Menu_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetSessionID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetSessionUser,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetMaxPlayers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_SessionIndex,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_GetMapType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::NewProp_Menu,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USessionEntryWidget, nullptr, "SessionSetup", nullptr, nullptr, Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::PropPointers), sizeof(Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::SessionEntryWidget_eventSessionSetup_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::Function_MetaDataParams), Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::SessionEntryWidget_eventSessionSetup_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_USessionEntryWidget_SessionSetup()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_USessionEntryWidget_SessionSetup_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(USessionEntryWidget::execSessionSetup)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_GetSessionID);
	P_GET_PROPERTY(FStrProperty,Z_Param_GetSessionUser);
	P_GET_PROPERTY(FStrProperty,Z_Param_GetMaxPlayers);
	P_GET_PROPERTY(FIntProperty,Z_Param_SessionIndex);
	P_GET_PROPERTY(FStrProperty,Z_Param_GetMapType);
	P_GET_OBJECT(UMenu,Z_Param_Menu);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SessionSetup(Z_Param_GetSessionID,Z_Param_GetSessionUser,Z_Param_GetMaxPlayers,Z_Param_SessionIndex,Z_Param_GetMapType,Z_Param_Menu);
	P_NATIVE_END;
}
// End Class USessionEntryWidget Function SessionSetup

// Begin Class USessionEntryWidget
void USessionEntryWidget::StaticRegisterNativesUSessionEntryWidget()
{
	UClass* Class = USessionEntryWidget::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnJoinSessionButtonClicked", &USessionEntryWidget::execOnJoinSessionButtonClicked },
		{ "SessionSetup", &USessionEntryWidget::execSessionSetup },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(USessionEntryWidget);
UClass* Z_Construct_UClass_USessionEntryWidget_NoRegister()
{
	return USessionEntryWidget::StaticClass();
}
struct Z_Construct_UClass_USessionEntryWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "SessionEntryWidget.h" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SessionNameText_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SessionMatchType_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SessionPlayers_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SessionMapType_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_JoinSessionButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GetSessionName_MetaData[] = {
		{ "Category", "SessionEntryWidget" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MenuRef_MetaData[] = {
		{ "Category", "SessionEntryWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/SessionEntryWidget.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SessionNameText;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SessionMatchType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SessionPlayers;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SessionMapType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_JoinSessionButton;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetSessionName;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MenuRef;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_USessionEntryWidget_OnJoinSessionButtonClicked, "OnJoinSessionButtonClicked" }, // 2724374371
		{ &Z_Construct_UFunction_USessionEntryWidget_SessionSetup, "SessionSetup" }, // 400289229
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USessionEntryWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionNameText = { "SessionNameText", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, SessionNameText), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SessionNameText_MetaData), NewProp_SessionNameText_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionMatchType = { "SessionMatchType", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, SessionMatchType), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SessionMatchType_MetaData), NewProp_SessionMatchType_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionPlayers = { "SessionPlayers", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, SessionPlayers), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SessionPlayers_MetaData), NewProp_SessionPlayers_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionMapType = { "SessionMapType", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, SessionMapType), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SessionMapType_MetaData), NewProp_SessionMapType_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_JoinSessionButton = { "JoinSessionButton", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, JoinSessionButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_JoinSessionButton_MetaData), NewProp_JoinSessionButton_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_GetSessionName = { "GetSessionName", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, GetSessionName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GetSessionName_MetaData), NewProp_GetSessionName_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_MenuRef = { "MenuRef", nullptr, (EPropertyFlags)0x001000000008001c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USessionEntryWidget, MenuRef), Z_Construct_UClass_UMenu_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MenuRef_MetaData), NewProp_MenuRef_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USessionEntryWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionNameText,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionMatchType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionPlayers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_SessionMapType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_JoinSessionButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_GetSessionName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USessionEntryWidget_Statics::NewProp_MenuRef,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USessionEntryWidget_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_USessionEntryWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USessionEntryWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_USessionEntryWidget_Statics::ClassParams = {
	&USessionEntryWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_USessionEntryWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_USessionEntryWidget_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_USessionEntryWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_USessionEntryWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_USessionEntryWidget()
{
	if (!Z_Registration_Info_UClass_USessionEntryWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_USessionEntryWidget.OuterSingleton, Z_Construct_UClass_USessionEntryWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_USessionEntryWidget.OuterSingleton;
}
template<> MULTIPLAYERSESSIONS_API UClass* StaticClass<USessionEntryWidget>()
{
	return USessionEntryWidget::StaticClass();
}
USessionEntryWidget::USessionEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(USessionEntryWidget);
USessionEntryWidget::~USessionEntryWidget() {}
// End Class USessionEntryWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_USessionEntryWidget, USessionEntryWidget::StaticClass, TEXT("USessionEntryWidget"), &Z_Registration_Info_UClass_USessionEntryWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(USessionEntryWidget), 1663534026U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_4286438176(TEXT("/Script/MultiplayerSessions"),
	Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
