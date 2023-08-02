// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/FirePdu.h>
#include "PDUs/WarfareFamily/GRILL_WarfareFamilyPDU.h"
#include "GRILL_FirePDU.generated.h"

USTRUCT(BlueprintType)
struct FFirePDU : public FWarfareFamilyPDU
{
	GENERATED_BODY()

	/** The index number of the fire mission */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		int32 FireMissionIndex;
	/** The entity id of the fired munition or expendable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		FEntityID MunitionEntityID;
	/**
	* The range that an entity's fire control system has assumed in computing
	* the fire control solution in meters. For systems where the range is unknown,
	* range shall be 0
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		float Range;
	/**
	* The velocity of the fired munition at the point whenn the issuing simulation
	* application intends the externally visable effects of the launch to first become
	* apparent represented in world coordinates in meters per second.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		FVector Velocity;
	/** The location from which the munition was launched in world coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		FVector EcefLocation;
	/**
	* The event ID generated by the firing entity to associate related firing and
	* detonation events
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		FEventID EventID;
	/**
	* Description of the detonation in the form of a munition descriptor, an explosion
	* descriptor, or an expendable descriptor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Fire")
		FBurstDescriptor BurstDescriptor;

	FFirePDU() : FWarfareFamilyPDU()
	{
		PduType = EPDUType::Fire;

		FireMissionIndex = 0;
		Range = 0.0;
		EcefLocation = FVector(0, 0, 0);
		Velocity = FVector(0, 0, 0);
	}

	virtual ~FFirePDU() {}

	void SetupFromOpenDIS(const DIS::FirePdu& FirePDUIn)
	{
		FWarfareFamilyPDU::SetupFromOpenDIS(FirePDUIn);

		// Fire PDU specifics
		//single vars
		FireMissionIndex = FirePDUIn.getFireMissionIndex();
		Range = FirePDUIn.getRange();

		//MunitionEntityID
		MunitionEntityID.Site = FirePDUIn.getMunitionID().getSite();
		MunitionEntityID.Application = FirePDUIn.getMunitionID().getApplication();
		MunitionEntityID.Entity = FirePDUIn.getMunitionID().getEntity();

		//velocity
		Velocity[0] = FirePDUIn.getVelocity().getX();
		Velocity[1] = FirePDUIn.getVelocity().getY();
		Velocity[2] = FirePDUIn.getVelocity().getZ();

		//location
		EcefLocation[0] = FirePDUIn.getLocationInWorldCoordinates().getX();
		EcefLocation[1] = FirePDUIn.getLocationInWorldCoordinates().getY();
		EcefLocation[2] = FirePDUIn.getLocationInWorldCoordinates().getZ();

		//event id
		EventID = FirePDUIn.getEventID();

		//burst descriptor
		BurstDescriptor.Warhead = FirePDUIn.getBurstDescriptor().getWarhead();
		BurstDescriptor.Fuse = FirePDUIn.getBurstDescriptor().getFuse();
		BurstDescriptor.Rate = FirePDUIn.getBurstDescriptor().getRate();
		BurstDescriptor.Quantity = FirePDUIn.getBurstDescriptor().getQuantity();
		BurstDescriptor.EntityType = FirePDUIn.getBurstDescriptor().getMunition();
	}

	void ToOpenDIS(DIS::FirePdu& FirePDUOut)
	{
		FWarfareFamilyPDU::ToOpenDIS(FirePDUOut);

		//Inherited PDU setup
		FirePDUOut.setFiringEntityID(FiringEntityID.ToOpenDIS());
		FirePDUOut.setTargetEntityID(TargetEntityID.ToOpenDIS());

		// Specific PDU setup
		FirePDUOut.setMunitionID(MunitionEntityID.ToOpenDIS());
		FirePDUOut.setFireMissionIndex(FireMissionIndex);
		FirePDUOut.setRange(Range);

		DIS::Vector3Float OutVelocity;
		OutVelocity.setX(Velocity.X);
		OutVelocity.setY(Velocity.Y);
		OutVelocity.setZ(Velocity.Z);
		FirePDUOut.setVelocity(OutVelocity);

		DIS::Vector3Double OutLocation;
		OutLocation.setX(EcefLocation[0]);
		OutLocation.setY(EcefLocation[1]);
		OutLocation.setZ(EcefLocation[2]);
		FirePDUOut.setLocationInWorldCoordinates(OutLocation);

		FirePDUOut.setEventID(EventID.ToOpenDIS());
		FirePDUOut.setBurstDescriptor(BurstDescriptor.ToOpenDIS());
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::FirePdu firePDU;

		ToOpenDIS(firePDU);
		firePDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};