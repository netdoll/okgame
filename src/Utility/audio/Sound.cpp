#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------




Logger Sound::log = Logger("Sound");

//Sound::Sound(Engine* g, int id)
//{ //=========================================================================================================================
//
//	this->e = g;
//	this->data = new SoundData(id, "", "");
//
//	for (int i = 0; i < (int)Main::audioManager->soundList->size(); i++)
//	{
//		if (Main::audioManager->soundList->get(i)->getName() == data->getName())
//		{
//			log.warn("Sound already exists:" + data->getName());
//			return;
//		}
//	}
//	Main::audioManager->soundList->add(this);
//}

Sound::Sound(Engine* g, shared_ptr<AudioFile> f)
{ //=========================================================================================================================
	this->e = g;

	this->audioFile = f;

	// This part is tricky because 'this' is a raw pointer and playingAudioList stores shared_ptr.
	// We cannot simply add 'this' to playingAudioList if Sound is created with make_shared.
	// The caller of the constructor (make_shared) will manage the shared_ptr.
	// If we need to add it to a list, we should do it outside the constructor or use shared_from_this().
	// But Sound inherits from EnginePart, not enable_shared_from_this.
	// If we want to use shared_ptr management, we should inherit enable_shared_from_this<Sound>.
	// However, looking at AudioManager, it adds the sound to the list when it creates it or when it plays it.

	
	// In the previous code: getAudioManager()->playingAudioList.add(this);
	// This implies Sound registers itself.
	// If Sound is managed by shared_ptr, we can't just add raw 'this'.
	// We should remove this self-registration and let AudioManager handle it.

	/*
	for (int i = 0; i < (int)getAudioManager()->playingAudioList.size(); i++)
	{
		if (getAudioManager()->playingAudioList.get(i)->getName() == f->getName())
		{
			if (getAudioManager()->playingAudioList.get(i)->getID() == -1)getAudioManager()->playingAudioList.get(i)->setID(f->getID());
			//log.warn("Sound already exists:" + data->getName());
			return;
		}
	}
	//getAudioManager()->playingAudioList.add(this); // CANNOT DO THIS WITH SHARED_PTR inside constructor easily without enable_shared_from_this
	*/


	if (f->getByteData() != nullptr)initFromByteData();
}



void Sound::initFromByteData()
{//=========================================================================================================================
	
#ifdef USE_SOLOUD
	filename = Main::getPath() + filename;
	soLoudWave = new SoLoud::Wav();
	//log.debug(filename);
	soLoudWave->load(filename.c_str());
#endif
#ifdef USE_SDL_MIXER

	SDL_IOStream* io = SDL_IOFromMem(audioFile->getByteData()->data(), (int)audioFile->getByteData()->size());
	mixAudio = MIX_LoadAudio_IO(AudioManager::mixer, io, true, true);

#endif

}


string Sound::getName()
{
	return audioFile->getName();

}

int Sound::getID()
{
	return audioFile->getID();

}
void Sound::setID(int id)
{
	return audioFile->setID(id);

}



void Sound::update()
{ //=========================================================================================================================

	//get the name and filename from the server



	if (audioFile->getFileExists() == true || audioFile->getByteData() != nullptr)
	{
#ifndef ORBIS
		if (mixAudio == nullptr)initFromByteData();
#else

#endif
		if (paused)
		{

		}
		else
		if (shouldBePlaying == true)
		{

	

			if (playingStarted)
			{

				bool playing = false;

#ifndef ORBIS
				if (mixTrack) playing = MIX_TrackPlaying(mixTrack);
#else

#endif
				if (playing == false)
				{


					//music
					if (loop == false)
					{
						if (timesToPlay > 1)
						{
							timesToPlay--;

#ifndef ORBIS
							if (!mixTrack) mixTrack = MIX_CreateTrack(AudioManager::mixer);
							MIX_SetTrackAudio(mixTrack, mixAudio);
							MIX_PlayTrack(mixTrack, 0);
#else

#endif
						}
						else
						{
							stop();
						}
					}
					else
					{

#ifndef ORBIS
						if (!mixTrack) mixTrack = MIX_CreateTrack(AudioManager::mixer);
						MIX_SetTrackAudio(mixTrack, mixAudio);
						MIX_PlayTrack(mixTrack, 0);
#else

#endif
					}

				}
			}
		}
		else
		{
			if (playingStarted == true)
			{
				stop();
			}
		}

		if (ticksToFadeOutTotal != -1)
		{
			ticksToFadeOutCounter -= (int)getEngine()->engineTicksPassed();

			if (ticksToFadeOutCounter <= 0)
			{
				stop();
			}
			else
			{
				setVolume(((float)(ticksToFadeOutCounter) / (float)(ticksToFadeOutTotal)) * volumeWhenStartedFade);
			}
		}

	}


}




