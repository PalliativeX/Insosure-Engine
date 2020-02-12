#pragma once

struct particle
{
    vec2 Position;
    vec2 Velocity;
    float Size;
    float Lifetime;
};

particle
SpawnParticle(vec2 ObjectPos, vec2 Offset, vec2 Velocity)
{
    vec2 Random = vec2{GetRandomFloat(-1, 1), GetRandomFloat(-1, 1)};

    particle Particle;
    Particle.Position = ObjectPos + Offset + Random;
    Particle.Lifetime = 2.5f;
    Particle.Velocity = Velocity;
    Particle.Size = GetRandomFloat(0, 1) * 2.f; // NOTE(vosure) Random range?!?

    return Particle;
}

// NOTE(insolence): Updating lifetime and position of a particle
void
UpdateParticles(std::vector<particle> &Particles, float DeltaTime)
{
    for (int i = 0; i < Particles.size(); i++)
    {
        Particles[i].Position += Particles[i].Velocity * DeltaTime;
        Particles[i].Lifetime -= DeltaTime;
        if (Particles[i].Lifetime <= 0.f)
            Particles.erase(Particles.begin() + i);
            //Particles[i].Lifetime = 0.f;
    }
}