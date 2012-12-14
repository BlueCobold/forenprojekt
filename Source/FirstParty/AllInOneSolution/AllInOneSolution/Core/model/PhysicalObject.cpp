
#include "PhysicalObject.hpp"
#include "../Utility.hpp"

#include <cfloat>
#include <cmath>

void PhysicalObject::updateKinematics(const float value, const float delta)
{
    if(delta == 0)
    {
        m_body->SetAngularVelocity(0.f);
        return;
    }

    if(m_rotation != nullptr)
    {
        float angle = utility::toDegree<float,float>(m_body->GetAngle());
        float targetAngle = m_rotation->getValue();
        float velocity = (targetAngle - angle) / delta;
        m_body->SetAngularVelocity(utility::toRadian<float,float>(velocity));
    }

    if(m_xPositionProvider == nullptr && m_yPositionProvider == nullptr)
        return;
    b2Vec2 pos = m_body->GetPosition();
    if(m_xPositionProvider != nullptr)
        pos.x = utility::toMeter(m_xPositionProvider->getValue()) + m_basePosition.x - pos.x;
    else
        pos.x = 0;
    if(m_yPositionProvider != nullptr)
        pos.y = utility::toMeter(m_yPositionProvider->getValue()) + m_basePosition.y - pos.y;
    else
        pos.y = 0;
    if(fabs(pos.x)>FLT_EPSILON || fabs(pos.y)>FLT_EPSILON)
        m_body->SetLinearVelocity(b2Vec2(pos.x/delta, pos.y/delta));
}