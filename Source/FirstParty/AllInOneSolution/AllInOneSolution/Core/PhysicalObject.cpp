
#include "PhysicalObject.hpp"
#include "Utility.hpp"

void PhysicalObject::updateKinematics(const float value, const float delta)
{
    if(m_rotation == nullptr)
        return;

    float angle = utility::toDegree<float,float>(m_body->GetAngle());
    float targetAngle = m_rotation->getValue();

    if(delta == 0)
    {
        m_body->SetAngularVelocity(0.f);
        return;
    }
    float velocity = (targetAngle - angle) / delta;
    m_body->SetAngularVelocity(utility::toRadian<float,float>(velocity));
}