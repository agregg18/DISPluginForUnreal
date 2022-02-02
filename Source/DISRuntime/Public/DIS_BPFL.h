// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <glm/gtx/quaternion.hpp>
#include "CoreMinimal.h"
#include "DISEnumsAndStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeoReferencingSystem.h"
#include "DIS_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class DISRUNTIME_API UDIS_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * Creates a 4x4 rotation matrix around the given axis of rotation rotating by Theta degrees
	 * @param AxisVector A 3x1 vector representing the axis of rotation
	 * @param ThetaRadians The amount to rotate given in radians
	 * @param OutRotationMatrix
	 */
	static void CreateRotationMatrix(FVector AxisVector, float ThetaRadians, FMatrix& OutRotationMatrix);

	/**
	 * Creates a 3x3 rotation matrix around the given axis of rotation rotating by Theta degrees
	 * @param AxisVector A 3x1 vector representing the axis of rotation
	 * @param ThetaRadians The amount to rotate given in radians
	 * @param OutRotationMatrix
	 */
	static void CreateRotationMatrix(glm::dvec3 AxisVector, double ThetaRadians, glm::dmat3x3& OutRotationMatrix);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param HeadingDegrees The degrees from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param NorthEastDownVectors The vectors pointing to the North, to the East, and toward the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 * @param NorthEastDownVectors The vectors pointing to the North, to the East, and toward the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyRollToNorthEastDownVector(const float RollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ);

