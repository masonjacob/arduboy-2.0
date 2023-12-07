//  ***************************************************************
//  Fruit - Creation date: 06/02/2016
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2016 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef Fruit_h__
#define Fruit_h__

#include "Console.h"

class Fruit
{
public:
	Fruit();
	~Fruit();

	void ResetForNewLevel();
	void OnPlayerDie();
	void Update();
	void OnPelletEaten();
	void Render(Console* gameboy);
	void UnRender(Console* gameboy);
	void RenderSideFruits(Console* gameboy);

private:
	
	bool m_bActive;
	unsigned long m_killTimer;
};

extern Fruit fruit;
#endif // Fruit_h__