#pragma once

#ifndef CONTROLLER_PARSER_HPP
#define CONTROLLER_PARSER_HPP

#include "../animation/provider/ValueProvider.hpp"
#include "ProviderParserContext.hpp"
#include "ProviderParser.hpp"

#include <memory>
#include <array>

#include "tinyxml2.h"

class ResourceManager;

template<typename T, size_t N>
class Array
{
    std::array<T, N> m_storage;

public:
    template<typename S, size_t I>
    class ArrayProxy
    {
        std::array<S, I>& m_target;
        size_t m_index;

    public:
        ArrayProxy(std::array<S, I>& target, size_t index) :
            m_target(target),
            m_index(index)
        { }

        S& operator= (S value)
        {
            m_target[m_index] = std::move(value);
            return m_target[m_index];
        }
        
        S& operator* () const
        {
            return m_target[m_index];
        }
    };

    Array<T, N>()
    { }

    Array<T, N>(Array<T, N>&& other)
    {
        std::move(begin(other.m_storage), end(other.m_storage), begin(m_storage));
    }

    Array<T, N>& operator= (Array<T, N>&& other)
    {
        std::move(begin(other.m_storage), end(other.m_storage), begin(m_storage));
        return *this;
    }

    ArrayProxy<T, N> operator[] (size_t index)
    {
        return ArrayProxy<T, N>(m_storage, index);
    }
};

class ControllerParser
{
    ProviderParserContext m_context;
    ProviderParser m_providerParser;
    
public:
    ControllerParser(ProviderParserContext context) :
        m_context(context),
        m_providerParser(context)
    { }

    Array<std::unique_ptr<ValueProvider>, 4> parseColor(const tinyxml2::XMLElement& xml);
    
    Array<std::unique_ptr<ValueProvider>, 2> parsePosition(const tinyxml2::XMLElement& xml);
    
    Array<std::unique_ptr<ValueProvider>, 2> parseScale(const tinyxml2::XMLElement& xml);
    
    std::unique_ptr<ValueProvider> parseRotation(const tinyxml2::XMLElement& xml);

    std::unique_ptr<ValueProvider> findController(
        const tinyxml2::XMLElement& xml,
        const std::string& childName,
        const std::string& propertyName,
        const std::string& propertyValue);
};

#endif // CONTROLLER_PARSER_HPP
