// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "Image.h"
#include "MediaTexture.h"

UVideoWidget::UVideoWidget() : Super()
{
	VideoPlayer = CreateDefaultSubobject<UMediaPlayer>(TEXT("Video Player"));
	VideoTexture = CreateDefaultSubobject<UMediaTexture>(TEXT("Video Texture"));
	VideoTexture->SetDefaultMediaPlayer(VideoPlayer);
}


void UVideoWidget::BindDelegates()
{
	Super::BindDelegates();
	if (VideoPlayer && VideoTexture && VideoScreen)
	{
		VideoTexture->SetDefaultMediaPlayer(VideoPlayer);
		VideoPlayer->OnEndReached.Clear();
		VideoPlayer->OnEndReached.AddDynamic(this, &UVideoWidget::ReachedEndOfVideo);
		VideoScreen->BrushDelegate.BindUFunction(this, TEXT("SetvideoBinding"));
	}
}

void UVideoWidget::Play()
{
	if(VideoPlayer)
		VideoPlayer->Play();
}

void UVideoWidget::Pause()
{
	if (VideoPlayer)
		VideoPlayer->Pause();
}

void UVideoWidget::Skip()
{
	if (VideoPlayer)
	{	
		VideoPlayer->Seek(VideoPlayer->GetDuration());
	}
		
}

void UVideoWidget::ReachedEndOfVideo()
{
	EndOfVideo.Broadcast();
}

FSlateBrush UVideoWidget::SetVideoBinding()
{
	FSlateBrush RetVal;
	if(VideoTexture)
	{
		RetVal.SetResourceObject(VideoTexture);
	}
	return RetVal;
}
