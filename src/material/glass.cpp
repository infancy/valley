
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// materials/glass.cpp*
#include "material/glass.h"
#include"bsdf/specular.h"

namespace valley
{

// GlassMaterial Method Definitions
void GlassMaterial::compute_scattering(SurfaceInteraction* si, //MemoryArena &arena,
	TransportMode mode,
	bool allowMultipleLobes) const
{
	// Perform bump mapping with _bumpMap_, if present
	//if (bumpMap) Bump(bumpMap, si);

	Float eta = index->evaluate(*si);
	Float urough = uRoughness->evaluate(*si);
	Float vrough = vRoughness->evaluate(*si);
	Spectrum R = Kr->evaluate(*si).clamp();
	Spectrum T = Kt->evaluate(*si).clamp();

	// Initialize _bsdf_ for smooth or rough dielectric
	si->bsdf.reset(new BSDF(*si, eta));
	if (R.is_black() && T.is_black()) return;

	bool isSpecular = urough == 0 && vrough == 0;

	if (isSpecular && allowMultipleLobes)
	{
		si->bsdf->add_BxDF(new FresnelSpecular(R, T, 1.f, eta, mode));
	}
	else
	{
		//if (remapRoughness)
		//{
		//	urough = TrowbridgeReitzDistribution::RoughnessToAlpha(urough);
		//	vrough = TrowbridgeReitzDistribution::RoughnessToAlpha(vrough);
		//}
		//MicrofacetDistribution *distrib =
		//	isSpecular ? nullptr
		//	: ARENA_ALLOC(arena, TrowbridgeReitzDistribution)(
		//		urough, vrough);
		if (!R.is_black())
		{
			Fresnel* fresnel = new FresnelDielectric(1.f, eta);
			if (isSpecular)
				si->bsdf->add_BxDF(new SpecularReflection(R, fresnel));
			//else
			//	si->bsdf->Add(ARENA_ALLOC(arena, MicrofacetReflection)(
			//		R, distrib, fresnel));
		}
		if (!T.is_black())
		{
			//if (isSpecular)
			si->bsdf->add_BxDF(new SpecularTransmission(T, 1.f, eta, mode));
			//else
			//	si->bsdf->Add(ARENA_ALLOC(arena, MicrofacetTransmission)(
			//		T, distrib, 1.f, eta, mode));
		}
	}
}
/*
GlassMaterial *CreateGlassMaterial(const TextureParams &mp) 
{
    std::shared_ptr<Texture<Spectrum>> Kr =
        mp.GetSpectrumTexture("Kr", Spectrum(1.f));
    std::shared_ptr<Texture<Spectrum>> Kt =
        mp.GetSpectrumTexture("Kt", Spectrum(1.f));
    std::shared_ptr<Texture<Float>> eta = mp.GetFloatTextureOrNull("eta");
    if (!eta) eta = mp.GetFloatTexture("index", 1.5f);
    std::shared_ptr<Texture<Float>> roughu =
        mp.GetFloatTexture("uroughness", 0.f);
    std::shared_ptr<Texture<Float>> roughv =
        mp.GetFloatTexture("vroughness", 0.f);
    std::shared_ptr<Texture<Float>> bumpMap =
        mp.GetFloatTextureOrNull("bumpmap");
    bool remapRoughness = mp.FindBool("remaproughness", true);
    return new GlassMaterial(Kr, Kt, roughu, roughv, eta, bumpMap,
                             remapRoughness);
}
*/
}  // namespace pbrt
