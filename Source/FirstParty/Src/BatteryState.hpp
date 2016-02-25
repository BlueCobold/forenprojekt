#pragma once

#ifndef BATTERY_STATE_HPP
#define BATTERY_STATE_HPP

struct BatteryState
{
	enum ChargeState
	{
		Unknown,
		Unplugged,
		Charging,
		Full
	};
	
	float percent;
	ChargeState state;

    BatteryState() :
        state(Unknown),
        percent(1.f)
    { }
};

#endif // BATTERY_STATE_HPP
