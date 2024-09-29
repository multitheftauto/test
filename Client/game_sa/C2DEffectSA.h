/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        game_sa/C2DEffectSA.h
 *  PURPOSE:     Header file for 2dfx static class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#define ARRAY_2DFXInfoStore 0xB4C2D8 // C2dfxInfoStore d2fxModels

#define FUNC_C2DEffect_Shutdown 0x4C57D0
#define FUNC_PushCurrentTxd     0x7316A0
#define FUNC_FindTxdSlot        0x731850
#define FUNC_SetCurrentTxd      0x7319C0
#define FUNC_PopCurrentTxd      0x7316B0

// Escalators stuff
#define ARRAY_CEscalators         0xC6E9A8
#define NUM_MAX_ESCALATORS        32
#define FUNC_CEscalator_SwitchOff 0x717860

// fx stuff
#define FUNC_Fx_c_DestroyEntityFx 0x4A1280
#define FUNC_Fx_c_CreateEntityFx  0x4A11E0
#define VAR_G_Fx                  0xA9AE00
#define OFFSET_FxSystem_Entities  0xC
#define OFFSET_FxSystem_Link_Prev 0x4

class C2DEffectSAInterface;

class C2DEffectSA
{
public:
    static int effect2dPluginOffset;

    static void Shutdown(C2DEffectSAInterface* effect);
    static void SafeDelete2DFXEffect(C2DEffectSAInterface* effect);
    static void PrepareTexturesForLightEffect(RwTexture*& coronaTex, RwTexture*& shadowTex, const char* coronaName, const char* shadowName, bool removeIfExist);
};
