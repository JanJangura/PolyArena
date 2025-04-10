// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSessions/Public/Menu.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMenu() {}

// Begin Cross Module References
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_UMenu();
MULTIPLAYERSESSIONS_API UClass* Z_Construct_UClass_UMenu_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessions();
// End Cross Module References

// Begin Class UMenu Function GetHostInformation
struct Z_Construct_UFunction_UMenu_GetHostInformation_Statics
{
	struct Menu_eventGetHostInformation_Parms
	{
		int32 NumberOfPublicConnections;
		FString CurrentMatchType;
		FName TempSessionName;
		FString CurrentMapType;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumberOfPublicConnections;
	static const UECodeGen_Private::FStrPropertyParams NewProp_CurrentMatchType;
	static const UECodeGen_Private::FNamePropertyParams NewProp_TempSessionName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_CurrentMapType;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_NumberOfPublicConnections = { "NumberOfPublicConnections", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventGetHostInformation_Parms, NumberOfPublicConnections), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_CurrentMatchType = { "CurrentMatchType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventGetHostInformation_Parms, CurrentMatchType), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_TempSessionName = { "TempSessionName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventGetHostInformation_Parms, TempSessionName), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_CurrentMapType = { "CurrentMapType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventGetHostInformation_Parms, CurrentMapType), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_GetHostInformation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_NumberOfPublicConnections,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_CurrentMatchType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_TempSessionName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_GetHostInformation_Statics::NewProp_CurrentMapType,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_GetHostInformation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_GetHostInformation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "GetHostInformation", nullptr, nullptr, Z_Construct_UFunction_UMenu_GetHostInformation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_GetHostInformation_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_GetHostInformation_Statics::Menu_eventGetHostInformation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_GetHostInformation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_GetHostInformation_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UMenu_GetHostInformation_Statics::Menu_eventGetHostInformation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_GetHostInformation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_GetHostInformation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execGetHostInformation)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NumberOfPublicConnections);
	P_GET_PROPERTY(FStrProperty,Z_Param_CurrentMatchType);
	P_GET_PROPERTY(FNameProperty,Z_Param_TempSessionName);
	P_GET_PROPERTY(FStrProperty,Z_Param_CurrentMapType);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetHostInformation(Z_Param_NumberOfPublicConnections,Z_Param_CurrentMatchType,Z_Param_TempSessionName,Z_Param_CurrentMapType);
	P_NATIVE_END;
}
// End Class UMenu Function GetHostInformation

// Begin Class UMenu Function HostButtonClicked
struct Z_Construct_UFunction_UMenu_HostButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "HostButtonClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMenu_HostButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_HostButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execHostButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HostButtonClicked();
	P_NATIVE_END;
}
// End Class UMenu Function HostButtonClicked

// Begin Class UMenu Function JoinButtonClicked
struct Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "JoinButtonClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMenu_JoinButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_JoinButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execJoinButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->JoinButtonClicked();
	P_NATIVE_END;
}
// End Class UMenu Function JoinButtonClicked

