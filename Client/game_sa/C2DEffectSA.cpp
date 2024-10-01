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
#include "CGameSA.h"

extern CGameSA* pGame;

C2DEffectSA::C2DEffectSA(C2DEffectSAInterface* effectInterface, std::uint32_t modelID) : m_effectInterface(effectInterface), m_model(modelID)
{
    pGame->Get2DEffects()->AddToList(this);
}

void C2DEffectSA::Destroy() const
{
    CModelInfo* modelInfo = pGame->GetModelInfo(m_model);
    if (modelInfo)
        modelInfo->Remove2DFX(m_effectInterface, false);
}

void C2DEffectSA::SetPosition(const CVector& position)
{
    if (m_effectInterface)
        m_effectInterface->position = RwV3d{position.fX, position.fY, position.fZ};
}

CVector& C2DEffectSA::GetPosition()
{
    if (m_effectInterface)
        return CVector(m_effectInterface->position.x, m_effectInterface->position.y, m_effectInterface->position.z);

    return CVector();
}

void C2DEffectSA::SetCoronaFarClip(float clip)
{
    if (IsValidLight())
        m_effectInterface->effect.light.coronaFarClip = clip;
}

void C2DEffectSA::SetCoronaPointLightRange(float range)
{
    if (IsValidLight())
        m_effectInterface->effect.light.pointLightRange = range;
}

void C2DEffectSA::SetCoronaSize(float size)
{
    if (IsValidLight())
        m_effectInterface->effect.light.coronaSize = size;
}

void C2DEffectSA::SetShadowSize(float size)
{
    if (IsValidLight())
        m_effectInterface->effect.light.shadowSize = size;
}

void C2DEffectSA::SetShadowMultiplier(std::uint8_t multiplier)
{
    if (IsValidLight())
        m_effectInterface->effect.light.shadowColorMultiplier = multiplier;
}

void C2DEffectSA::SetCoronaShowMode(e2dCoronaFlashType showMode)
{
    if (IsValidLight())
        m_effectInterface->effect.light.coronaFlashType = showMode;
}

void C2DEffectSA::SetCoronaReflectionsEnabled(bool enable)
{
    if (IsValidLight())
        m_effectInterface->effect.light.coronaEnableReflection = enable;
}

void C2DEffectSA::SetCoronaFlareType(std::uint8_t flareType)
{
    if (IsValidLight())
        m_effectInterface->effect.light.coronaFlareType = flareType;
}

void C2DEffectSA::SetLightFlags(std::uint16_t flags)
{
    if (IsValidLight())
        m_effectInterface->effect.light.flags = flags;
}

void C2DEffectSA::SetShadowDistance(std::int8_t distance)
{
    if (IsValidLight())
        m_effectInterface->effect.light.shadowZDistance = distance;
}

void C2DEffectSA::SetCoronaOffsets(const CVector& offsets)
{
    if (IsValidLight())
    {
        m_effectInterface->effect.light.offsetX = offsets.fX;
        m_effectInterface->effect.light.offsetY = offsets.fY;
        m_effectInterface->effect.light.offsetZ = offsets.fZ;
    }
}

void C2DEffectSA::SetCoronaColor(const RwColor& color)
{
    if (IsValidLight())
        m_effectInterface->effect.light.color = color;
}

void C2DEffectSA::SetCoronaTexture(const std::string& name)
{
    if (IsValidLight())
    {
        if (m_effectInterface->effect.light.coronaTex)
            RwTextureDestroy(m_effectInterface->effect.light.coronaTex);

        // Call CTxdStore::PushCurrentTxd
        ((void(__cdecl*)())FUNC_PushCurrentTxd)();
        // Call CTxdStore::FindTxdSlot
        int slot = ((int(__cdecl*)(const char*))FUNC_FindTxdSlot)("particle");
        // Call CTxdStore::SetCurrentTxd
        ((void(__cdecl*)(int))FUNC_SetCurrentTxd)(slot);

        m_effectInterface->effect.light.coronaTex = RwReadTexture(name.c_str(), nullptr);

        // Call CTxdStore::PopCurrentTxd
        ((void(__cdecl*)())FUNC_PopCurrentTxd)();
    }
}

