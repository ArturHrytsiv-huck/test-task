// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class TEST_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called for forwards/backwards input */
	void MoveForward(float Value);

	/** Call for side to side input */
	void MoveRight(float Value);

	void TurnRate(float Rate);

	void LookUpRate(float Rate);

	/** Called when thee fire button is pressed */
	void FireWeapon();

	UFUNCTION(Server, Reliable)
	void FireButtonPressed();

	UFUNCTION(Server, Reliable)
	void FireButtonReleased();

	UFUNCTION(Server, Reliable)
	void StartFireTimer();
	
	UFUNCTION(Server, Reliable)
	void AutoFireReset();
	
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera that follows tha character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base look up/down, in deg/sec. Other scaling may affect final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	USoundWave* ShootingSound;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	bool bFireButtonPressed;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	bool bShouldFire;

	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	FTimerHandle AutoFireTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	float AutomaticFireRate = 1;
};
