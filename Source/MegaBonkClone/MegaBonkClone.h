// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 여기에 로그 카테고리들을 선언합니다.
// (카테고리명, 기본 로그 수준, 컴파일 타임 로그 수준)
DECLARE_LOG_CATEGORY_EXTERN(LogObjectPool, Log, All);  // 오브젝트 풀용
DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, Log, All);      // 무기 시스템용
DECLARE_LOG_CATEGORY_EXTERN(LogItem, Log, All);        // 아이템용