#pragma once
#include "Object.h"
class Sprite : public Object
{
public:
	Sprite(); // ver se ta certo
	~Sprite();

	void setSpritesheet(GLuint texID, int nAnimations, int nFrames){ this->nAnimations = nAnimations; this->nFrames = nFrames;
	}
	inline void setAnimation(int iAnimation) { this->iAnimation = iAnimation; }
	inline void setFrame(int iFrame) { this->iFrame = iFrame; }
	void update();
	inline void setMoving(bool moving) { this->moving = moving; }

protected:
	int nAnimations, nFrames;
	int iAnimation, iFrame;
	float dx, dy;
	bool moving;
};

