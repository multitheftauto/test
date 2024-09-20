/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/logic/CClient2DFXManager.cpp
 *  PURPOSE:     Handling 2DFX effects manager
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CClient2DFXManager.h"

#define mask(n) ((1 << (n)) - 1)

CClient2DFXManager::CClient2DFXManager(CClientManager* mainManager) : m_mainManager(mainManager)
{
    // Init
    m_canRemoveFromList = true;
}

CClient2DFXManager::~CClient2DFXManager()
{
    // Delete all 2DFX's
    RemoveAll();
}

void CClient2DFXManager::RemoveAll()
{
    // Make sure they don't remove themselves from our list
    m_canRemoveFromList = false;

    // Run through our list deleting the 2DFX's
    for (auto* effect : m_effectsList)
        delete effect;

    // Allow list removal again
    m_canRemoveFromList = true;
}

void CClient2DFXManager::RemoveFromList(CClient2DFX* effect)
{
    if (m_canRemoveFromList)
        m_effectsList.remove(effect);
}

bool CClient2DFXManager::Exists(CClient2DFX* effect)
{
    return std::find(m_effectsList.begin(), m_effectsList.end(), effect) != m_effectsList.end();
}

CClient2DFX* CClient2DFXManager::Add2DFX(std::uint32_t model, const CVector& position, const e2dEffectType& type, std::unordered_map<std::string, std::variant<bool, float, std::string>>& effectData)
{
    auto* effect = new CClient2DFX(m_mainManager, INVALID_ELEMENT_ID);
    if (!effect)
        return nullptr;

    if (!effect->Create(model, position, type, effectData))
    {
        delete effect;
        effect = nullptr;
    }

    return effect;
}

bool CClient2DFXManager::Set2DFXProperties(C2DEffectSAInterface* effect, std::unordered_map<std::string, std::variant<bool, float, std::string>>& effectData)
{
    if (!effect)
        return false;

    // Set properties
    switch (effect->type)
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

            auto*              showMode = MapFind(effectData, "show_mode");
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
            int   colorValue = static_cast<int>(std::get<float>(*color));
            light.color = RwColor{static_cast<std::uint8_t>((colorValue >> 0) & mask(8)), static_cast<std::uint8_t>((colorValue >> 8) & mask(8)), static_cast<std::uint8_t>((colorValue >> 16) & mask(8)), static_cast<std::uint8_t>((colorValue >> 24) & mask(8))};

            auto* coronaTexture = MapFind(effectData, "corona_name");
            auto* shadowTexture = MapFind(effectData, "shadow_name");
            PrepareTexturesForLightEffect(light.coronaTex, light.shadowTex, std::get<std::string>(*coronaTexture).c_str(), std::get<std::string>(*shadowTexture).c_str(), true);
            break;
        }
        case e2dEffectType::PARTICLE:
        {
            auto* particleName = MapFind(effectData, "name");
            auto& stringName = std::get<std::string>(*particleName);
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
        case e2dEffectType::TRIGGER_POINT:
        case e2dEffectType::SUN_GLARE: // This effect has no properties but works in MTA
            break;
    }

    return true;
}

