#pragma once

#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactListener : public b2ContactListener
{    
    void BeginContact(b2Contact* contact);

    void EndContact(b2Contact* contact);

};

#endif // CONTACT_LISTENER_HPP

