/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        sdk/game/CModelInfo.h
 *  PURPOSE:     Entity model info interface
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include <array>
#include <CVector.h>
#include "CAnimBlock.h"
#include "Common.h"

class CBaseModelInfoSAInterface;
class CColModel;
class CPedModelInfo;
struct RpClump;
struct RwObject;
class C2DEffectSAInterface;

class CBoundingBox
{
public:
    CVector vecBoundMin;
    CVector vecBoundMax;
    CVector vecBoundOffset;
    float   fRadius;
};

enum class eModelIdeFlag
{
    IS_ROAD,
    DRAW_LAST,
    ADDITIVE,
    IGNORE_LIGHTING,            // Used with animated objects
    NO_ZBUFFER_WRITE,
    DONT_RECEIVE_SHADOWS,
    IS_GLASS_TYPE_1,
    IS_GLASS_TYPE_2,
    IS_GARAGE_DOOR,
    IS_DAMAGABLE,
    IS_TREE,
    IS_PALM,
    DOES_NOT_COLLIDE_WITH_FLYER,
    IS_TAG,
    DISABLE_BACKFACE_CULLING,
    IS_BREAKABLE_STATUE,
    IS_CRANE,
};

enum class eModelInfoType : unsigned char
{
    ATOMIC = 1,
    TIME = 3,
    WEAPON = 4,
    CLUMP = 5,
    VEHICLE = 6,
    PED = 7,
    LOD_ATOMIC = 8,
    UNKNOWN = 9
};

enum eVehicleUpgradePosn
{
    VEHICLE_UPGRADE_POSN_BONET = 0,
    VEHICLE_UPGRADE_POSN_VENT,
    VEHICLE_UPGRADE_POSN_SPOILER,
    VEHICLE_UPGRADE_POSN_SIDESKIRT,
    VEHICLE_UPGRADE_POSN_FRONT_BULLBARS,
    VEHICLE_UPGRADE_POSN_REAR_BULLBARS,
    VEHICLE_UPGRADE_POSN_HEADLIGHTS,
    VEHICLE_UPGRADE_POSN_ROOF,
    VEHICLE_UPGRADE_POSN_NITRO,
    VEHICLE_UPGRADE_POSN_HYDRAULICS,
    VEHICLE_UPGRADE_POSN_STEREO,
    VEHICLE_UPGRADE_POSN_UNKNOWN,
    VEHICLE_UPGRADE_POSN_WHEELS,
    VEHICLE_UPGRADE_POSN_EXHAUST,
    VEHICLE_UPGRADE_POSN_FRONT_BUMPER,
    VEHICLE_UPGRADE_POSN_REAR_BUMPER,
    VEHICLE_UPGRADE_POSN_MISC,
};

enum class e2dEffectType : std::uint8_t
{
    LIGHT = 0,
    PARTICLE,
    ATTRACTOR,
    SUN_GLARE,
    FURNITURE,
    ENEX,
    ROADSIGN,
    TRIGGER_POINT,
    COVER_POINT,
    ESCALATOR,

    NONE,
};

enum class e2dCoronaFlashType : std::uint8_t
{
    DEFAULT = 0,
    RANDOM,
    RANDOM_WHEN_WET,
    ANIM_SPEED_4X,
    ANIM_SPEED_2X,
    ANIM_SPEED_1X,
    WARNLIGHT, // Used on model nt_roadblockci
    TRAFFICLIGHT,
    TRAINCROSSING,
    UNUSED,
    ONLY_RAIN,
    ON5_OFF5,
    ON6_OFF4,
    ON4_OFF6,
};

enum class e2dEffectProperty
{
    // light properties
    FAR_CLIP_DISTANCE = 0,
    LIGHT_RANGE,
    CORONA_SIZE,
    SHADOW_SIZE,
    SHADOW_MULT,
    FLASH_TYPE,
    CORONA_REFLECTION,
    FLARE_TYPE,
    SHADOW_DISTANCE,
    OFFSET_X,
    OFFSET_Y,
    OFFSET_Z,
    COLOR,
    CORONA_NAME,
    SHADOW_NAME,
    FLAGS, // for light & roadsign

    // particle properties
    PRT_NAME,

    // roadsign properties
    SIZE_X,
    SIZE_Y,
    ROT_X,
    ROT_Y,
    ROT_Z,
    TEXT,
    TEXT_2,
    TEXT_3,
    TEXT_4,

    // escalator properties
    BOTTOM_X,
    BOTTOM_Y,
    BOTTOM_Z,
    TOP_X,
    TOP_Y,
    TOP_Z,
    END_X,
    END_Y,
    END_Z,
    DIRECTION,
};

