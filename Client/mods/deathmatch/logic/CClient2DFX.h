/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/logic/CClient2DFX.h
 *  PURPOSE:     Handling 2DFX effects
 *
 *****************************************************************************/
#pragma once

#include "CClientEntity.h"

class CClient2DFX final : public CClientEntity
{
    DECLARE_CLASS(CClient2DFX, CClientEntity)
    friend class CClient2DFXManager;

public:
    CClient2DFX(class CClientManager* manager, ElementID ID);
    ~CClient2DFX();

    eClientEntityType GetType() const { return CCLIENT2DFX; }

    void Unlink(){};
    void GetPosition(CVector& vecPosition) const {}
    void SetPosition(const CVector& vecPosition){}

    bool Create(std::uint32_t model, const CVector& position, const e2dEffectType& type, std::unordered_map<std::string, std::variant<bool, float, std::string>>& effectData);

    e2dEffectType  Get2DFXType() const noexcept { return m_effectType; }
    C2DEffectSAInterface* Get2DFX() const noexcept { return m_effectInterface; }

private:
    class CClient2DFXManager* m_2DFXManager;
    C2DEffectSAInterface*     m_effectInterface;
    e2dEffectType             m_effectType;
};