// Begin Class UMenu Function MenuSetup
struct Z_Construct_UFunction_UMenu_MenuSetup_Statics
{
	struct Menu_eventMenuSetup_Parms
	{
		int32 NumberOfPublicConnections;
		FString TypeOfMatch;
		FString LobbyPath;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CPP_Default_LobbyPath", "/Game/Scenes/PolyArenaLobbyMap" },
		{ "CPP_Default_NumberOfPublicConnections", "4" },
		{ "CPP_Default_TypeOfMatch", "FreeForAll" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumberOfPublicConnections;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TypeOfMatch;
	static const UECodeGen_Private::FStrPropertyParams NewProp_LobbyPath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_NumberOfPublicConnections = { "NumberOfPublicConnections", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, NumberOfPublicConnections), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_TypeOfMatch = { "TypeOfMatch", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, TypeOfMatch), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_LobbyPath = { "LobbyPath", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventMenuSetup_Parms, LobbyPath), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_NumberOfPublicConnections,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_TypeOfMatch,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_MenuSetup_Statics::NewProp_LobbyPath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_MenuSetup_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "MenuSetup", nullptr, nullptr, Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Menu_eventMenuSetup_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_MenuSetup_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UMenu_MenuSetup_Statics::Menu_eventMenuSetup_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_MenuSetup()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_MenuSetup_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execMenuSetup)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NumberOfPublicConnections);
	P_GET_PROPERTY(FStrProperty,Z_Param_TypeOfMatch);
	P_GET_PROPERTY(FStrProperty,Z_Param_LobbyPath);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->MenuSetup(Z_Param_NumberOfPublicConnections,Z_Param_TypeOfMatch,Z_Param_LobbyPath);
	P_NATIVE_END;
}
// End Class UMenu Function MenuSetup