void Sound::playOnce()
{ //=========================================================================================================================
	play(1.0f, getVolume(), 1);
}
void Sound::playLoop()
{ //=========================================================================================================================
	play(1.0f, getVolume(), true);
}


void Sound::play(float pitch, float volume, int timesToPlay)
{ //=========================================================================================================================

	if (timesToPlay < 0)
	{
		timesToPlay = 1;
		// log.error("Trying to play sound -1 times. Sounds cannot be infinitely looped, only music can.");
	}

	if (timesToPlay == 1)
	{
		timesToPlay = 1;
	}

	this->pitch = pitch;
	this->volume = volume;
	this->timesToPlay = timesToPlay;

	playImmediately();
}




void Sound::play(float pitch, float volume, bool loop)
{ //=========================================================================================================================

	if (this->pitch != pitch || this->volume != volume || this->loop != loop)
	{
		this->pitch = pitch;
		this->volume = volume;
		this->loop = loop;
	}

	shouldBePlaying = true;

	playImmediately();
}

void Sound::playImmediately()
{//=========================================================================================================================
	shouldBePlaying = true;


#ifdef USE_SOLOUD
	AudioManager::soLoud->play(*soLoudWave);
#endif
#ifdef USE_SDL_MIXER
	if (!mixTrack) mixTrack = MIX_CreateTrack(AudioManager::mixer);
	MIX_SetTrackAudio(mixTrack, mixAudio);
	MIX_SetTrackGain(mixTrack, volume);
	MIX_SetTrackLoops(mixTrack, loop ? -1 : 0);
	MIX_PlayTrack(mixTrack, 0);
#endif
	playingStarted = true;


	update();

}


void Sound::fadeOutAndStop(int ticksToFadeOut)
{ //=========================================================================================================================

	this->ticksToFadeOutTotal = ticksToFadeOut;
	this->ticksToFadeOutCounter = ticksToFadeOut;
	this->volumeWhenStartedFade = volume;
}

bool Sound::isFadingOut()
{ //=========================================================================================================================
	if (ticksToFadeOutTotal != -1)
	{
		return true;
	}

	return false;
}

void Sound::pause()
{ //=========================================================================================================================

#ifndef ORBIS
	if (mixTrack) MIX_PauseTrack(mixTrack);
#else

#endif
	paused = true;

}

void Sound::unpause()
{ //=========================================================================================================================

#ifndef ORBIS
	if (mixTrack) MIX_ResumeTrack(mixTrack);
#else

#endif
	paused = false;

}


//=========================================================================================================================
void Sound::stop()
{//=========================================================================================================================

	pitch = 1.0f;
	volume = 1.0f;
	timesToPlay = 1;

	loop = false;
	ticksToFadeOutCounter = -1;
	ticksToFadeOutTotal = -1;
	volumeWhenStartedFade = 0;

	shouldBePlaying = false;

	if (playingStarted)
	{
		if (mixTrack != nullptr)
		{

#ifndef ORBIS
			MIX_StopTrack(mixTrack, 0);
#else

#endif
		}

		playingStarted = false;
	}

	// We don't necessarily destroy the track, we can reuse it.
	// But for simplicity let's leave it.
}




void Sound::setLoop(bool b)
{ //=========================================================================================================================
	this->loop = b;
	#ifdef USE_SDL_MIXER
	if (mixTrack) MIX_SetTrackLoops(mixTrack, loop ? -1 : 0);
	#endif
}

bool Sound::getLoop()
{ //=========================================================================================================================
	return loop;
}

bool Sound::isPlaying()
{ //=========================================================================================================================

	return shouldBePlaying;

}

void Sound::setVolume(float v)
{ //=========================================================================================================================
	volume = v;
#ifdef USE_SDL_MIXER
	if (mixTrack) MIX_SetTrackGain(mixTrack, volume);
#endif
}
float Sound::getVolume()
{
	return volume;

}
float Sound::getPitch()
{
	return pitch;

}
void Sound::setPitch(float p)
{ //=========================================================================================================================

	pitch = p;
	//   if (channel != nullptr)
	//   {
	//      channel->setPitch(p);
	//   }
}


