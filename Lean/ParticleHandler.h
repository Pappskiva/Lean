////////////////////////////////////////////////////////////////////////////////
// Filename: ParticleHandler.h
////////////////////////////////////////////////////////////////////////////////

/*
Syftet med denna klassen är att skapa ett objekt som lättare han hantera partiklar,
så om man vill ha mer än en, eller lägga till och ta bort dem så ska det fungera.

Tanken är att partiklar ska, för det mesta, ha en tid, alltså hur länge partikeleffekten ska finnas innan den försvinner.
Vissa partiklar kanske ska hålla sig tills man tar bort dem, som vid regn och snö, till exempel.
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

	/*Lägg till en partikel, int är för stunden ett värde som säger åt oss
	vilken typ av partikel man vill ha. Detta lär behövas utökas senare. så att position och sådant finns*/
	void AddParticles(int);
	/*Ta bort alla partiklar, som när man byter bana, till exempel*/
	void RemoveAllParticles();

private:
	InstanceRenderer particleRenderer;
	Array<ParticleEmitter> emitters;
	Shader* particleBillboard;
	Texture *leaf;
};
#endif