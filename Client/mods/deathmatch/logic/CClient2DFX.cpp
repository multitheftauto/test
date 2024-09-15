/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/logic/CClient2DFX.cpp
 *  PURPOSE:     Handling 2DFX effects
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CClient2DFX.h"
#include "CClient2DFXManager.h"
#include "../game_sa/C2DEffectSAInterface.h"

#define mask(n) ((1 << (n)) - 1)

CClient2DFX::CClient2DFX(class CClientManager* manager, ElementID ID) : ClassInit(this), CClientEntity(ID), m_2DFXManager(manager->Get2DFXManager())
{
    m_pManager = manager;
    SetTypeName("2dfx");

    m_2DFXManager->AddToList(this);
}

CClient2DFX::~CClient2DFX()
{
    m_2DFXManager->RemoveFromList(this);
}

bool CClient2DFX::Create(std::uint32_t model, const CVector& position, const e2dEffectType& type, std::unordered_map<std::string, std::variant<bool, float, std::string>>& effectData)
{
    CModelInfo* modelInfo = g_pGame->GetModelInfo(static_cast<DWORD>(model));
    if (!modelInfo)
        return false;

    C2DEffectSAInterface* effect = modelInfo->Add2DFXEffect(position, type);
    if (!effect)
        return false;

    // Set effect
    m_effectInterface = effect;

    // Push current TXD
    ((void(__cdecl*)())0x7316A0)();

    // Find txd slot
    int slot = ((int(__fastcall*)(int,int,char*))0x731850)(0,0,"particle");

    // Set txd slot
    ((void(__cdecl*)(int))0x7319C0)(slot);

    // Set properties
    switch (type)
    {
        case e2dEffectType::LIGHT:
        {
            t2dEffectLight& light = effect->effect.light;

            auto* drawDistance = MapFind(effectData, "draw_distance");
            light.coronaFarClip = std::get<float>(*drawDistance);

            auto* lightRange = MapFind(effectData, "light_range");
            light.pointLightRange = std::get<float>(*lightRange);

            auto* coronaSize = MapFind(effectData, "corona_size");
            light.coronaSize = std::get<float>(*coronaSize);

            auto* shadowSize = MapFind(effectData, "shadow_size");
            light.shadowSize = std::get<float>(*shadowSize);

            auto* shadowMultiplier = MapFind(effectData, "shadow_multiplier");
            light.shadowColorMultiplier = static_cast<std::uint8_t>(std::get<float>(*shadowMultiplier));

            auto* showMode = MapFind(effectData, "show_mode");
            e2dCoronaFlashType flashType;
            StringToEnum(std::get<std::string>(*showMode), flashType);
            light.coronaFlashType = flashType;

            auto* coronaReflection = MapFind(effectData, "corona_reflection");
            light.coronaEnableReflection = std::get<bool>(*coronaReflection);

            auto* coronaFlareType = MapFind(effectData, "flare_type");
            light.coronaFlareType = static_cast<std::uint8_t>(std::get<float>(*coronaFlareType));

            auto* flags = MapFind(effectData, "flags");
            light.flags = static_cast<std::uint16_t>(std::get<float>(*flags));

            auto* shadowZDistance = MapFind(effectData, "shadow_distance");
            light.shadowZDistance = static_cast<std::int8_t>(std::get<float>(*shadowZDistance));

            auto* offsetX = MapFind(effectData, "offsetX");
            light.offsetX = static_cast<std::int8_t>(std::get<float>(*offsetX));

            auto* offsetY = MapFind(effectData, "offsetY");
            light.offsetY = static_cast<std::int8_t>(std::get<float>(*offsetY));

            auto* offsetZ = MapFind(effectData, "offsetZ");
            light.offsetZ = static_cast<std::int8_t>(std::get<float>(*offsetZ));

            auto* color = MapFind(effectData, "color");
            int colorValue = static_cast<int>(std::get<float>(*color));
            light.color = RwColor{static_cast<std::uint8_t>((colorValue >> 0) & mask(8)), static_cast<std::uint8_t>((colorValue >> 8) & mask(8)), static_cast<std::uint8_t>((colorValue >> 16) & mask(8)), static_cast<std::uint8_t>((colorValue >> 24) & mask(8))};

            auto* coronaTexture = MapFind(effectData, "corona_name");
            light.coronaTex = ((RwTexture * (__cdecl*)(const char*, const char*))0x7F3AC0)(std::get<std::string>(*coronaTexture).c_str(), nullptr);

            auto* shadowTexture = MapFind(effectData, "shadow_name");
            light.shadowTex = ((RwTexture * (__cdecl*)(const char*, const char*))0x7F3AC0)(std::get<std::string>(*shadowTexture).c_str(), nullptr);

            // ???
            if (light.flags & 4) // withoutCorona
                effect->effect.particle.szName[20] &= ~2u;

            break;
        }
        case e2dEffectType::PARTICLE:
        {
            auto* particleName = MapFind(effectData, "name");
            auto&  stringName = std::get<std::string>(*particleName);
            std::strncpy(effect->effect.particle.szName, stringName.c_str(), sizeof(effect->effect.particle.szName) - 1);

            break;
        }
        case e2dEffectType::ROADSIGN:
        {
            t2dEffectRoadsign& roadsign = effect->effect.roadsign;

            auto* sizeX = MapFind(effectData, "sizeX");
            auto* sizeY = MapFind(effectData, "sizeY");
            roadsign.size = RwV2d{std::get<float>(*sizeX), std::get<float>(*sizeY)};

            auto* rotX = MapFind(effectData, "rotX");
            auto* rotY = MapFind(effectData, "rotY");
            auto* rotZ = MapFind(effectData, "rotZ");
            roadsign.rotation = RwV3d{std::get<float>(*rotX), std::get<float>(*rotY), std::get<float>(*rotZ)};

            auto* flags = MapFind(effectData, "flags");
            roadsign.flags = static_cast<std::uint8_t>(std::get<float>(*flags));

            auto* text = MapFind(effectData, "text");
            auto* text2 = MapFind(effectData, "text2");
            auto* text3 = MapFind(effectData, "text3");
            auto* text4 = MapFind(effectData, "text4");

            roadsign.text = static_cast<char*>(malloc(64));
            if (roadsign.text)
            {
                std::memcpy(roadsign.text, std::get<std::string>(*text).c_str(), 4);
                std::memcpy(roadsign.text + 4, std::get<std::string>(*text2).c_str(), 4);
                std::memcpy(roadsign.text + 8, std::get<std::string>(*text3).c_str(), 4);
                std::memcpy(roadsign.text + 12, std::get<std::string>(*text4).c_str(), 4);
            }

            roadsign.atomic = nullptr;

            break;
        }
        case e2dEffectType::TRIGGER_POINT:
        {
            auto* triggerID = MapFind(effectData, "id");
            effect->effect.triggerPoint.id = static_cast<std::int32_t>(std::get<float>(*triggerID));

            break;
        }
        case e2dEffectType::ESCALATOR:
        {
            t2dEffectEscalator& escalator = effect->effect.escalator;

            auto* bottomX = MapFind(effectData, "bottomX");
            auto* bottomY = MapFind(effectData, "bottomY");
            auto* bottomZ = MapFind(effectData, "bottomZ");
            escalator.bottom = RwV3d{std::get<float>(*bottomX), std::get<float>(*bottomY), std::get<float>(*bottomZ)};

            auto* topX = MapFind(effectData, "topX");
            auto* topY = MapFind(effectData, "topY");
            auto* topZ = MapFind(effectData, "topZ");
            escalator.top = RwV3d{std::get<float>(*topX), std::get<float>(*topY), std::get<float>(*topZ)};

            auto* endX = MapFind(effectData, "endX");
            auto* endY = MapFind(effectData, "endY");
            auto* endZ = MapFind(effectData, "endZ");
            escalator.end = RwV3d{std::get<float>(*endX), std::get<float>(*endY), std::get<float>(*endZ)};

            auto* dir = MapFind(effectData, "direction");
            escalator.direction = static_cast<std::uint8_t>(std::get<float>(*dir));

            break;
        }
        case e2dEffectType::ATTRACTOR:
        case e2dEffectType::ENEX:
        case e2dEffectType::FURNITURE:
        case e2dEffectType::COVER_POINT:
        case e2dEffectType::SUN_GLARE:
            break;
    }

    // Pop current txd
    ((void(__cdecl*)())0x7316B0)();

    auto* createdEffect = ((C2DEffectSAInterface * (__thiscall*)(CBaseModelInfoSAInterface*, int))0x4C4C70)(modelInfo->GetInterface(), 0);

    return true;
}
