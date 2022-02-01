// Fill out your copyright notice in the Description page of Project Settings.

#include "PDUProcessor.h"
#include "UDPSubsystem.h"

void UPDUProcessor::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(UUDPSubsystem::StaticClass());
	Super::Initialize(Collection);

	//Get the UDP Subsystem and bind to receiving UDP Bytes
	GetGameInstance()->GetSubsystem<UUDPSubsystem>()->OnReceivedBytes.AddDynamic(this, &UPDUProcessor::HandleOnReceivedUDPBytes);
}

void UPDUProcessor::Deinitialize()
{
	Super::Deinitialize();
}

void UPDUProcessor::HandleOnReceivedUDPBytes(const TArray<uint8>& Bytes, const FString& IPAddress)
{
	ProcessDISPacket(Bytes);
}

void UPDUProcessor::ProcessDISPacket(TArray<uint8> InData)
{
	int bytesArrayLength = InData.Num();

	if (bytesArrayLength < 1)
	{
		return;
	}

	DIS::Pdu* pdu = NULL;

	pdu = DIS::PduBank::GetStaticPDU(static_cast<DIS::PDUType>(InData[PDU_TYPE_POSITION]));
	if (pdu)
	{
		//For list of enums for PDU type refer to SISO-REF-010-2015, ANNEX A
		switch (static_cast<EPDUType>(pdu->getPduType()))
		{
		case EPDUType::EntityState:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FEntityStatePDU entityStatePDU = ConvertEntityStatePDUtoBPStruct(static_cast<DIS::EntityStatePdu*>(pdu));

			OnEntityStatePDUProcessed.Broadcast(entityStatePDU);

			break;
		}
		case EPDUType::Fire:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FFirePDU firePDU = ConvertFirePDUtoBPStruct(static_cast<DIS::FirePdu*>(pdu));

			OnFirePDUProcessed.Broadcast(firePDU);

			break;
		}
		case EPDUType::Detonation:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FDetonationPDU detPDU = ConvertDetonationPDUtoBPStruct(static_cast<DIS::DetonationPdu*>(pdu));

			OnDetonationPDUProcessed.Broadcast(detPDU);

			break;
		}
		case EPDUType::RemoveEntity:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FRemoveEntityPDU removeEntityPDU = ConvertRemoveEntityPDUtoBPStruct(static_cast<DIS::RemoveEntityPdu*>(pdu));

			OnRemoveEntityPDUProcessed.Broadcast(removeEntityPDU);

			break;
		}
		case EPDUType::Start_Resume:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FStartResumePDU StartResumePDU = ConvertStartResumePDUtoBPStruct(static_cast<DIS::StartResumePdu*>(pdu));

			OnStartResumePDUProcessed.Broadcast(StartResumePDU);

			break;
		}
		case EPDUType::Stop_Freeze:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FStopFreezePDU StopFreezePDU = ConvertStopFreezePDUtoBPStruct(static_cast<DIS::StopFreezePdu*>(pdu));

			OnStopFreezePDUProcessed.Broadcast(StopFreezePDU);

			break;
		}
		case EPDUType::EntityStateUpdate:
		{
			DIS::DataStream ds((char*)&InData[0], bytesArrayLength, BigEndian);
			pdu->unmarshal(ds);
			FEntityStateUpdatePDU entityStateUpdatePDU = ConvertEntityStateUpdatePDUtoBPStruct(static_cast<DIS::EntityStateUpdatePdu*>(pdu));

			OnEntityStateUpdatePDUProcessed.Broadcast(entityStateUpdatePDU);

			break;
		}
		default:
		{
			break;
		}
		}
	}
}

