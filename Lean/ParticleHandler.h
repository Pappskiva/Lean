////////////////////////////////////////////////////////////////////////////////
// Filename: ParticleHandler.h
////////////////////////////////////////////////////////////////////////////////

/*
Syftet med denna klassen �r att skapa ett objekt som l�ttare han hantera partiklar,
s� om man vill ha mer �n en, eller l�gga till och ta bort dem s� ska det fungera.

Tanken �r att partiklar ska, f�r det mesta, ha en tid, allts� hur l�nge partikeleffekten ska finnas innan den f�rsvinner.
Vissa partiklar kanske ska h�lla sig tills man tar bort dem, som vid regn och sn�, till exempel.
*/

#include "D3D.h"
#include "ParticleEmitter.h"
#include "InstanceRenderer.h"

#pragma once
#ifndef PARTICLEHANDLER_H
#define PARTICLEHANDLER_H

class ParticleHandler
{
public:
	ParticleHandler();
	~ParticleHandler();

	void Initialize(D3D*, Shader*);
	void Shutdown();
	void Update(const uint, const float);
	void Render();

	/*L�gg till en partikel, int �r f�r stunden ett v�rde som s�ger �t oss
	vilken typ av partikel man vill ha. Detta l�r beh�vas ut�kas senare. s� att position och s�dant finns*/
	void AddParticles(int);
	/*Ta bort alla partiklar, som n�r man byter bana, till exempel*/
	void RemoveAllParticles();

private:
	InstanceRenderer particleRenderer;
	Array<ParticleEmitter> emitters;
	Shader* particleBillboard;
	Texture *leaf;
};
#endif