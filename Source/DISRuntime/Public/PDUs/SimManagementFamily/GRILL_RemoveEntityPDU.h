// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/RemoveEntityPdu.h>
#include "PDUs/SimManagementFamily/GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_RemoveEntityPDU.generated.h"

USTRUCT(BlueprintType)
struct FRemoveEntityPDU : public FSimulationManagementFamilyPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FRemoveEntityPDU() : FSimulationManagementFamilyPDU()
	{
		PduType = EPDUType::RemoveEntity;

		RequestID = 0;
	}

	virtual ~FRemoveEntityPDU() {}

	void SetupFromOpenDIS(DIS::RemoveEntityPdu* RemoveEntityPDUIn)
	{
		FSimulationManagementFamilyPDU::SetupFromOpenDIS(RemoveEntityPDUIn);

		RequestID = RemoveEntityPDUIn->getRequestID();
	}

	void ToOpenDIS(DIS::RemoveEntityPdu& RemoveEntityPDUOut)
	{
		FSimulationManagementFamilyPDU::ToOpenDIS(RemoveEntityPDUOut);

		// Remove entity specific PDU setup
		RemoveEntityPDUOut.setRequestID(RequestID);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::RemoveEntityPdu removeEntityPDU;

		ToOpenDIS(removeEntityPDU);
		removeEntityPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};