#pragma once

#ifndef BATTERY_STATE_HPP
#define BATTERY_STATE_HPP

struct BatteryState
{
    enum ChargeState : int
	{
		Unknown = 0,
		Unplugged = 1,
		Charging = 2,
		Full = 3
	};
	
	float percent;
	ChargeState state;

    BatteryState() :
        state(Unknown),
        percent(0.f)
    { }
};

#endif // BATTERY_STATE_HPP