public:
	/**
	 * Creates a 4x4 n^x matrix used for creating a rotation matrix
	 * @param NVector A 3x1 vector representing the axis of rotation
	 */
	static FMatrix CreateNCrossXMatrix(FVector NVector);

	/**
	 * Creates a 3x3 n^x matrix used for creating a rotation matrix
	 * @param NVector A 3x1 vector representing the axis of rotation
	 */
	static glm::dmat3x3 CreateNCrossXMatrix(glm::dvec3 NVector);

	/**
	 * Converts DIS X, Y, Z coordinates (ECEF) to Latitude, Longitude, and Height (LLH) all in double (64-bit) precision
	 * @param Ecef The ECEF location
	 * @param OutLatLonHeightDegreesMeters The converted latitude in degrees, longitude in degrees, and height in meters
	 */
	static void CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedDouble Ecef, FLatLonHeightDouble& OutLatLonHeightDegreesMeters);

	/**
	 * Converts DIS X, Y, Z coordinates (ECEF) to Latitude, Longitude, and Height (LLH) all in double (32-bit) precision
	 * @param Ecef The ECEF location
	 * @param OutLatLonHeightDegreesMeters The converted latitude in degrees, longitude in degrees, and height in meters
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedFloat Ecef, FLatLonHeightFloat& OutLatLonHeightDegreesMeters);

	/**
	 * Converts Latitude, Longitude, and Height (LLH) to DIS X, Y, Z coordinates (ECEF) all in double (64-bit) precision
	 * @param LatLonHeightDegreesMeters The latitude in degrees, longitude in degrees, and height in meters
	 * @param OutEcef The converted ECEF location
	 */
	static void CalculateEcefXYZFromLatLonHeight(const FLatLonHeightDouble LatLonHeightDegreesMeters, FEarthCenteredEarthFixedDouble& OutEcef);

	/**
	 * Converts Latitude, Longitude, and Height (LLH) to DIS X, Y, Z coordinates (ECEF) all in floating point (32-bit) precision
	 * @param LatLonHeightDegreesMeters The latitude in degrees, longitude in degrees, and height in meters
	 * @param OutECEF The converted ECEF location in double
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateEcefXYZFromLatLonHeight(const FLatLonHeightFloat LatLonHeightDegreesMeters, FEarthCenteredEarthFixedFloat& OutECEF);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaRadians The desired amount to rotation in radians
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
		static void RotateVectorAroundAxisByRadians(glm::dvec3 VectorToRotate, double ThetaRadians, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaRadians The desired amount to rotation in radians
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void RotateVectorAroundAxisByRadians(FVector VectorToRotate, float ThetaRadians, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta degrees
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaDegrees The desired amount to rotation in degrees
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
		static void RotateVectorAroundAxisByDegrees(glm::dvec3 VectorToRotate, float ThetaDegrees, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta degrees
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaDegrees The desired amount to rotation in degrees
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void RotateVectorAroundAxisByDegrees(FVector VectorToRotate, float ThetaDegrees, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Applies the given heading, pitch, and roll in degrees to the local East North Down vectors
	 * @param HeadingPitchRollDegrees The degrees from North of the facing direction (heading), the degrees rotated about the local Y axis (pitch), and the degrees rotated about the local X axis (roll)
	 * @param NorthEastDownVectors The vectors pointing to the North, to the East, and toward the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (down) vector with the heading and pitch applied
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void ApplyHeadingPitchRollToNorthEastDownVector(const FHeadingPitchRoll HeadingPitchRollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Calculates the East, North, and Up vectors at given latitude and longitude.
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param OutNorthEastDownVectors The local vectors pointing North, pointing East, and toward the center of the Earth at the given latitude and longitude
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees, FNorthEastDown& OutNorthEastDownVectors);

	/**
	 * Calculates the East, North, and Up vectors at given latitude and longitude.
	 * @param NorthEastDownVectors The vectors pointing to the North, to the East, and toward the center of the Earth
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees	 
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateLatLongFromNorthEastDownVectors(FNorthEastDown NorthEastDownVectors, float& LatitudeDegrees, float& LongitudeDegrees);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingPitchRollDegrees The degrees from North of the facing direction (heading), the degrees rotated about the local Y axis (pitch), and the degrees rotated about the local X axis (roll)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiThetaPhiDegrees The rotation about the Z axis in degrees (Psi),the rotation about the Y axis in degrees (Theta), the rotation about the X axis in degrees (Phi)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingPitchRollRadians The radians from North of the facing direction (heading), the radians rotated about the local Y axis (pitch), and the radians rotated about the local X axis (roll)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiThetaPhiRadians The rotation about the Z axis in radians (Psi),the rotation about the Y axis in radians (Theta), the rotation about the X axis in radians (Phi)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingPitchRollDegrees The degrees from North of the facing direction (heading), the degrees rotated about the local Y axis (pitch), and the degrees rotated about the local X axis (roll)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiThetaPhiRadians The rotation about the Z axis in radians (Psi),the rotation about the Y axis in radians (Theta), the rotation about the X axis in radians (Phi)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingPitchRollRadians The radians from North of the facing direction (heading), the radians rotated about the local Y axis (pitch), and the radians rotated about the local X axis (roll)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiThetaPhiDegrees The rotation about the Z axis in degrees (Psi),the rotation about the Y axis in degrees (Theta), the rotation about the X axis in degrees (Phi)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiThetaPhiDegrees The rotation about the Z axis in degrees (Psi),the rotation about the Y axis in degrees (Theta), the rotation about the X axis in degrees (Phi)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingPitchRollDegrees The degrees from North of the facing direction (heading), the degrees rotated about the local Y axis (pitch), and the degrees rotated about the local X axis (roll)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiThetaPhiRadians The rotation about the Z axis in radians (Psi),the rotation about the Y axis in radians (Theta), the rotation about the X axis in radians (Phi)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingPitchRollRadians The radians from North of the facing direction (heading), the radians rotated about the local Y axis (pitch), and the radians rotated about the local X axis (roll)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiThetaPhiRadians The rotation about the Z axis in radians (Psi),the rotation about the Y axis in radians (Theta), the rotation about the X axis in radians (Phi)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingPitchRollDegrees The degrees from North of the facing direction (heading), the degrees rotated about the local Y axis (pitch), and the degrees rotated about the local X axis (roll)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiThetaPhiDegrees The rotation about the Z axis in degrees (Psi),the rotation about the Y axis in degrees (Theta), the rotation about the X axis in degrees (Phi)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingPitchRollRadians The radians from North of the facing direction (heading), the radians rotated about the local Y axis (pitch), and the radians rotated about the local X axis (roll)
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians);

	/**
	 * Calculate the ECEF location of the given UE location
	 * @param UELocation The UE location to convert to ECEF.
	 * @param GeoReferencingSystem The GeoReferencing Subsystem reference.
	 * @param ECEF The ECEF location of the given UE location.
	*/
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateEcefXYZFromUnrealLocation(const FVector UELocation, AGeoReferencingSystem* GeoReferencingSystem, FEarthCenteredEarthFixedFloat& ECEF);

	/**
	 * Calculate the latitude in degrees, longitude in degrees, and height in meters of the given UE location
	 * @param UELocation The UE location to convert to latitude, longitude, height.
	 * @param GeoReferencingSystem The GeoReferencing Subsystem reference.
	 * @param LatLonHeightDegreesMeters The latitude in degrees, longitude in degrees, and height in meters of the given UE location.
	*/
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void CalculateLatLonHeightFromUnrealLocation(const FVector UELocation, AGeoReferencingSystem* GeoReferencingSystem, FLatLonHeightFloat& LatLonHeightDegreesMeters);

	/**
	 * Get Unreal rotation from a DIS entity state PDU
	 * @param EntityStatePdu The DIS PDU struct indicating the current state of the DIS entity
	 * @param GeoReferencingSystem The GeoReferencing Subsystem reference.
	 * @param EntityRotation The Heading (yaw), Pitch, and Roll calculated from the given entity state
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void GetUnrealRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FRotator& EntityRotation);

	/**
	 * Gets the unreal X, Y, Z coordinates of the entity from the given ECEF values in the DIS entity state pdu. Values returned change depending on if GeoReferencing Subsystem is set to Flat Earth or Round Earth.
	 * @param EntityStatePdu The DIS PDU struct indicating the current state of the DIS entity
	 * @param GeoReferencingSystem The GeoReferencing Subsystem reference.
	 * @param EntityLocation The resulting Unreal XYZ location of the entity in Unreal units
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void GetEntityUnrealLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& EntityLocation);

	/**
	 * Gets the unreal X, Y, Z coordinates and rotation from a DIS entity state PDU. Location values returned change depending on if GeoReferencing Subsystem is set to Flat Earth or Round Earth.
	 * @param EntityStatePdu The DIS PDU struct indicating the current state of the DIS entity
	 * @param GeoReferencingSystem The GeoReferencing Subsystem reference.
	 * @param EntityLocation The location of the entity as a vector where X is Latitude, Y is Longitude, and Z is Height
	 * @param EntityRotation The desired Yaw, Pitch, and Roll calculated from the given entity state
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
		static void GetEntityUnrealLocationAndOrientation(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& EntityLocation, FRotator& EntityRotation);

	/**
	 * Get the East, North, and Up vectors from the North, East, and Down vector struct
	 * @param NorthEastDownVectors The North, East, and Down vectors representing the current orientation
	 * @param EastNorthUpVectors The resulting East, North, and Up vectors representing the current orientation
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
	static void GetEastNorthUpVectorsFromNorthEastDownVectors(FNorthEastDown NorthEastDownVectors, FEastNorthUp& EastNorthUpVectors);

	/**
	 * Get the North, East, and Down vectors from the East, North, and Up vector struct
	 * @param EastNorthUpVectors The East, North, and Up vectors representing the current orientation
	 * @param NorthEastDownVectors The resulting North, East, and Down vectors representing the current orientation
	 */
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Unit Conversions")
	static void GetNorthEastDownVectorsFromEastNorthUpVectors(FEastNorthUp EastNorthUpVectors, FNorthEastDown& NorthEastDownVectors);

	/**
	 * Convert between North, East, Down and East, North, Up orientation representation using double values
	 * @param StartingVectors The matrix representation of the starting vectors (each vector is a column)
	 * @return The resulting conversion of swapping the first two vectors and negating the last
	 */
	static glm::dmat3 ConvertNedAndEnu(glm::dmat3 StartingVectors);

	/**
	 * Convert between North, East, Down and East, North, Up orientation representation using double values
	 * @param StartingVectors The matrix representation of the starting vectors (each vector is a column and the final vector is 0)
	 * @return The resulting conversion of swapping the first two vectors and negating the third
	 */
	static FMatrix ConvertNedAndEnu(FMatrix StartingVectors);
};
