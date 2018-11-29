#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Footstep.generated.h"


/**
 *	@class 
 */
UCLASS(ClassGroup = (Character))
class TAKAHANOPARIS_API UFootstep : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFootstep();

protected:

	/**
	 *	@property Intensity
	 *	@brief Defines how strong the sound should be played
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
		float Intensity;


	/**
	 *	@property SoundCue
	 *	@brief The sound to play
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
		class USoundBase * SoundCue;


	/**
	 *	@fn Notify
	 *	overriden from parent class
	 *	@see UAnimNotify
	 */
	virtual void Notify(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation) override;


private:

	/**
	 *	@property WalkingSurface
	 *	@brief EPhysicalSurface you're currently over, updated asynchronously
	 */
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> WalkingSurface;

	/**
	 *	@property bWantsTrace
	 *	@brief boolean telling if we need to do a new asynchronous trace
	 */
	UPROPERTY()
		uint32 bWantsTrace : 1; // Did user request trace, just a funny way to write a bool

	FTraceHandle	LastFootTrace;		// Handle to the last trace we did
	FTraceDelegate FootTraceDelegate; // Delegate fired when trace is complete

	/**	Foot Trace functions*/
	FTraceHandle RequestTrace(class USkeletalMeshComponent * Creator); // Actually Do Trace Request
	void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data); // Function Delegate
	void UseTraceResults(const FTraceDatum& TraceData);
	void SetWantTrace(class USkeletalMeshComponent * Creator);

};