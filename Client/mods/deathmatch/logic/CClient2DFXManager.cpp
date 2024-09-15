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
        return nullptr;

    return effect;
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
        case e2dEffectType::ATTRACTOR:
            // Unnecessary in MTA
            break;
        case e2dEffectType::SUN_GLARE:
            // It has no properties, it only uses position and type fields
            break;
        case e2dEffectType::FURNITURE:
            // Unnecessary in MTA (Probably unused even in SA)
            break;
        case e2dEffectType::ENEX:
            // Unnecessary in MTA
            break;
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
        case e2dEffectType::TRIGGER_POINT:
        {
            auto* id = MapFind(effectData, "id");
            if (!id || !std::holds_alternative<float>(*id))
                return "Invalid \"id\" value";

            break;
        }
        case e2dEffectType::COVER_POINT:
            // Unnecessary in MTA
            break;
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
    }

    return nullptr;
}