// TODO: Implement additional PDU conversions to bytes to support sending of more types.
void UPDUProcessor::ConvertESPDU2Bytes(int Exercise, FEntityStatePDU EntityStatePDUIn, TArray<uint8>& BytesOut)
{
	DIS::DataStream buffer(BigEndian);

	//protocol and exercise
	DIS::EntityStatePdu tempEntity;
	tempEntity.setProtocolVersion(6);
	tempEntity.setExerciseID(Exercise);

	//entity id
	DIS::EntityID tempID;
	tempID.setSite(EntityStatePDUIn.EntityID.Site);
	tempID.setApplication(EntityStatePDUIn.EntityID.Application);
	tempID.setEntity(EntityStatePDUIn.EntityID.Entity);
	tempEntity.setEntityID(tempID);

	//entity type
	DIS::EntityType tempType;
	tempType.setCategory(EntityStatePDUIn.EntityType.Category);
	tempType.setCountry(EntityStatePDUIn.EntityType.Country);
	tempType.setDomain(EntityStatePDUIn.EntityType.Domain);
	tempType.setEntityKind(EntityStatePDUIn.EntityType.EntityKind);
	tempType.setExtra(EntityStatePDUIn.EntityType.Extra);
	tempType.setSpecific(EntityStatePDUIn.EntityType.Specific);
	tempType.setSubcategory(EntityStatePDUIn.EntityType.Subcategory);
	tempEntity.setEntityType(tempType);

	//dead reckoning
	DIS::DeadReckoningParameter drp;
	drp.setDeadReckoningAlgorithm(4);
	DIS::Vector3Float tempFloat;
	tempFloat.setX(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.X);
	tempFloat.setY(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.Y);
	tempFloat.setZ(EntityStatePDUIn.DeadReckoningParameters.EntityAngularVelocity.Z);
	drp.setEntityAngularVelocity(tempFloat);
	tempFloat.setX(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.X);
	tempFloat.setY(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.Y);
	tempFloat.setZ(EntityStatePDUIn.DeadReckoningParameters.EntityLinearAcceleration.Z);
	drp.setEntityLinearAcceleration(tempFloat);
	tempEntity.setDeadReckoningParameters(drp);

	// TODO: location FIX TO USE DOUBLE LATER, USING FLOAT FOR SIMPLE TESTING
	DIS::Vector3Double tempDouble;
	tempDouble.setX(EntityStatePDUIn.EntityLocation[0]);
	tempDouble.setY(EntityStatePDUIn.EntityLocation[1]);
	tempDouble.setZ(EntityStatePDUIn.EntityLocation[2]);
	tempEntity.setEntityLocation(tempDouble);

	//rotation
	DIS::Orientation tempOri;
	tempOri.setPhi(0.0);
	tempOri.setPsi(0.0);
	tempOri.setTheta(0.0);
	tempEntity.setEntityOrientation(tempOri);

	//marking
	DIS::Marking tempMarking;
	tempMarking.setCharacterSet(1);
	tempMarking.setByStringCharacters(TCHAR_TO_ANSI(*EntityStatePDUIn.Marking));
	tempEntity.setMarking(tempMarking);

	//marshal
	tempEntity.marshal(buffer);
	TArray<uint8> tempBytes;
	tempBytes.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		tempBytes[i] = buffer[i];
	}
	buffer.clear();

	BytesOut = tempBytes;
}

FEntityStatePDU UPDUProcessor::ConvertEntityStatePDUtoBPStruct(DIS::EntityStatePdu* EntityStatePDUIn)
{
	FEntityStatePDU entityStatePDU;

	DIS::Vector3Double& position = EntityStatePDUIn->getEntityLocation();
	DIS::Orientation& rotation = EntityStatePDUIn->getEntityOrientation();
	const DIS::EntityID EntityID = EntityStatePDUIn->getEntityID();
	const DIS::EntityType EntityType = EntityStatePDUIn->getEntityType();

	//pure since unsupported in BP
	entityStatePDU.EntityLocationDouble[0] = position.getX();
	entityStatePDU.EntityLocationDouble[1] = position.getY();
	entityStatePDU.EntityLocationDouble[2] = position.getZ();

	//entity id
	entityStatePDU.EntityID.Site = EntityID.getSite();
	entityStatePDU.EntityID.Application = EntityID.getApplication();
	entityStatePDU.EntityID.Entity = EntityID.getEntity();

	//location
	entityStatePDU.EntityLocation[0] = position.getX();
	entityStatePDU.EntityLocation[1] = position.getY();
	entityStatePDU.EntityLocation[2] = position.getZ();

	//rotation
	entityStatePDU.EntityOrientation.Yaw = rotation.getPsi();
	entityStatePDU.EntityOrientation.Roll = rotation.getPhi();
	entityStatePDU.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	entityStatePDU.EntityLinearVelocity[0] = EntityStatePDUIn->getEntityLinearVelocity().getX();
	entityStatePDU.EntityLinearVelocity[1] = EntityStatePDUIn->getEntityLinearVelocity().getY();
	entityStatePDU.EntityLinearVelocity[2] = EntityStatePDUIn->getEntityLinearVelocity().getZ();

	//Dead reckoning
	entityStatePDU.DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUIn->getDeadReckoningParameters().getDeadReckoningAlgorithm();
	// TODO: figure out how to get the character buffer of 15 8bits and put it into tarray of 15 elements each with 8bits
	//returnStruct.DeadReckoningParameters.OtherParameters = espdu.getDeadReckoningParameters().getOtherParameters();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getX();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getY();
	entityStatePDU.DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getX();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getY();
	entityStatePDU.DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getZ();

	//single vars
	entityStatePDU.ForceID = static_cast<EForceID>(EntityStatePDUIn->getForceId());
	entityStatePDU.Marking = FString(EntityStatePDUIn->getMarking().getCharacters());
	//entityStatePDU.PduType = EntityStatePDUIn->getPduType();
	entityStatePDU.EntityAppearance = EntityStatePDUIn->getEntityAppearance();
	entityStatePDU.NumberOfArticulationParameters = EntityStatePDUIn->getNumberOfArticulationParameters();
	entityStatePDU.Capabilities = EntityStatePDUIn->getCapabilities();

	//Entity type
	entityStatePDU.EntityType.EntityKind = EntityType.getEntityKind();
	entityStatePDU.EntityType.Domain = EntityType.getDomain();
	entityStatePDU.EntityType.Country = EntityType.getCountry();
	entityStatePDU.EntityType.Category = EntityType.getCategory();
	entityStatePDU.EntityType.Subcategory = EntityType.getSubcategory();
	entityStatePDU.EntityType.Specific = EntityType.getSpecific();
	entityStatePDU.EntityType.Extra = EntityType.getExtra();

	return entityStatePDU;
}

