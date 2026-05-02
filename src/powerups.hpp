#pragma once

#include <string>
#include <iostream>

class Game;

class BasePowerUp
{
protected:
    int id;
    std::string name;

public:
    BasePowerUp(int id, std::string name);
    virtual ~BasePowerUp();

    void use(Game& game, int x = 0, int y = 0);
    
    virtual void activate(Game& game, int x, int y) = 0;
    virtual std::string getDescription() const = 0;

    std::string getName() const;
};

class TemporalEffect : virtual public BasePowerUp
{
protected:
    float duration;

public:
    TemporalEffect(int id, std::string name, float duration);
    virtual ~TemporalEffect();
    
    float getDuration() const;
};

class SpatialEffect : virtual public BasePowerUp
{
protected:
    int radius;

public:
    SpatialEffect(int id, std::string name, int radius);
    virtual ~SpatialEffect();

    int getRadius() const;
};

class ShieldPowerUp : public TemporalEffect
{
public:
    ShieldPowerUp(int id, float duration);
    virtual ~ShieldPowerUp();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
};

class ScannerPowerUp : public SpatialEffect
{
public:
    ScannerPowerUp(int id, int radius);
    virtual ~ScannerPowerUp();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
};

class UltimatePowerUp final : public TemporalEffect, public SpatialEffect
{
public:
    UltimatePowerUp(int id, float duration, int radius);
    ~UltimatePowerUp();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
};

class BombDefuser : public BasePowerUp
{
private:
    double ratio;

public:
    BombDefuser(int id, double ratio);
    virtual ~BombDefuser();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
};
