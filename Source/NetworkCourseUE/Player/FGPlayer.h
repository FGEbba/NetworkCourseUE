#pragma once

#include "CoreMinimal.h"
#include "FGPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UFGMovementComponent;
class UStaticMeshComponent;
class USphereComponent;

class UFGPlayerSettings;
class UFGNetDebugWidget;


UCLASS()
class NETWORKCOURSEUE_API AFGPlayer : public APawn
{
	GENERATED_BODY()

public:
	AFGPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = Settings)
		UFGPlayerSettings* PlayerSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = Debug)
		TSubclassOf<UFGNetDebugWidget> DebugMenuClass;




	UFUNCTION(BlueprintPure)
		bool IsBraking() const { return bBrake; }

	UFUNCTION(BlueprintPure)
		int32 GetPing() const;


	UFUNCTION(Server, Unreliable)
		void Server_SendLocation(const FVector& LocationToSend, float DeltaTime);


	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_SendLocation(const FVector& LocationToSend, float DeltaTime);

	UFUNCTION(Server, Unreliable)
		void Server_SendRotation(const FRotator& RotationToSend, float DeltaTime);

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_SendRotation(const FRotator& RotationToSend, float DeltaTime);




	void ShowDebugMenu();
	void HideDebugMenu();


private:
	void Handle_Accelerate(float Value);
	void Handle_Turn(float Value);
	void Handle_BrakePressed();
	void Handle_BrakeReleased();

	void Handle_DebugMenuPressed();
	void CreateDebugWidget();

	UPROPERTY(Transient)
		UFGNetDebugWidget* DebugMenuInstance = nullptr;

	FVector prevPingedLocation = FVector::ZeroVector;
	FRotator prevPingedRotation = FRotator::ZeroRotator;

	float PrevPingTime = 0.0f;
	const float TransitionTime = 2.5f;

	float Forward = 0.0f;
	float Turn = 0.0f;

	float MovementVelocity = 0.0f;
	float Yaw = 0.0f;

	bool bBrake = false;
	bool bShowDebugMenu = false;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UFGMovementComponent* MovementComponent;

};
