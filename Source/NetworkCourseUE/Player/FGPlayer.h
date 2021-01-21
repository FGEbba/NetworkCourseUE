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
class AFGRocket;
class AFGPickup;

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

	void OnPickup(AFGPickup* Pickup);

	UFUNCTION(Server, Reliable)
		void Server_OnPickup(AFGPickup* Pickup);

	UFUNCTION(Client, Reliable)
		void Client_OnPickupRockets(int32 PickedUpRockets);

	UFUNCTION(Server, Unreliable)
		void Server_SendLocation(const FVector& LocationToSend);

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_SendLocation(const FVector& LocationToSend);

	UFUNCTION(Server, Unreliable)
		void Server_SendYaw(float NewYaw);


	/*The old send rotation scripts.
	UFUNCTION(Server, Unreliable)
		void Server_SendRotation(const FRotator& RotationToSend, float DeltaTime);

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_SendRotation(const FRotator& RotationToSend, float DeltaTime);
	*/

	void ShowDebugMenu();
	void HideDebugMenu();

	UFUNCTION(BlueprintPure)
		int32 GetNumRockets() const { return NumRockets; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Player", meta = (DisplayName = "On Num Rockets Changed"))
		void BP_OnNumRocketsChanged(int32 NewNumRockets);

	int32 GetNumActiveRockets() const;

	void FireRocket();

	void SpawnRockets();


private:
	int32 ServerNumRockets = 0;

	int32 NumRockets = 0;

	FVector GetRocketStartLocation() const;

	AFGRocket* GetFreeRocket() const;

	UFUNCTION(Server, Reliable)
		void Server_FireRocket(AFGRocket* NewRocket, const FVector& RocketStartLocation, const FRotator& RocketFacingRotation);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_FireRocket(AFGRocket* NewRocket, const FVector& RocketStartLocation, const FRotator& RocketFacingRotation);

	UFUNCTION(Client, Reliable)
		void Client_RemoveRocket(AFGRocket* RocketToRemove);

	UFUNCTION(BlueprintCallable)
		void Cheat_IncreaseRocket(int32 InNumRockets);

	UPROPERTY(Replicated, Transient)
		TArray<AFGRocket*> RocketInstances;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<AFGRocket> RocketClass;

	int32 MaxActiveRockets = 3;

	float FireCooldownElapsed = 3;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bUnlimitedRockets = false;

	void Handle_Accelerate(float Value);
	void Handle_Turn(float Value);
	void Handle_BrakePressed();
	void Handle_BrakeReleased();

	void Handle_FirePressed();

	void Handle_DebugMenuPressed();
	void CreateDebugWidget();

	UPROPERTY(Transient)
		UFGNetDebugWidget* DebugMenuInstance = nullptr;

	UPROPERTY(Replicated)
		float ReplicatedYaw = 0.0f;

	UPROPERTY(Replicated)
		FVector ReplicatedLocation;

	/*Used for the old send rotation script.
	FVector prevPingedLocation = FVector::ZeroVector;
	FRotator prevPingedRotation = FRotator::ZeroRotator;

	float PrevPingTime = 0.0f;
	const float TransitionTime = 2.5f;*/

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