enum class e2dAttractorType : std::int8_t
{
    UNDEFINED = -1,
    ATM = 0,
    SEAT,
    STOP,
    PIZZA,
    SHELTER,
    TRIGGER_SCRIPT,
    LOOK_AT,
    SCRIPTED,
    PARK,
    STEP,
};

struct SVehicleSupportedUpgrades
{
    SVehicleSupportedUpgrades() { Reset(); }
    void Reset()
    {
        m_bBonnet = false;
        m_bBonnet_Left = false;
        m_bBonnet_Left_dam = false;
        m_bBonnet_Right = false;
        m_bBonnet_Right_dam = false;
        m_bSpoiler = false;
        m_bSpoiler_dam = false;
        m_bVent = false;
        m_bSideSkirt_Right = false;
        m_bSideSkirt_Left = false;
        m_bFrontBullbars = false;
        m_bRearBullbars = false;
        m_bLamps = false;
        m_bLamps_dam = false;
        m_bRoof = false;
        m_bExhaust = false;
        m_bFrontBumper = false;
        m_bRearBumper = false;
        m_bMisc = false;
        m_bInitialised = false;
    }
    bool m_bBonnet;
    bool m_bBonnet_Left;
    bool m_bBonnet_Left_dam;
    bool m_bBonnet_Right;
    bool m_bBonnet_Right_dam;
    bool m_bSpoiler;
    bool m_bSpoiler_dam;
    bool m_bVent;
    bool m_bSideSkirt_Right;
    bool m_bSideSkirt_Left;
    bool m_bFrontBullbars;
    bool m_bRearBullbars;
    bool m_bLamps;
    bool m_bLamps_dam;
    bool m_bRoof;
    bool m_bExhaust;
    bool m_bFrontBumper;
    bool m_bRearBumper;
    bool m_bMisc;
    bool m_bInitialised;
};
class CModelInfo
{
public:
    virtual class CBaseModelInfoSAInterface* GetInterface() = 0;

    virtual eModelInfoType GetModelType() = 0;
    virtual DWORD          GetModel() = 0;
    virtual bool           IsPlayerModel() = 0;
    virtual bool           IsBoat() = 0;
    virtual bool           IsCar() = 0;
    virtual bool           IsTrain() = 0;
    virtual bool           IsHeli() = 0;
    virtual bool           IsPlane() = 0;
    virtual bool           IsBike() = 0;
    virtual bool           IsFakePlane() = 0;
    virtual bool           IsMonsterTruck() = 0;
    virtual bool           IsQuadBike() = 0;
    virtual bool           IsBmx() = 0;
    virtual bool           IsTrailer() = 0;
    virtual bool           IsVehicle() const = 0;

    virtual char* GetNameIfVehicle() = 0;

    virtual BYTE           GetVehicleType() = 0;
    virtual void           Request(EModelRequestType requestType, const char* szTag /* = NULL*/) = 0;
    virtual bool           IsLoaded() = 0;
    virtual unsigned short GetFlags() = 0;
    virtual unsigned short GetOriginalFlags() = 0;
    virtual void           SetFlags(unsigned short usFlags) = 0;
    virtual void           SetIdeFlags(unsigned int uiFlags) = 0;
    virtual bool           GetIdeFlag(eModelIdeFlag eFlag) = 0;
    virtual void           SetIdeFlag(eModelIdeFlag eFlag, bool bState) = 0;
    virtual CBoundingBox*  GetBoundingBox() = 0;
    virtual bool           IsValid() = 0;
    virtual bool           IsAllocatedInArchive() = 0;
    virtual unsigned short GetTextureDictionaryID() = 0;
    virtual void           SetTextureDictionaryID(unsigned short usTxdId) = 0;
    virtual void           ResetTextureDictionaryID() = 0;
    virtual float          GetLODDistance() = 0;
    virtual float          GetOriginalLODDistance() = 0;
    virtual void           SetLODDistance(float fDistance, bool bOverrideMaxDistance = false) = 0;
    virtual void           RestreamIPL() = 0;
    virtual bool           GetTime(char& hourOn, char& hourOff) = 0;
    virtual bool           SetTime(char hourOn, char hourOff) = 0;

    virtual void ModelAddRef(EModelRequestType requestType, const char* szTag /* = NULL*/) = 0;
    virtual void RemoveRef(bool bRemoveExtraGTARef = false) = 0;
    virtual int  GetRefCount() = 0;
    virtual bool ForceUnload() = 0;
    virtual bool UnloadUnused() = 0;
    virtual void DeallocateModel() = 0;

    virtual float GetDistanceFromCentreOfMassToBaseOfModel() = 0;

