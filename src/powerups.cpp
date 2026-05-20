#include "powerups.hpp"
#include "game.hpp"

#include <sstream>

BasePowerUp::BasePowerUp(int id, std::string name) : typeId(id), name(name)
{
#ifdef DEBUG
    std::cout << "BasePowerUp() " << name << "\n";
#endif
}

BasePowerUp::~BasePowerUp()
{
#ifdef DEBUG
    std::cout << "~BasePowerUp() " << name << "\n";
#endif
}

std::string BasePowerUp::getName() const { return name; }

int BasePowerUp::getTypeId() const noexcept { return typeId; }

std::string BasePowerUp::toString() const
{
    std::ostringstream os;
    os << "PowerUp #" << getId() << " [type " << typeId << "] " << name
       << " - " << getDescription() << " (" << getCategory() << ")";
    return os.str();
}

void BasePowerUp::use(Game &game, int x, int y)
{
    this->activate(game, x, y); // Valid (metoda virtuala publica)
}

void BasePowerUp::activate(Game &game, int, int)
{
    game.addPowerUpToHistory(this); // Valid (metoda publica a clasei Game)
}

TemporalEffect::TemporalEffect(int id, std::string name, float duration)
    : BasePowerUp(id, name), duration(duration)
{
#ifdef DEBUG
    std::cout << "TemporalEffect()\n";
#endif
}

TemporalEffect::~TemporalEffect()
{
#ifdef DEBUG
    std::cout << "~TemporalEffect()\n";
#endif
}

float TemporalEffect::getDuration() const
{
    return duration;
}

SpatialEffect::SpatialEffect(int id, std::string name, int radius)
    : BasePowerUp(id, name), radius(radius)
{
#ifdef DEBUG
    std::cout << "SpatialEffect()\n";
#endif
}

SpatialEffect::~SpatialEffect()
{
#ifdef DEBUG
    std::cout << "~SpatialEffect()\n";
#endif
}

int SpatialEffect::getRadius() const
{
    return radius;
}

ShieldPowerUp::ShieldPowerUp(int id, float duration)
    : BasePowerUp(id, "Shield"), TemporalEffect(id, "Shield", duration) {}

ShieldPowerUp::~ShieldPowerUp()
{
    std::cout << "~ShieldPowerUp()\n";
}

void ShieldPowerUp::activate(Game &game, int x, int y)
{
    BasePowerUp::activate(game, x, y);
    game.setInvulnerable(true);
}

std::string ShieldPowerUp::getDescription() const
{
    return "Protects you from the next bomb you click.";
}

std::string ShieldPowerUp::getCategory() const
{
    return "Defense";
}

ScannerPowerUp::ScannerPowerUp(int id, int radius)
    : BasePowerUp(id, "Scanner"), SpatialEffect(id, "Scanner", radius) {}

ScannerPowerUp::~ScannerPowerUp()
{
    std::cout << "~ScannerPowerUp()\n";
}

void ScannerPowerUp::activate(Game &game, int x, int y)
{
    BasePowerUp::activate(game, x, y);
    game.activateRadar(x, y, radius);
}

std::string ScannerPowerUp::getDescription() const
{
    return "Reveals a small area around the cursor.";
}

std::string ScannerPowerUp::getCategory() const
{
    return "Detection";
}

UltimatePowerUp::UltimatePowerUp(int id, float duration, int radius)
    : BasePowerUp(id, "Ultimate"), TemporalEffect(id, "Ultimate", duration), SpatialEffect(id, "Ultimate", radius) {}

UltimatePowerUp::~UltimatePowerUp()
{
    std::cout << "~UltimatePowerUp()\n";
}

void UltimatePowerUp::activate(Game &game, int x, int y)
{
    BasePowerUp::activate(game, x, y);
    game.setInvulnerable(true);
    game.activateRadar(x, y, radius);
}

std::string UltimatePowerUp::getDescription() const
{
    return "Combines the scanner and shield power-ups.";
}

std::string UltimatePowerUp::getCategory() const
{
    return "Tactical";
}

BombDefuser::BombDefuser(int id, double ratio)
    : BasePowerUp(id, "Bomb Defuser"), ratio(ratio) {}

BombDefuser::~BombDefuser()
{
    std::cout << "~BombDefuser()\n";
}

void BombDefuser::activate(Game &game, int x, int y)
{
    BasePowerUp::activate(game, x, y);
    game *ratio; // operator overload :)
}

std::string BombDefuser::getDescription() const
{
    return "Instantly defuses half of the hidden bombs.";
}

std::string BombDefuser::getCategory() const
{
    return "Utility";
}
