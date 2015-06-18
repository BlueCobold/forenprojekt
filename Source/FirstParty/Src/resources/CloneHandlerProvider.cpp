
#include "CloneHandlerProvider.hpp"

std::unordered_map<const void*, const TimedObject*> CloneHandlerProvider::timeReplacements;
std::unordered_map<const void*, const OrientedObject*> CloneHandlerProvider::oriReplacements;
std::unordered_map<const void*, Stoppable*> CloneHandlerProvider::stopReplacements;
std::unordered_map<const void*, const VariableHandler*> CloneHandlerProvider::varReplacements;
std::unordered_map<const void*, VariableHandler*> CloneHandlerProvider::setReplacements;