#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(int totalFrames, float animationSpeed)
	: animationSpeed(animationSpeed), totalFrames(totalFrames), currentFrame(0) {}

int AnimatedObject::getCurrentFrame() const {
	return currentFrame;
}