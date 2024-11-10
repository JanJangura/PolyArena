// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SessionEntryWidget.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MULTIPLAYERSESSIONS_SessionEntryWidget_generated_h
#error "SessionEntryWidget.generated.h already included, missing '#pragma once' in SessionEntryWidget.h"
#endif
#define MULTIPLAYERSESSIONS_SessionEntryWidget_generated_h

#define FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execTestingSetup); \
	DECLARE_FUNCTION(execOnJoinSessionButtonClicked);


#define FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSessionEntryWidget(); \
	friend struct Z_Construct_UClass_USessionEntryWidget_Statics; \
public: \
	DECLARE_CLASS(USessionEntryWidget, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MultiplayerSessions"), NO_API) \
	DECLARE_SERIALIZER(USessionEntryWidget)


#define FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USessionEntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	USessionEntryWidget(USessionEntryWidget&&); \
	USessionEntryWidget(const USessionEntryWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USessionEntryWidget); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USessionEntryWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USessionEntryWidget) \
	NO_API virtual ~USessionEntryWidget();


#define FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_14_PROLOG
#define FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_INCLASS_NO_PURE_DECLS \
	FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> MULTIPLAYERSESSIONS_API UClass* StaticClass<class USessionEntryWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_MultiplayerCourse_BlasterGame_Plugins_Plugins_MultiplayerSessions_Source_MultiplayerSessions_Public_SessionEntryWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
