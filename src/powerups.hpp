#pragma once

#include <string>
#include <iostream>

class Game;

class BasePowerUp
{
protected:
    // Folosim 'protected' pentru a permite claselor derivate (Temporal/Spatial) 
    // acces direct la atributele de baza, mentinandu-le in acelasi timp 
    // incapsulate fata de exterior.
    int id;
    std::string name;

public:
    BasePowerUp(int id, std::string name);
    virtual ~BasePowerUp();

    void use(Game& game, int x = 0, int y = 0);

    virtual void activate(Game& game, int x, int y);
    virtual std::string getDescription() const = 0;
    virtual std::string getCategory() const = 0;

    std::string getName() const;
};

// Utilizam 'virtual public' pentru a preveni duplicarea membrilor clasei BasePowerUp
// in ierarhia de tip diamant (UltimatePowerUp).
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
    std::string getCategory() const override;
};

class ScannerPowerUp : public SpatialEffect
{
public:
    ScannerPowerUp(int id, int radius);
    virtual ~ScannerPowerUp();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
    std::string getCategory() const override;
};

class UltimatePowerUp final : public TemporalEffect, public SpatialEffect
{
public:
    UltimatePowerUp(int id, float duration, int radius);
    ~UltimatePowerUp();

    void activate(Game& game, int x, int y) override;
    std::string getDescription() const override;
    std::string getCategory() const override;
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
    std::string getCategory() const override;
};
