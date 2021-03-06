/*
  ==============================================================================

    SynthVoice.h
    Created: 13 Nov 2018 2:55:49pm
    Author:  Teo

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"


class SynthVoice : public SynthesiserVoice
{
public:

	bool canPlaySound(SynthesiserSound* sound)
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}
	//============================================ canPlaySound()

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition)
	{
		//called to start a new note
		env1.trigger = 1;
		level = velocity;
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		return;
	}
	//============================================ startNote()

	void stopNote(float velocity, bool allowTailOff)
	{
		//called to stop a note
		env1.trigger = 0;
		allowTailOff = true;

		if(velocity == 0)
			clearCurrentNote();
		return;
	}
	//============================================ stopNote()

	void renderNextBlock(AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
	{
		//Renders the next block of data for this voice
		for(int sample = 0; sample < numSamples; ++sample){
			double sound = env1.adsr(setOscType(), env1.trigger) * level;
			double soundFiltered = sound;/*filter1.lores(sound, 200, 0.1);*/

			for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel){


				outputBuffer.addSample(channel, startSample, soundFiltered * 0.3f);		// * 0.3f so it donesn`t distort randomly
			}

			++startSample;
		}

		return;
	}
	//============================================ renderNextBlock()

	void pitchWheelMoved(int newPitchWheelValue)
	{
		//Called to let the voice know that the pitch wheel has been moved


		return;
	}
	//============================================ pitchWheelMoved()

	void controllerMoved(int controllerNumber, int newControllerValue)
	{
		//Called to let the voice know that a midi controller has been moved


		return;
	}
	//============================================ controllerMoved()w
	double setOscType(){
		switch(waveType){
			case 0:
				return osc.sinewave(frequency);
				break;
			case 1:
				return osc.saw(frequency);
				break;
			case 2:
				return osc.triangle(frequency);
				break;
			case 3:
				return osc.square(frequency);
				break;
			case 4:
				return osc.pulse(frequency, 0.25f);
				break;
			case 5:
				return osc.sawn(frequency);	//bandlimited saw will be changed
				break;
			default:		//if broke use the sinewave
				return osc.sinewave(frequency);
				break;
		}
		return 0.0f;
	}

	//Setters
	void setAtkS(float param){
		env1.setAttack(param);
	}

	void setDecS(float param){
		env1.setDecay(param);
	}

	void setSusS(float param){
		env1.setSustain(param);
	}

	void setRelS(float param){
		env1.setRelease(param);
	}

	void getOscType(float* type){
		waveType = (int)(*type);
	}

private:
	double level;
	double frequency;
	unsigned short int waveType;

	maxiOsc osc;
	maxiEnv env1;
	maxiFilter filter1;
};