FEntityStateUpdatePDU UPDUProcessor::ConvertEntityStateUpdatePDUtoBPStruct(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn) 
{
	FEntityStateUpdatePDU entityStateUpdatePDU;

	DIS::Vector3Double& position = EntityStateUpdatePDUIn->getEntityLocation();
	DIS::Orientation& rotation = EntityStateUpdatePDUIn->getEntityOrientation();
	const DIS::EntityID EntityID = EntityStateUpdatePDUIn->getEntityID();

	//pure since unsupported in BP
	entityStateUpdatePDU.EntityLocationDouble[0] = position.getX();
	entityStateUpdatePDU.EntityLocationDouble[1] = position.getY();
	entityStateUpdatePDU.EntityLocationDouble[2] = position.getZ();

	//entity id
	entityStateUpdatePDU.EntityID.Site = EntityID.getSite();
	entityStateUpdatePDU.EntityID.Application = EntityID.getApplication();
	entityStateUpdatePDU.EntityID.Entity = EntityID.getEntity();

	//location
	entityStateUpdatePDU.EntityLocation[0] = position.getX();
	entityStateUpdatePDU.EntityLocation[1] = position.getY();
	entityStateUpdatePDU.EntityLocation[2] = position.getZ();

	//rotation
	entityStateUpdatePDU.EntityOrientation.Yaw = rotation.getPsi();
	entityStateUpdatePDU.EntityOrientation.Roll = rotation.getPhi();
	entityStateUpdatePDU.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	entityStateUpdatePDU.EntityLinearVelocity[0] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getX();
	entityStateUpdatePDU.EntityLinearVelocity[1] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getY();
	entityStateUpdatePDU.EntityLinearVelocity[2] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getZ();

	entityStateUpdatePDU.Padding = EntityStateUpdatePDUIn->getPadding();
	entityStateUpdatePDU.Padding1 = EntityStateUpdatePDUIn->getPadding1();
	entityStateUpdatePDU.EntityAppearance = EntityStateUpdatePDUIn->getEntityAppearance();
	entityStateUpdatePDU.NumberOfArticulationParameters = EntityStateUpdatePDUIn->getNumberOfArticulationParameters();

	return entityStateUpdatePDU;
}

