/*!
\brief A special effect involving a small, stable flame, useful for candles.
*/

#ifdef EYE_CANDY

#ifndef EFFECT_CANDLE_H
#define EFFECT_CANDLE_H

// I N C L U D E S ////////////////////////////////////////////////////////////

#include "eye_candy.h"

namespace ec
{

// C L A S S E S //////////////////////////////////////////////////////////////

class CandleEffect : public Effect
{
public: 
  CandleEffect(EyeCandy* _base, bool* _dead, Vec3* _pos, const float scale, const Uint16 _LOD);
  ~CandleEffect(); 
  
  virtual EffectEnum get_type() { return EC_CANDLE; };
  bool idle(const Uint64 usec);

  GradientMover* mover;
  ParticleSpawner* spawner;
  float scale;
  float sqrt_scale;
};

class CandleParticle : public Particle
{
public:
  CandleParticle(Effect* _effect, ParticleMover* _mover, const Vec3 _pos, const Vec3 _velocity, const float _scale, const Uint16 _LOD);
  ~CandleParticle() { };
  
  virtual bool idle(const Uint64 delta_t);
  virtual GLuint get_texture(const Uint16 res_index);
  virtual void draw(const Uint64 usec);
  virtual light_t estimate_light_level() const { return 0.0; };	// Like above
  virtual light_t get_light_level() { return 0.0; }; // Same.
  
  Uint16 LOD;
};

///////////////////////////////////////////////////////////////////////////////

}	// End namespace ec

#endif	// defined EFFECT_CANDLE_H

#endif	// #ifdef EYE_CANDY