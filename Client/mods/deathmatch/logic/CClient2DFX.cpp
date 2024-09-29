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

CClient2DFX::CClient2DFX(class CClientManager* manager, ElementID ID)
    : ClassInit(this), CClientEntity(ID), m_2DFXManager(manager->Get2DFXManager()), m_effectInterface(nullptr), m_effectType(e2dEffectType::NONE)
{
    m_pManager = manager;
    SetTypeName("2dfx");

    m_2DFXManager->AddToList(this);
}

CClient2DFX::~CClient2DFX()
{
    m_2DFXManager->RemoveFromList(this);

    // Destroy effect
    CModelInfo* modelInfo = g_pGame->GetModelInfo(m_model);
    if (!modelInfo)
        return;

    modelInfo->Remove2DFX(m_effectInterface, false);
}

bool CClient2DFX::Create(std::uint32_t model, const CVector& position, const e2dEffectType& type, const effectDataMap& effectData)
{
    CModelInfo* modelInfo = g_pGame->GetModelInfo(static_cast<DWORD>(model));
    if (!modelInfo)
        return false;

    C2DEffectSAInterface* effect = modelInfo->Add2DFXEffect(position, type);
    if (!effect)
        return false;

    // Set effect
    m_effectInterface = effect;
    m_effectType = effect->type;
    m_model = static_cast<DWORD>(model);

    switch (effect->type)
    {
        case e2dEffectType::ROADSIGN:
        {
            t2dEffectRoadsign& roadsign = effect->effect.roadsign;
            roadsign.size = RwV2d{0, 0};
            roadsign.rotation = RwV3d{0, 0, 0};
            roadsign.atomic = nullptr;
            roadsign.text = nullptr;
        }
    }

    if (!m_2DFXManager->Set2DFXProperties(effect, effectData))
        return false;

    return true;
}