std::unordered_map<std::string, std::variant<bool, std::uint32_t, std::uint8_t, std::int8_t, std::uint16_t, float, std::string>> CClient2DFXManager::Get2DFXProperties(C2DEffectSAInterface* effect)
{
    auto properties = std::unordered_map<std::string, std::variant<bool, std::uint32_t, std::uint8_t, std::int8_t, std::uint16_t, float, std::string>>();
    if (!effect)
        return properties;

    // Get properties
    switch (effect->type)
    {
        case e2dEffectType::LIGHT:
        {
            const t2dEffectLight& light = effect->effect.light;

            MapSet(properties, "draw_distance", light.coronaFarClip);
            MapSet(properties, "light_range", light.pointLightRange);
            MapSet(properties, "corona_size", light.coronaSize);
            MapSet(properties, "shadow_size", light.shadowSize);
            MapSet(properties, "shadow_multiplier", light.shadowColorMultiplier);
            MapSet(properties, "show_mode", EnumToString(light.coronaFlashType));
            MapSet(properties, "corona_reflection", light.coronaEnableReflection);
            MapSet(properties, "flare_type", light.coronaFlareType);
            MapSet(properties, "flags", light.flags);
            MapSet(properties, "shadow_distance", light.shadowZDistance);
            MapSet(properties, "offsetX", light.offsetX);
            MapSet(properties, "offsetY", light.offsetY);
            MapSet(properties, "offsetZ", light.offsetZ);
            std::uint32_t colorValue = (static_cast<std::uint32_t>(light.color.r) << 0) |
                               (static_cast<std::uint32_t>(light.color.g) << 8) |
                               (static_cast<std::uint32_t>(light.color.b) << 16) |
                               (static_cast<std::uint32_t>(light.color.a) << 24);
            MapSet(properties, "color", colorValue);
            MapSet(properties, "corona_name", !light.coronaTex ? "" : static_cast<std::string>(light.coronaTex->name));
            MapSet(properties, "shadow_name", !light.shadowTex ? "" : static_cast<std::string>(light.shadowTex->name));

            break;
        }
        case e2dEffectType::PARTICLE:
        {
            MapSet(properties, "name", static_cast<std::string>(effect->effect.particle.szName));
            break;
        }
        case e2dEffectType::ROADSIGN:
        {
            const t2dEffectRoadsign& roadsign = effect->effect.roadsign;

            MapSet(properties, "sizeX", roadsign.size.x);
            MapSet(properties, "sizeY", roadsign.size.y);
            MapSet(properties, "rotX", roadsign.rotation.x);
            MapSet(properties, "rotY", roadsign.rotation.y);
            MapSet(properties, "rotZ", roadsign.rotation.z);
            MapSet(properties, "flags", roadsign.flags);
            MapSet(properties, "text", !roadsign.text ? "" : roadsign.text);

            break;
        }
        case e2dEffectType::ESCALATOR:
        {
            const t2dEffectEscalator& escalator = effect->effect.escalator;

            MapSet(properties, "bottomX", escalator.bottom.x);
            MapSet(properties, "bottomY", escalator.bottom.y);
            MapSet(properties, "bottomZ", escalator.bottom.z);
            MapSet(properties, "topX", escalator.top.x);
            MapSet(properties, "topY", escalator.top.y);
            MapSet(properties, "topZ", escalator.top.z);
            MapSet(properties, "endX", escalator.end.x);
            MapSet(properties, "endY", escalator.end.y);
            MapSet(properties, "endZ", escalator.end.z);
            MapSet(properties, "direction", escalator.direction);

            break;
        }
        case e2dEffectType::ATTRACTOR:
        case e2dEffectType::ENEX:
        case e2dEffectType::FURNITURE:
        case e2dEffectType::COVER_POINT:
        case e2dEffectType::TRIGGER_POINT:
        case e2dEffectType::SUN_GLARE:
            break;
    }

    return properties;
}

void CClient2DFXManager::Set2DFXPosition(C2DEffectSAInterface* effect, const CVector& position)
{
    if (!effect)
        return;

    effect->position = RwV3d{position.fX, position.fY, position.fZ};
}

CVector* CClient2DFXManager::Get2DFXPosition(C2DEffectSAInterface* effect)
{
    if (!effect)
        return nullptr;

    return reinterpret_cast<CVector*>(&effect->position);
}

