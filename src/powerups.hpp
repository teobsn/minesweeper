#pragma once

#include <string>
#include <iostream>

#include "iobject.hpp"

class Game;

class BasePowerUp : public IObject
{
protected:
    // Folosim 'protected' pentru a permite claselor derivate (Temporal/Spatial) 
    // acces direct la atributele de baza, mentinandu-le in acelasi timp 
    // incapsulate fata de exterior.
    const int typeId;
    std::string name;

public:
    BasePowerUp(int id, std::string name);
    virtual ~BasePowerUp();

    void use(Game& game, int x = 0, int y = 0);

    virtual void activate(Game& game, int x, int y);
    // Clasa BasePowerUp nu poate fi instantiata direct deoarece include
    // doua functii virtual pure
    virtual std::string getDescription() const = 0;
    virtual std::string getCategory() const = 0;

    std::string getName() const;
    int getTypeId() const noexcept;
    std::string toString() const override;
};

// Utilizam 'virtual public' pentru a preveni duplicarea membrilor clasei BasePowerUp
// in ierarhia de tip diamant (UltimatePowerUp).
class TemporalEffect : virtual public BasePowerUp
{
protected:
    float duration;

public:
    TemporalEffect(int id, std::string name, float duration);
    virtual ~TemporalEffect() override;
    
    float getDuration() const;
};

class SpatialEffect : virtual public BasePowerUp
{
protected:
    int radius;

public:
    SpatialEffect(int id, std::string name, int radius);
    virtual ~SpatialEffect() override;

    int getRadius() const;
};

class ShieldPowerUp : public TemporalEffect
{
public:
    ShieldPowerUp(int id, float duration);
    virtual ~ShieldPowerUp();

    // Utilizam 'final' pe metodele claselor care nu sunt destinate sa fie baze
    // pentru alte derivate. Acest lucru garanteaza ca logica de activare
    // nu va fi modificata accidental in viitor (si ajuta la devirtualizare).
    void activate(Game& game, int x, int y) override final;
    std::string getDescription() const override final;
    std::string getCategory() const override final;
};

class ScannerPowerUp : public SpatialEffect
{
public:
    ScannerPowerUp(int id, int radius);
    virtual ~ScannerPowerUp();

    void activate(Game& game, int x, int y) override final;
    std::string getDescription() const override final;
    std::string getCategory() const override final;
};

// Utilizam 'final' pentru a marca faptul ca UltimatePowerUp este varful ierarhiei
// si nu mai poate fi derivat.
// Acest lucru permite compilatorului sa optimizeze
// apelurile de functii virtuale, eliminand overhead-ul 
// legat de vtable acolo unde este posibil.
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

    void activate(Game& game, int x, int y) override final;
    std::string getDescription() const override final;
    std::string getCategory() const override final;
};