FFirePDU UPDUProcessor::ConvertFirePDUtoBPStruct(DIS::FirePdu* FirePDUIn)
{
	FFirePDU firePDU;

	//single vars
	firePDU.FireMissionIndex = FirePDUIn->getFireMissionIndex();
	firePDU.Range = FirePDUIn->getRange();

	//MunitionEntityID
	firePDU.MunitionEntityID.Site = FirePDUIn->getMunitionID().getSite();
	firePDU.MunitionEntityID.Application = FirePDUIn->getMunitionID().getApplication();
	firePDU.MunitionEntityID.Entity = FirePDUIn->getMunitionID().getEntity();

	//velocity
	firePDU.Velocity[0] = FirePDUIn->getVelocity().getX();
	firePDU.Velocity[1] = FirePDUIn->getVelocity().getY();
	firePDU.Velocity[2] = FirePDUIn->getVelocity().getZ();

	//location
	firePDU.Location[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	firePDU.Location[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	firePDU.Location[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	firePDU.LocationDouble[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	firePDU.LocationDouble[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	firePDU.LocationDouble[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//event id
	firePDU.EventID.Site = FirePDUIn->getEventID().getSite();
	firePDU.EventID.Application = FirePDUIn->getEventID().getApplication();
	firePDU.EventID.EventID = FirePDUIn->getEventID().getEventNumber();

	//burst descriptor
	firePDU.BurstDescriptor.Warhead = FirePDUIn->getBurstDescriptor().getWarhead();
	firePDU.BurstDescriptor.Fuse = FirePDUIn->getBurstDescriptor().getFuse();
	firePDU.BurstDescriptor.Rate = FirePDUIn->getBurstDescriptor().getRate();
	firePDU.BurstDescriptor.Quantity = FirePDUIn->getBurstDescriptor().getQuantity();
	firePDU.BurstDescriptor.EntityType.EntityKind = FirePDUIn->getBurstDescriptor().getMunition().getEntityKind();
	firePDU.BurstDescriptor.EntityType.Domain = FirePDUIn->getBurstDescriptor().getMunition().getDomain();
	firePDU.BurstDescriptor.EntityType.Country = FirePDUIn->getBurstDescriptor().getMunition().getCountry();
	firePDU.BurstDescriptor.EntityType.Category = FirePDUIn->getBurstDescriptor().getMunition().getCategory();
	firePDU.BurstDescriptor.EntityType.Subcategory = FirePDUIn->getBurstDescriptor().getMunition().getSubcategory();
	firePDU.BurstDescriptor.EntityType.Specific = FirePDUIn->getBurstDescriptor().getMunition().getSpecific();
	firePDU.BurstDescriptor.EntityType.Extra = FirePDUIn->getBurstDescriptor().getMunition().getExtra();

	return firePDU;
}

FDetonationPDU UPDUProcessor::ConvertDetonationPDUtoBPStruct(DIS::DetonationPdu* DetPDUIn)
{
	FDetonationPDU detonationPDU;

	//MunitionEntityID
	detonationPDU.MunitionEntityID.Site = DetPDUIn->getMunitionID().getSite();
	detonationPDU.MunitionEntityID.Application = DetPDUIn->getMunitionID().getApplication();
	detonationPDU.MunitionEntityID.Entity = DetPDUIn->getMunitionID().getEntity();

	//event id
	detonationPDU.EventID.Site = DetPDUIn->getEventID().getSite();
	detonationPDU.EventID.Application = DetPDUIn->getEventID().getApplication();
	detonationPDU.EventID.EventID = DetPDUIn->getEventID().getEventNumber();

	//velocity
	detonationPDU.Velocity[0] = DetPDUIn->getVelocity().getX();
	detonationPDU.Velocity[1] = DetPDUIn->getVelocity().getY();
	detonationPDU.Velocity[2] = DetPDUIn->getVelocity().getZ();

	//location
	detonationPDU.Location[0] = DetPDUIn->getLocationInWorldCoordinates().getX();
	detonationPDU.Location[1] = DetPDUIn->getLocationInWorldCoordinates().getY();
	detonationPDU.Location[2] = DetPDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	detonationPDU.LocationDouble[0] = DetPDUIn->getLocationInWorldCoordinates().getX();
	detonationPDU.LocationDouble[1] = DetPDUIn->getLocationInWorldCoordinates().getY();
	detonationPDU.LocationDouble[2] = DetPDUIn->getLocationInWorldCoordinates().getZ();

	//location
	detonationPDU.LocationInEntityCoords[0] = DetPDUIn->getLocationInEntityCoordinates().getX();
	detonationPDU.LocationInEntityCoords[1] = DetPDUIn->getLocationInEntityCoordinates().getY();
	detonationPDU.LocationInEntityCoords[2] = DetPDUIn->getLocationInEntityCoordinates().getZ();

	//burst descriptor
	detonationPDU.BurstDescriptor.Warhead = DetPDUIn->getBurstDescriptor().getWarhead();
	detonationPDU.BurstDescriptor.Fuse = DetPDUIn->getBurstDescriptor().getFuse();
	detonationPDU.BurstDescriptor.Rate = DetPDUIn->getBurstDescriptor().getRate();
	detonationPDU.BurstDescriptor.Quantity = DetPDUIn->getBurstDescriptor().getQuantity();
	detonationPDU.BurstDescriptor.EntityType.EntityKind = DetPDUIn->getBurstDescriptor().getMunition().getEntityKind();
	detonationPDU.BurstDescriptor.EntityType.Domain = DetPDUIn->getBurstDescriptor().getMunition().getDomain();
	detonationPDU.BurstDescriptor.EntityType.Country = DetPDUIn->getBurstDescriptor().getMunition().getCountry();
	detonationPDU.BurstDescriptor.EntityType.Category = DetPDUIn->getBurstDescriptor().getMunition().getCategory();
	detonationPDU.BurstDescriptor.EntityType.Subcategory = DetPDUIn->getBurstDescriptor().getMunition().getSubcategory();
	detonationPDU.BurstDescriptor.EntityType.Specific = DetPDUIn->getBurstDescriptor().getMunition().getSpecific();
	detonationPDU.BurstDescriptor.EntityType.Extra = DetPDUIn->getBurstDescriptor().getMunition().getExtra();

	//single vars
	detonationPDU.DetonationResult = DetPDUIn->getDetonationResult();
	detonationPDU.NumberOfArticulationParameters = DetPDUIn->getNumberOfArticulationParameters();
	detonationPDU.Pad = DetPDUIn->getPad();

	return detonationPDU;
}

FRemoveEntityPDU UPDUProcessor::ConvertRemoveEntityPDUtoBPStruct(DIS::RemoveEntityPdu* RemovePDUIn)
{
	FRemoveEntityPDU removeEntityPDU;

	removeEntityPDU.OriginatingEntityID.Site = RemovePDUIn->getOriginatingEntityID().getSite();
	removeEntityPDU.OriginatingEntityID.Application = RemovePDUIn->getOriginatingEntityID().getApplication();
	removeEntityPDU.OriginatingEntityID.Entity = RemovePDUIn->getOriginatingEntityID().getEntity();
	removeEntityPDU.ReceivingEntityID.Site = RemovePDUIn->getReceivingEntityID().getSite();
	removeEntityPDU.ReceivingEntityID.Application = RemovePDUIn->getReceivingEntityID().getApplication();
	removeEntityPDU.ReceivingEntityID.Entity = RemovePDUIn->getReceivingEntityID().getEntity();
	removeEntityPDU.RequestID = RemovePDUIn->getRequestID();

	return removeEntityPDU;
}

FStartResumePDU UPDUProcessor::ConvertStartResumePDUtoBPStruct(DIS::StartResumePdu* StartResumePDUIn) 
{
	FStartResumePDU startResumePDU;

	DIS::ClockTime RealWorldTime = StartResumePDUIn->getRealWorldTime();
	DIS::ClockTime SimulationTime = StartResumePDUIn->getRealWorldTime();

	startResumePDU.RealWorldTime.Hour = RealWorldTime.getHour();
	startResumePDU.RealWorldTime.TimePastHour = RealWorldTime.getTimePastHour();

	startResumePDU.SimulationTime.Hour = SimulationTime.getHour();
	startResumePDU.SimulationTime.TimePastHour = SimulationTime.getTimePastHour();

	startResumePDU.RequestID = StartResumePDUIn->getRequestID();

	return startResumePDU;
}

FStopFreezePDU UPDUProcessor::ConvertStopFreezePDUtoBPStruct(DIS::StopFreezePdu* StopFreezePDUIn)
{
	FStopFreezePDU stopFreezePDU;

	DIS::ClockTime RealWorldTime = StopFreezePDUIn->getRealWorldTime();
	
	stopFreezePDU.RealWorldTime.Hour = RealWorldTime.getHour();
	stopFreezePDU.RealWorldTime.TimePastHour = RealWorldTime.getTimePastHour();

	stopFreezePDU.Reason = static_cast<EReason>(StopFreezePDUIn->getReason());
	stopFreezePDU.FrozenBehavior = StopFreezePDUIn->getFrozenBehavior();
	stopFreezePDU.Padding = StopFreezePDUIn->getPadding1();
	stopFreezePDU.RequestID = StopFreezePDUIn->getRequestID();

	return stopFreezePDU;
}