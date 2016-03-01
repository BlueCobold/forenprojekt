
#include <jni.h>

#include "../../Src/Utility.hpp"

namespace
{
	BatteryState batteryState;
}

BatteryState getBatteryStateImpl()
{
	return batteryState;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_de_gamecoding_ricketyracquet_MainApplication_setBattery
          (JNIEnv *env, jobject thisObj, jint percent, jint state) {
       batteryState.percent = static_cast<float>(percent) / 100.f;
	   batteryState.state = static_cast<BatteryState::ChargeState>(state);
    }
}
