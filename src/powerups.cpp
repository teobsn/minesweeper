#include "powerups.hpp"
#include "game.hpp"

BasePowerUp::BasePowerUp(int id, std::string name) : id(id), name(name)
{
    std::cout << "BasePowerUp() " << name;
}

BasePowerUp::~BasePowerUp()
{
    std::cout << "~BasePowerUp() " << name;
}

std::string BasePowerUp::getName() const { return name; }

void BasePowerUp::use(Game &game, int x, int y)
{
    this->activate(game, x, y);
}


TemporalEffect::TemporalEffect(int id, std::string name, float duration)
    : BasePowerUp(id, name), duration(duration)
{
    std::cout << "TemporalEffect()\n";
}

TemporalEffect::~TemporalEffect()
{
    std::cout << "~TemporalEffect()\n";
}

float TemporalEffect::getDuration() const
{
    return duration;
}


SpatialEffect::SpatialEffect(int id, std::string name, int radius)
    : BasePowerUp(id, name), radius(radius)
{
    std::cout << "SpatialEffect()\n";
}

SpatialEffect::~SpatialEffect()
{
    std::cout << "~SpatialEffect()\n";
}

int SpatialEffect::getRadius() const
{
    return radius;
}


ShieldPowerUp::ShieldPowerUp(int id, float duration)
    : BasePowerUp(id, "Shield"), TemporalEffect(id, "Shield", duration) {}

ShieldPowerUp::~ShieldPowerUp() {}

void ShieldPowerUp::activate(Game &game, int, int)
{
    game.setInvulnerable(true);
}

std::string ShieldPowerUp::getDescription() const
{
    return "Protects from the NEXT bomb you click.";
}


ScannerPowerUp::ScannerPowerUp(int id, int radius)
    : BasePowerUp(id, "Scanner"), SpatialEffect(id, "Scanner", radius) {}

ScannerPowerUp::~ScannerPowerUp() {}

void ScannerPowerUp::activate(Game &game, int x, int y)
{
    game.activateRadar(x, y, radius);
}

std::string ScannerPowerUp::getDescription() const
{
    return "Reveals a small area around cursor.";
}


UltimatePowerUp::UltimatePowerUp(int id, float duration, int radius)
    : BasePowerUp(id, "Ultimate"), TemporalEffect(id, "Ultimate", duration), SpatialEffect(id, "Ultimate", radius) {}

UltimatePowerUp::~UltimatePowerUp() {}

void UltimatePowerUp::activate(Game &game, int x, int y)
{
    game.setInvulnerable(true);
    game.activateRadar(x, y, radius);
}

std::string UltimatePowerUp::getDescription() const
{
    return "One-hit Shield + Large Area Scan.";
}


BombDefuser::BombDefuser(int id, double ratio)
    : BasePowerUp(id, "Bomb Defuser"), ratio(ratio) {}

BombDefuser::~BombDefuser() {}

void BombDefuser::activate(Game &game, int, int)
{
    game * ratio; // operator overload :)
}

std::string BombDefuser::getDescription() const
{
    return "Instantly defuses half of hidden bombs.";
}
