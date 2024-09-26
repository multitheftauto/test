/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/logic/CClient2DFXManager.h
 *  PURPOSE:     Handling 2DFX effects manager
 *
 *****************************************************************************/
#pragma once

#include "CClient2DFX.h"
#include "../game_sa/C2DEffectSAInterface.h"

class CClient2DFXManager
{
    friend class CClient2DFX;

public:
    CClient2DFXManager(class CClientManager* mainManager);
    ~CClient2DFXManager();

    void RemoveAll();
    bool Exists(CClient2DFX* effect) const;

    CClient2DFX* Add2DFX(std::uint32_t model, const CVector& position, const e2dEffectType& type, const effectDataMap& effectData);

    bool Set2DFXProperties(C2DEffectSAInterface* effect, const effectDataMap& effectData);
    effectDataMap Get2DFXProperties(C2DEffectSAInterface* effect) const;

    bool Set2DFXProperty(C2DEffectSAInterface* effect, const e2dEffectProperty& property, const std::variant<bool, float, std::string>& propertyValue);
    std::variant<float, bool, std::string> Get2DFXProperty(C2DEffectSAInterface* effect, const e2dEffectProperty& property);

    void Set2DFXPosition(C2DEffectSAInterface* effect, const CVector& position);
    CVector* Get2DFXPosition(C2DEffectSAInterface* effect) const;

    static bool        IsValidModel(std::uint32_t model) noexcept;
    static const char* IsValidEffectData(const e2dEffectType& effectType, const effectDataMap& effectData);

private:
    void AddToList(CClient2DFX* effect) { m_effectsList.push_back(effect); }
    void RemoveFromList(CClient2DFX* effect);

    class CClientManager* m_mainManager;

    std::list<CClient2DFX*> m_effectsList;
    bool                    m_canRemoveFromList;
};
