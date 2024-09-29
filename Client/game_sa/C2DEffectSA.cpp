/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        game_sa/C2DEffectSA.cpp
 *  PURPOSE:     2DFX static class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "gamesa_renderware.h"
#include "C2DEffectSA.h"
#include "C2DEffectSAInterface.h"

// C2DEffect::Shutdown causes random unknown crash in ntdll.dll so we need own function
void C2DEffectSA::Shutdown(C2DEffectSAInterface* effect)
{
    if (!effect)
        return;

    if (effect->type == e2dEffectType::ROADSIGN)
    {
        t2dEffectRoadsign& roadsign = effect->effect.roadsign;

        if (roadsign.text)
        {
            std::free(roadsign.text);
            roadsign.text = nullptr;
        }

        if (roadsign.atomic)
        {
            RwFrame* frame = RpAtomicGetFrame(roadsign.atomic);
            if (frame)
            {
                RpAtomicSetFrame(roadsign.atomic, nullptr);
                RwFrameDestroy(frame);
            }

            RpAtomicDestroy(roadsign.atomic);
            roadsign.atomic = nullptr;
        }
    }
    else if (effect->type == e2dEffectType::LIGHT)
    {
        t2dEffectLight& light = effect->effect.light;

        if (light.coronaTex)
        {
            RwTextureDestroy(light.coronaTex);
            light.coronaTex = nullptr;
        }

        if (light.shadowTex)
        {
            RwTextureDestroy(light.shadowTex);
            light.shadowTex = nullptr;
        }
    }
}

void C2DEffectSA::SafeDelete2DFXEffect(C2DEffectSAInterface* effect)
{
    if (!effect)
        return;

    if (effect->type == e2dEffectType::ROADSIGN || effect->type == e2dEffectType::LIGHT)
        Shutdown(effect);

    delete effect;
    effect = nullptr;
}

void C2DEffectSA::PrepareTexturesForLightEffect(RwTexture*& coronaTex, RwTexture*& shadowTex, const char* coronaName, const char* shadowName, bool removeIfExist)
{
    // Call CTxdStore::PushCurrentTxd
    ((void(__cdecl*)())FUNC_PushCurrentTxd)();
    // Call CTxdStore::FindTxdSlot
    int slot = ((int(__cdecl*)(const char*))FUNC_FindTxdSlot)("particle");
    // Call CTxdStore::SetCurrentTxd
    ((void(__cdecl*)(int))FUNC_SetCurrentTxd)(slot);

    if (removeIfExist)
    {
        if (coronaTex && coronaName)
            RwTextureDestroy(coronaTex);
        if (shadowTex && shadowName)
            RwTextureDestroy(shadowTex);
    }

    if (coronaName)
        coronaTex = RwReadTexture(coronaName, nullptr);

    if (shadowName)
        shadowTex = RwReadTexture(shadowName, nullptr);

    // Call CTxdStore::PopCurrentTxd
    ((void(__cdecl*)())FUNC_PopCurrentTxd)();
}