// Begin Class UMenu Function OnCreateSession
struct Z_Construct_UFunction_UMenu_OnCreateSession_Statics
{
	struct Menu_eventOnCreateSession_Parms
	{
		bool bWasSuccessful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "///////////* CUSTOM DELEGATE CALLBACKS */////////\n// Callbacks for the Custom Delegates on the MultiplayerSessionsSubsystem.\n" },
#endif
		{ "ModuleRelativePath", "Public/Menu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "/ CUSTOM DELEGATE CALLBACKS\n Callbacks for the Custom Delegates on the MultiplayerSessionsSubsystem." },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bWasSuccessful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSuccessful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnCreateSession_Statics::NewProp_bWasSuccessful_SetBit(void* Obj)
{
	((Menu_eventOnCreateSession_Parms*)Obj)->bWasSuccessful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnCreateSession_Statics::NewProp_bWasSuccessful = { "bWasSuccessful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnCreateSession_Parms), &Z_Construct_UFunction_UMenu_OnCreateSession_Statics::NewProp_bWasSuccessful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnCreateSession_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnCreateSession_Statics::NewProp_bWasSuccessful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSession_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnCreateSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnCreateSession", nullptr, nullptr, Z_Construct_UFunction_UMenu_OnCreateSession_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSession_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnCreateSession_Statics::Menu_eventOnCreateSession_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSession_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnCreateSession_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnCreateSession_Statics::Menu_eventOnCreateSession_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnCreateSession()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnCreateSession_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnCreateSession)
{
	P_GET_UBOOL(Z_Param_bWasSuccessful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnCreateSession(Z_Param_bWasSuccessful);
	P_NATIVE_END;
}
// End Class UMenu Function OnCreateSession

// Begin Class UMenu Function OnCreateSessionEntryWidget
struct Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnCreateSessionEntryWidget", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnCreateSessionEntryWidget)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnCreateSessionEntryWidget();
	P_NATIVE_END;
}
// End Class UMenu Function OnCreateSessionEntryWidget

// Begin Class UMenu Function OnDestroySession
struct Z_Construct_UFunction_UMenu_OnDestroySession_Statics
{
	struct Menu_eventOnDestroySession_Parms
	{
		bool bWasSuccessful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bWasSuccessful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSuccessful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnDestroySession_Statics::NewProp_bWasSuccessful_SetBit(void* Obj)
{
	((Menu_eventOnDestroySession_Parms*)Obj)->bWasSuccessful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnDestroySession_Statics::NewProp_bWasSuccessful = { "bWasSuccessful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnDestroySession_Parms), &Z_Construct_UFunction_UMenu_OnDestroySession_Statics::NewProp_bWasSuccessful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnDestroySession_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnDestroySession_Statics::NewProp_bWasSuccessful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySession_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnDestroySession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnDestroySession", nullptr, nullptr, Z_Construct_UFunction_UMenu_OnDestroySession_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySession_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnDestroySession_Statics::Menu_eventOnDestroySession_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnDestroySession_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnDestroySession_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnDestroySession_Statics::Menu_eventOnDestroySession_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnDestroySession()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnDestroySession_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnDestroySession)
{
	P_GET_UBOOL(Z_Param_bWasSuccessful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnDestroySession(Z_Param_bWasSuccessful);
	P_NATIVE_END;
}
// End Class UMenu Function OnDestroySession

// Begin Class UMenu Function OnStartSession
struct Z_Construct_UFunction_UMenu_OnStartSession_Statics
{
	struct Menu_eventOnStartSession_Parms
	{
		bool bWasSucessful;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bWasSucessful_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWasSucessful;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_OnStartSession_Statics::NewProp_bWasSucessful_SetBit(void* Obj)
{
	((Menu_eventOnStartSession_Parms*)Obj)->bWasSucessful = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_OnStartSession_Statics::NewProp_bWasSucessful = { "bWasSucessful", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventOnStartSession_Parms), &Z_Construct_UFunction_UMenu_OnStartSession_Statics::NewProp_bWasSucessful_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_OnStartSession_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_OnStartSession_Statics::NewProp_bWasSucessful,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSession_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_OnStartSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "OnStartSession", nullptr, nullptr, Z_Construct_UFunction_UMenu_OnStartSession_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSession_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_OnStartSession_Statics::Menu_eventOnStartSession_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_OnStartSession_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_OnStartSession_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UMenu_OnStartSession_Statics::Menu_eventOnStartSession_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_OnStartSession()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_OnStartSession_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execOnStartSession)
{
	P_GET_UBOOL(Z_Param_bWasSucessful);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnStartSession(Z_Param_bWasSucessful);
	P_NATIVE_END;
}
// End Class UMenu Function OnStartSession

// Begin Class UMenu Function Refresh
struct Z_Construct_UFunction_UMenu_Refresh_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_Refresh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "Refresh", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_Refresh_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_Refresh_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMenu_Refresh()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_Refresh_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execRefresh)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Refresh();
	P_NATIVE_END;
}
// End Class UMenu Function Refresh

// Begin Class UMenu
void UMenu::StaticRegisterNativesUMenu()
{
	UClass* Class = UMenu::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetHostInformation", &UMenu::execGetHostInformation },
		{ "HostButtonClicked", &UMenu::execHostButtonClicked },
		{ "JoinButtonClicked", &UMenu::execJoinButtonClicked },
		{ "MenuSetup", &UMenu::execMenuSetup },
		{ "OnCreateSession", &UMenu::execOnCreateSession },
		{ "OnCreateSessionEntryWidget", &UMenu::execOnCreateSessionEntryWidget },
		{ "OnDestroySession", &UMenu::execOnDestroySession },
		{ "OnStartSession", &UMenu::execOnStartSession },
		{ "Refresh", &UMenu::execRefresh },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMenu);
UClass* Z_Construct_UClass_UMenu_NoRegister()
{
	return UMenu::StaticClass();
}
struct Z_Construct_UClass_UMenu_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "Menu.h" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Host_Button_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "Menu" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// We've set default values to these passed in parameters. \n// This means that the button Widget on our blueprint will be linked to our button variable in C++.\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "We've set default values to these passed in parameters.\nThis means that the button Widget on our blueprint will be linked to our button variable in C++." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Join_Button_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "Menu" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Find_Button_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "Menu" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentSessionLength_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GetSessionID_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GetSessionUser_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CurrentMaxPlayers_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GetMapType_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bSessionsFound_MetaData[] = {
		{ "Category", "Menu" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Host_Button;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Join_Button;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Find_Button;
	static const UECodeGen_Private::FIntPropertyParams NewProp_CurrentSessionLength;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetSessionID_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_GetSessionID;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetSessionUser_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_GetSessionUser;
	static const UECodeGen_Private::FStrPropertyParams NewProp_CurrentMaxPlayers_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_CurrentMaxPlayers;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GetMapType_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_GetMapType;
	static void NewProp_bSessionsFound_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSessionsFound;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMenu_GetHostInformation, "GetHostInformation" }, // 4006212955
		{ &Z_Construct_UFunction_UMenu_HostButtonClicked, "HostButtonClicked" }, // 3776737500
		{ &Z_Construct_UFunction_UMenu_JoinButtonClicked, "JoinButtonClicked" }, // 3398739101
		{ &Z_Construct_UFunction_UMenu_MenuSetup, "MenuSetup" }, // 1009425111
		{ &Z_Construct_UFunction_UMenu_OnCreateSession, "OnCreateSession" }, // 3813687764
		{ &Z_Construct_UFunction_UMenu_OnCreateSessionEntryWidget, "OnCreateSessionEntryWidget" }, // 3338383630
		{ &Z_Construct_UFunction_UMenu_OnDestroySession, "OnDestroySession" }, // 122917252
		{ &Z_Construct_UFunction_UMenu_OnStartSession, "OnStartSession" }, // 3290975137
		{ &Z_Construct_UFunction_UMenu_Refresh, "Refresh" }, // 139430954
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMenu>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_Host_Button = { "Host_Button", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, Host_Button), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Host_Button_MetaData), NewProp_Host_Button_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_Join_Button = { "Join_Button", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, Join_Button), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Join_Button_MetaData), NewProp_Join_Button_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_Find_Button = { "Find_Button", nullptr, (EPropertyFlags)0x001000000008000c, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, Find_Button), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Find_Button_MetaData), NewProp_Find_Button_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_CurrentSessionLength = { "CurrentSessionLength", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, CurrentSessionLength), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentSessionLength_MetaData), NewProp_CurrentSessionLength_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionID_Inner = { "GetSessionID", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionID = { "GetSessionID", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, GetSessionID), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GetSessionID_MetaData), NewProp_GetSessionID_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionUser_Inner = { "GetSessionUser", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionUser = { "GetSessionUser", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, GetSessionUser), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GetSessionUser_MetaData), NewProp_GetSessionUser_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_CurrentMaxPlayers_Inner = { "CurrentMaxPlayers", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_CurrentMaxPlayers = { "CurrentMaxPlayers", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, CurrentMaxPlayers), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CurrentMaxPlayers_MetaData), NewProp_CurrentMaxPlayers_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetMapType_Inner = { "GetMapType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_GetMapType = { "GetMapType", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, GetMapType), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GetMapType_MetaData), NewProp_GetMapType_MetaData) };
void Z_Construct_UClass_UMenu_Statics::NewProp_bSessionsFound_SetBit(void* Obj)
{
	((UMenu*)Obj)->bSessionsFound = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_bSessionsFound = { "bSessionsFound", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UMenu), &Z_Construct_UClass_UMenu_Statics::NewProp_bSessionsFound_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bSessionsFound_MetaData), NewProp_bSessionsFound_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_Host_Button,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_Join_Button,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_Find_Button,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_CurrentSessionLength,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionID_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionUser_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetSessionUser,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_CurrentMaxPlayers_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_CurrentMaxPlayers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetMapType_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_GetMapType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_bSessionsFound,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UMenu_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMenu_Statics::ClassParams = {
	&UMenu::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UMenu_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams), Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMenu()
{
	if (!Z_Registration_Info_UClass_UMenu.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMenu.OuterSingleton, Z_Construct_UClass_UMenu_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMenu.OuterSingleton;
}
template<> MULTIPLAYERSESSIONS_API UClass* StaticClass<UMenu>()
{
	return UMenu::StaticClass();
}
UMenu::UMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMenu);
UMenu::~UMenu() {}
// End Class UMenu

// Begin Registration
struct Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMenu, UMenu::StaticClass, TEXT("UMenu"), &Z_Registration_Info_UClass_UMenu, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMenu), 1083689319U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h_3472532317(TEXT("/Script/MultiplayerSessions"),
	Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_Menu_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
