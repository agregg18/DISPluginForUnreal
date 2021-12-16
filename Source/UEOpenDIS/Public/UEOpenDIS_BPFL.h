// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEOpenDISGameState.h"
#include "UEOpenDIS_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class UEOPENDIS_API UUEOpenDIS_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * Creates a 4x4 n^x matrix used for creating a rotation matrix
	 * @param NVector - A 3x1 vector representing the axis of rotation
	 */
	static FMatrix CreateNCrossXMatrix(FVector NVector);

	/**
	 * Creates a 4x4 rotation matrix around the given axis of rotation rotating by Theta degrees
	 * @param AxisVector - A 3x1 vector representing the axis of rotation
	 * @param ThetaRadians - The amount to rotate given in radians
	 * @param OutRotationMatrix
	 */
	static void CreateRotationMatrix(FVector AxisVector, float ThetaRadians, FMatrix& OutRotationMatrix);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param Heading - The degrees from North of the facing direction (spin left and right)
	 * @param Pitch - The degrees rotated about the local Y axis (front tip up and down)
	 * @param NorthVector - The vector pointing to the NorthVector
	 * @param EastVector - The vector pointing toward the North
	 * @param DownVector - The vector pointing away from the center of the Earth
	 * @param OutX - The x axis (forward) vector with the heading and pitch applied
	 * @param OutY - The y axis (right) vector with the heading and pitch applied
	 * @param OutZ - the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyHeadingPitchToNorthEastDownVector(const float Heading, const float Pitch, const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param Roll - The degrees rotated about the local X axis (tilt left and right)
	 * @param East - The vector pointing to the East
	 * @param North - The vector pointing toward the North
	 * @param Up - The vector pointing away from the center of the Earth
	 * @param OutX - The x axis (forward) vector with the heading and pitch applied
	 * @param OutY - The y axis (right) vector with the heading and pitch applied
	 * @param OutZ - the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyRollToNorthEastDownVector(const float Roll, const FVector East, const FVector North, const FVector Up, FVector& OutX, FVector& OutY, FVector& OutZ);

public:
	static float GetHeadingFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetPitchFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetRollFromEuler(float Lat, float Lon, float Psi, float Theta, float Phi);
	static double GetHeadingFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetPitchFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetRollFromEulerDouble(double Lat, double Lon, float Psi, float Theta, float Phi);

	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void EulerToENU(float LatInRad, float LonInRad, float Psi, float Theta, float Phi, FRotator& TaitBryanAnglesOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2UE4LocationESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LocationOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2LongLatHeightESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LonLatHeight);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2ENU2UERotESPDU(FEntityStatePDU EntityStatePDUIn, FRotator& RotationOut);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate - The target vector to rotate
	 * @param ThetaRadians - The desired amount to rotation in radians
	 * @param AxisVector - The vector indicating the axis of rotation
	 * @param OutRotatedVector - The resultant rotated vector
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void RotateVectorAroundAxisByRadians(FVector VectorToRotate, float ThetaRadians, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate - The target vector to rotate
	 * @param ThetaDegrees - The desired amount to rotation in degrees
	 * @param AxisVector - The vector indicating the axis of rotation
	 * @param OutRotatedVector - The resultant rotated vector
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void RotateVectorAroundAxisByDegrees(FVector VectorToRotate, float ThetaDegrees, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Applies the given heading, pitch, and roll in degrees to the local East North Down vectors
	 * @param HeadingDegrees - The degrees from North of the facing direction (spin left and right)
	 * @param PitchDegrees - The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees - The degrees rotated about the local X axis (tilt left and right)
	 * @param NorthVector - The vector pointing to the East
	 * @param EastVector - The vector pointing toward the North
	 * @param DownVector - The vector pointing away from the center of the Earth
	 * @param OutX - The x axis (forward) vector with the heading and pitch applied
	 * @param OutY - The y axis (right) vector with the heading and pitch applied
	 * @param OutZ - the z axis (up) vector with the heading and pitch applied
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ApplyHeadingPitchRollToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Calculates the East, North, and Up vectors at given latitude and longitude.
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param NorthVector - The local vector pointing east at the given latitude and longitude
	 * @param EastVector - The local vector pointing north at the given latitude and longitude
	 * @param DownVector - The local vector pointing away from the center of the Earth at the given latitude and longitude
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees, FVector& NorthVector, FVector& EastVector, FVector& DownVector);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingDegrees - The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees - The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees - The degrees rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param PsiDegrees - The rotation about the Z axis in degrees
	 * @param ThetaDegrees - The rotation about the Y axis in degrees
	 * @param PhiDegrees - The rotation about the X axis in degrees
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiDegrees, float& ThetaDegrees, float& PhiDegrees);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingRadians - The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians - The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians - The radians rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param PsiRadians - The rotation about the Z axis in radians
	 * @param ThetaRadians - The rotation about the Y axis in radians
	 * @param PhiRadians - The rotation about the X axis in radians
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingDegrees - The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees - The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees - The degrees rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param PsiRadians - The rotation about the Z axis in radians
	 * @param ThetaRadians - The rotation about the Y axis in radians
	 * @param PhiRadians - The rotation about the X axis in radians
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingRadians - The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians - The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians - The radians rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param PsiDegrees - The rotation about the Z axis in degrees
	 * @param ThetaDegrees - The rotation about the Y axis in degrees
	 * @param PhiDegrees - The rotation about the X axis in degrees
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiDegrees, float& ThetaDegrees, float& PhiDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiDegrees - The rotation about the Z axis in degrees
	 * @param ThetaDegrees - The rotation about the Y axis in degrees
	 * @param PhiDegrees - The rotation about the X axis in degrees
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param HeadingDegrees - The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees - The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees - The degrees rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees, const float ThetaDegrees, const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingDegrees, float& PitchDegrees, float& RollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiRadians - The rotation about the Z axis in radians
	 * @param ThetaRadians - The rotation about the Y axis in radians
	 * @param PhiRadians - The rotation about the X axis in radians
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param HeadingRadians - The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians - The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians - The radians rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians, const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingRadians, float& PitchRadians, float& RollRadians);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiRadians - The rotation about the Z axis in radians
	 * @param ThetaRadians - The rotation about the Y axis in radians
	 * @param PhiRadians - The rotation about the X axis in radians
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param HeadingDegrees - The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees - The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees - The degrees rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians, const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingDegrees, float& PitchDegrees, float& RollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiDegrees - The rotation about the Z axis in degrees
	 * @param ThetaDegrees - The rotation about the Y axis in degrees
	 * @param PhiDegrees - The rotation about the X axis in degrees
	 * @param LatitudeDegrees - The target latitude given in degrees
	 * @param LongitudeDegrees - The target longitude given in degrees
	 * @param HeadingRadians - The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians - The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians - The radians rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees, const float ThetaDegrees, const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingRadians, float& PitchRadians, float& RollRadians);
};