const char* CClient2DFXManager::IsValidEffectData(const e2dEffectType& effectType, std::unordered_map<std::string, std::variant<bool, float, std::string>>& effectData)
{
    // Check if keys & values are ok!
    switch (effectType)
    {
        case e2dEffectType::LIGHT:
        {
            // corona far clip
            auto* drawDistance = MapFind(effectData, "draw_distance");
            if (!drawDistance || !std::holds_alternative<float>(*drawDistance))
                return "Invalid \"draw_distance\" value";

            auto* lightRange = MapFind(effectData, "light_range");
            if (!lightRange || !std::holds_alternative<float>(*lightRange))
                return "Invalid \"light_range\" value";

            auto* coronaSize = MapFind(effectData, "corona_size");
            if (!coronaSize || !std::holds_alternative<float>(*coronaSize))
                return "Invalid \"corona_size\" value";

            auto* shadowSize = MapFind(effectData, "shadow_size");
            if (!shadowSize || !std::holds_alternative<float>(*shadowSize))
                return "Invalid \"shadow_size\" value";

            auto* shadowMultiplier = MapFind(effectData, "shadow_multiplier");
            if (!shadowMultiplier || !std::holds_alternative<float>(*shadowMultiplier) || std::get<float>(*shadowMultiplier) < 0)
                return "Invalid \"shadow_multiplier\" value";

            auto* showMode = MapFind(effectData, "show_mode");
            e2dCoronaFlashType tempType;
            if (!showMode || !std::holds_alternative<std::string>(*showMode) || !StringToEnum(std::get<std::string>(*showMode), tempType))
                return "Invalid \"show_mode\" value";

            auto* coronaReflection = MapFind(effectData, "corona_reflection");
            if (!coronaReflection || !std::holds_alternative<bool>(*coronaReflection))
                return "Invalid \"corona_reflection\" value";

            auto* coronaFlareType = MapFind(effectData, "flare_type");
            if (!coronaFlareType || !std::holds_alternative<float>(*coronaFlareType) || (std::get<float>(*coronaFlareType) < 0 || std::get<float>(*coronaFlareType) > 1.0f))
                return "Invalid \"flare_type\" value";

            auto* flags = MapFind(effectData, "flags");
            if (!flags || !std::holds_alternative<float>(*flags) || std::get<float>(*flags) < 0)
                return "Invalid \"flags\" value";

            auto* shadowZDistance = MapFind(effectData, "shadow_distance");
            if (!shadowZDistance || (!std::holds_alternative<float>(*shadowZDistance)))
                return "Invalid \"shadow_distance\" value";

            auto* offsetX = MapFind(effectData, "offsetX");
            if (!offsetX || !std::holds_alternative<float>(*offsetX))
                return "Invalid \"offsetX\" value";

            auto* offsetY = MapFind(effectData, "offsetY");
            if (!offsetY || !std::holds_alternative<float>(*offsetY))
                return "Invalid \"offsetY\" value";

            auto* offsetZ = MapFind(effectData, "offsetZ");
            if (!offsetZ || !std::holds_alternative<float>(*offsetZ))
                return "Invalid \"offsetZ\" value";

            auto* color = MapFind(effectData, "color");
            if (!color || !std::holds_alternative<float>(*color))
                return "Invalid \"color\" value";

            auto* coronaTexture = MapFind(effectData, "corona_name");
            e2dEffectTextureName tempName;
            if (!coronaTexture || !std::holds_alternative<std::string>(*coronaTexture) || !StringToEnum(std::get<std::string>(*coronaTexture), tempName))
                return "Invalid \"corona_name\" value";

            auto* shadowTexture = MapFind(effectData, "shadow_name");
            e2dEffectTextureName shadowName;
            if (!shadowTexture || !std::holds_alternative<std::string>(*shadowTexture) || !StringToEnum(std::get<std::string>(*shadowTexture), shadowName))
                return "Invalid \"shadow_name\" value";

            break;
        }
        case e2dEffectType::PARTICLE:
        {
            auto* particleName = MapFind(effectData, "name");
            if (!particleName || !std::holds_alternative<std::string>(*particleName) || std::get<std::string>(*particleName).length() > 24)
                return "Invalid \"particle name\" value";

            break;
        }
        case e2dEffectType::ROADSIGN:
        {
            auto* sizeX = MapFind(effectData, "sizeX");
            if (!sizeX || !std::holds_alternative<float>(*sizeX))
                return "Invalid \"sizeX\" value";

            auto* sizeY = MapFind(effectData, "sizeY");
            if (!sizeY || !std::holds_alternative<float>(*sizeY))
                return "Invalid \"sizeY\" value";

            auto* rotX = MapFind(effectData, "rotX");
            if (!rotX || !std::holds_alternative<float>(*rotX))
                return "Invalid \"rotX\" value";

            auto* rotY = MapFind(effectData, "rotY");
            if (!rotY || !std::holds_alternative<float>(*rotY))
                return "Invalid \"rotY\" value";

            auto* rotZ = MapFind(effectData, "rotZ");
            if (!rotZ || !std::holds_alternative<float>(*rotZ))
                return "Invalid \"rotZ\" value";

            auto* flags = MapFind(effectData, "flags");
            if (!flags || !std::holds_alternative<float>(*flags) || std::get<float>(*flags) < 0)
                return "Invalid \"flags\" value";

            auto* text = MapFind(effectData, "text1");
            if (!text || !std::holds_alternative<std::string>(*text))
                return "Invalid \"text1\" value";

            auto* text2 = MapFind(effectData, "text2");
            if (!text2 || !std::holds_alternative<std::string>(*text2))
                return "Invalid \"text2\" value";

            auto* text3 = MapFind(effectData, "text3");
            if (!text3 || !std::holds_alternative<std::string>(*text3))
                return "Invalid \"text3\" value";

            auto* text4 = MapFind(effectData, "text4");
            if (!text4 || !std::holds_alternative<std::string>(*text4))
                return "Invalid \"text4\" value";

            break;
        }
        case e2dEffectType::ESCALATOR:
        {
            auto* bottomX = MapFind(effectData, "bottomX");
            if (!bottomX || !std::holds_alternative<float>(*bottomX))
                return "Invalid \"bottomX\" value";

            auto* bottomY = MapFind(effectData, "bottomY");
            if (!bottomY || !std::holds_alternative<float>(*bottomY))
                return "Invalid \"bottomY\" value";

            auto* bottomZ = MapFind(effectData, "bottomZ");
            if (!bottomZ || !std::holds_alternative<float>(*bottomZ))
                return "Invalid \"bottomZ\" value";

            auto* topX = MapFind(effectData, "topX");
            if (!topX || !std::holds_alternative<float>(*topX))
                return "Invalid \"topX\" value";

            auto* topY = MapFind(effectData, "topY");
            if (!topY || !std::holds_alternative<float>(*topY))
                return "Invalid \"topY\" value";

            auto* topZ = MapFind(effectData, "topZ");
            if (!topX || !std::holds_alternative<float>(*topZ))
                return "Invalid \"topZ\" value";

            auto* endX = MapFind(effectData, "endX");
            if (!endX || !std::holds_alternative<float>(*endX))
                return "Invalid \"endX\" value";

            auto* endY = MapFind(effectData, "endY");
            if (!endY || !std::holds_alternative<float>(*endY))
                return "Invalid \"endY\" value";

            auto* endZ = MapFind(effectData, "endZ");
            if (!endZ || !std::holds_alternative<float>(*endZ))
                return "Invalid \"endZ\" value";

            auto* direction = MapFind(effectData, "direction");
            if (!direction || !std::holds_alternative<float>(*direction) || (std::get<float>(*direction) < 0 || std::get<float>(*direction) > 1.0f))
                return "Invalid \"direction\" value";

            break;
        }
        case e2dEffectType::SUN_GLARE:
            // It has no properties, it only uses position and type fields
            break;
        case e2dEffectType::ATTRACTOR:
            // Unnecessary in MTA
            break;
        case e2dEffectType::FURNITURE:
            // Unnecessary in MTA (Probably unused even in SA)
            break;
        case e2dEffectType::ENEX:
            // Unnecessary in MTA
            break;
        case e2dEffectType::TRIGGER_POINT:
        {
            // Unnecessary in MTA
            break;
        }
        case e2dEffectType::COVER_POINT:
            // Unnecessary in MTA
            break;
    }

    return nullptr;
}

bool CClient2DFXManager::IsValidModel(std::uint32_t model) noexcept
{
    return CClientObjectManager::IsValidModel(model) || CClientBuildingManager::IsValidModel(model) || CClientVehicleManager::IsValidModel(model);
}