    virtual void SetAlphaTransparencyEnabled(bool bEnabled) = 0;
    virtual bool IsAlphaTransparencyEnabled() = 0;
    virtual void ResetAlphaTransparency() = 0;

    // ONLY use for CVehicleModelInfos
    virtual short        GetAvailableVehicleMod(unsigned short usSlot) = 0;
    virtual bool         IsUpgradeAvailable(eVehicleUpgradePosn posn) = 0;
    virtual void         SetCustomCarPlateText(const char* szText) = 0;
    virtual unsigned int GetNumRemaps() = 0;
    virtual void*        GetVehicleSuspensionData() = 0;
    virtual void*        SetVehicleSuspensionData(void* pSuspensionLines) = 0;
    virtual CVector      GetVehicleExhaustFumesPosition() = 0;
    virtual void         SetVehicleExhaustFumesPosition(const CVector& position) = 0;
    virtual CVector      GetVehicleDummyDefaultPosition(eVehicleDummies eDummy) = 0;
    virtual CVector      GetVehicleDummyPosition(eVehicleDummies eDummy) = 0;
    virtual bool         GetVehicleDummyPositions(std::array<CVector, VEHICLE_DUMMY_COUNT>& positions) const = 0;
    virtual void         SetVehicleDummyPosition(eVehicleDummies eDummy, const CVector& vecPosition) = 0;
    virtual void         ResetVehicleDummies(bool bRemoveFromDummiesMap) = 0;
    virtual float        GetVehicleWheelSize(eResizableVehicleWheelGroup eWheelGroup) = 0;
    virtual void         SetVehicleWheelSize(eResizableVehicleWheelGroup eWheelGroup, float fWheelSize) = 0;
    virtual void         ResetVehicleWheelSizes(std::pair<float, float>* defaultSizes = nullptr) = 0;

    // Init the supported upgrades structure
    virtual void InitialiseSupportedUpgrades(RpClump* pClump) = 0;

    // ONLY use for peds
    virtual void GetVoice(short* psVoiceType, short* psVoice) = 0;
    virtual void GetVoice(const char** pszVoiceType, const char** szVoice) = 0;
    virtual void SetVoice(short sVoiceType, short sVoice) = 0;
    virtual void SetVoice(const char* szVoiceType, const char* szVoice) = 0;

    // Custom collision related functions
    virtual bool SetCustomModel(RpClump* pClump) = 0;
    virtual void RestoreOriginalModel() = 0;
    virtual void SetColModel(CColModel* pColModel) = 0;
    virtual void RestoreColModel() = 0;

    // Increases the collision slot reference counter for this model
    virtual void AddColRef() = 0;

    // Decreases the collision slot reference counter for this model
    virtual void RemoveColRef() = 0;

    // Call this to make sure the custom vehicle models are being used after a load.
    virtual void      MakeCustomModel() = 0;
    virtual RwObject* GetRwObject() = 0;
    virtual void      MakePedModel(char* szTexture) = 0;
    virtual void      MakeObjectModel(unsigned short usBaseID) = 0;
    virtual void      MakeVehicleAutomobile(unsigned short usBaseID) = 0;
    virtual void      MakeTimedObjectModel(unsigned short usBaseID) = 0;
    virtual void      MakeClumpModel(unsigned short usBaseID) = 0;

    virtual SVehicleSupportedUpgrades GetVehicleSupportedUpgrades() = 0;
    virtual void                      ResetSupportedUpgrades() = 0;

    virtual void           SetObjectPropertiesGroup(unsigned short usObjectGroup) = 0;
    virtual unsigned short GetObjectPropertiesGroup() = 0;
    virtual void           RestoreObjectPropertiesGroup() = 0;

    // Vehicle towing functions
    virtual bool IsTowableBy(CModelInfo* towingModel) = 0;

    // 2dfx functions
    virtual C2DEffectSAInterface* Add2DFXEffect(const CVector& position, const e2dEffectType& type) = 0;
    virtual void                  Remove2DFX(C2DEffectSAInterface* effect, bool isCustom = false, bool decrementCounters = false) = 0;
    virtual bool                  Remove2DFXEffectAtIndex(std::uint32_t index, bool includeDefault = false) = 0;
    virtual bool                  RemoveAll2DFXEffects(bool includeDefault = false) = 0;

    virtual C2DEffectSAInterface* Get2DFXFromIndex(std::uint32_t index) = 0;
    virtual std::uint32_t         Get2DFXCount() const = 0;

    virtual void StoreDefault2DFXEffect(C2DEffectSAInterface* effect) = 0;
    virtual bool Reset2DFXEffects(bool removeCustomEffects = false) = 0;

    virtual unsigned int GetParentID() = 0;
    virtual bool         IsDynamic() = 0;
};
