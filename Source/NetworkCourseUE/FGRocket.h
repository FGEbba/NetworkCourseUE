#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGRocket.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class NETWORKCOURSEUE_API AFGRocket : public AActor
{
	GENERATED_BODY()

public:
	AFGRocket();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void StartMoving(const FVector& Forward, const FVector& InStartLocation);
	void ApplyCorrection(const FVector& Forward);

	void Explode();
	void MakeFree();

	bool IsFree() const { return bIsFree; }

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = 1))
		int Damage = 10;
private:

	void SetRocketVisibility(bool bVisible);
	FCollisionQueryParams CachedCollisionQuaryParams;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UParticleSystem* Explosion = nullptr;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Debug")
		bool bDebugDrawCorrection = true;

	FVector OriginalFacingDirection = FVector::ZeroVector;

	FVector FacingRotationStart = FVector::ZeroVector;
	FQuat FacingRotationCorrection = FQuat::Identity;

	FVector RocketStartLocation = FVector::ZeroVector;

	float LifeTime = 2.0f;
	float LifeTimeElapsed = 0.0f;

	float DistanceMoved = 0.0f;

	UPROPERTY(EditAnywhere)
		float MovementVelocity = 1300.0f;

	bool bIsFree = true;
};
