#pragma once
#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H

class AnimatedObject {
protected:
	float animationSpeed;
	int currentFrame;
	int totalFrames;
public:
	AnimatedObject(int totalFrames, float animationSpeed);
	~AnimatedObject() = default;

	virtual void animate() = 0;

	int getCurrentFrame() const;
};

#endif 