#pragma once
#include "MouseEvent.h"

class OnMouseActionListener
{
public:
	OnMouseActionListener();
	~OnMouseActionListener();

	inline virtual bool onMouseAction(MouseEvent*, MouseEvent::Type, MouseEvent::Input) = 0;
};