void C2DEffectSA::SetShadowTexture(const std::string& name)
{
    if (IsValidLight())
    {
        if (m_effectInterface->effect.light.shadowTex)
            RwTextureDestroy(m_effectInterface->effect.light.shadowTex);

        // Call CTxdStore::PushCurrentTxd
        ((void(__cdecl*)())FUNC_PushCurrentTxd)();
        // Call CTxdStore::FindTxdSlot
        int slot = ((int(__cdecl*)(const char*))FUNC_FindTxdSlot)("particle");
        // Call CTxdStore::SetCurrentTxd
        ((void(__cdecl*)(int))FUNC_SetCurrentTxd)(slot);

        m_effectInterface->effect.light.shadowTex = RwReadTexture(name.c_str(), nullptr);

        // Call CTxdStore::PopCurrentTxd
        ((void(__cdecl*)())FUNC_PopCurrentTxd)();
    }
}

CVector C2DEffectSA::GetCoronaOffsets() const
{
    if (IsValidLight())
        return CVector(m_effectInterface->effect.light.offsetX, m_effectInterface->effect.light.offsetY, m_effectInterface->effect.light.offsetZ);

    return CVector();
}

void C2DEffectSA::SetParticleName(const std::string& name)
{
    if (m_effectInterface && m_effectInterface->type == e2dEffectType::PARTICLE)
        std::strncpy(m_effectInterface->effect.particle.szName, name.c_str(), 24);
}

void C2DEffectSA::SetRoadsignSize(const RwV2d& size)
{
    if (IsValidRoadsign())
        m_effectInterface->effect.roadsign.size = size;
}

void C2DEffectSA::SetRoadsignRotation(const RwV3d& rotation)
{
    if (IsValidRoadsign())
        m_effectInterface->effect.roadsign.rotation = rotation;
}

void C2DEffectSA::SetRoadsignFlags(std::uint8_t flags)
{
    if (IsValidRoadsign())
        m_effectInterface->effect.roadsign.flags = flags;
}

void C2DEffectSA::SetRoadsignText(const std::string& text, std::uint8_t line)
{
    if (IsValidRoadsign())
    {
        if (!m_effectInterface->effect.roadsign.text)
            m_effectInterface->effect.roadsign.text = static_cast<char*>(std::malloc(64));

        if (!m_effectInterface->effect.roadsign.text)
            return;

        std::strncpy(m_effectInterface->effect.roadsign.text + 16 * (line - 1), text.c_str(), 16);
    }
}

void C2DEffectSA::SetEscalatorBottom(const RwV3d& bottom)
{
    if (IsValidEscalator())
        m_effectInterface->effect.escalator.bottom = bottom;
}

void C2DEffectSA::SetEscalatorTop(const RwV3d& top)
{
    if (IsValidEscalator())
        m_effectInterface->effect.escalator.top = top;
}

void C2DEffectSA::SetEscalatorEnd(const RwV3d& end)
{
    if (IsValidEscalator())
        m_effectInterface->effect.escalator.end = end;
}

void C2DEffectSA::SetEscalatorDirection(std::uint8_t direction)
{
    if (IsValidEscalator())
        m_effectInterface->effect.escalator.direction = direction;
}

C2DEffectSAInterface* C2DEffectSA::CreateCopy(C2DEffectSAInterface* effect)
{
    C2DEffectSAInterface* copy = new C2DEffectSAInterface();
    MemCpyFast(copy, effect, sizeof(C2DEffectSAInterface));

    // Create a copy of textures for the lights
    // We must to do this, because C2DEffect::Shutdown removes them
    if (copy->type == e2dEffectType::LIGHT)
    {
        if (effect->effect.light.coronaTex && effect->effect.light.shadowTex)
            C2DEffectSA::PrepareTexturesForLightEffect(copy->effect.light.coronaTex, copy->effect.light.shadowTex, effect->effect.light.coronaTex->name, effect->effect.light.shadowTex->name, false);
    }
    else if (copy->type == e2dEffectType::ROADSIGN)
    {
        // Create a copy of text and atomic for the roadsign
        // We must to do this, because C2DEffect::Shutdown removes them
        copy->effect.roadsign.text = static_cast<char*>(std::malloc(64));
        if (copy->effect.roadsign.text)
        {
            std::memset(copy->effect.roadsign.text, 0, 64);
            std::strncpy(copy->effect.roadsign.text, effect->effect.roadsign.text, 64);
        }

        copy->effect.roadsign.atomic = RpAtomicClone(effect->effect.roadsign.atomic);
    }

    return copy;
}

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
