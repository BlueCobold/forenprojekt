#pragma once

#ifndef ERRORMESSAGEBOX_HPP
#define ERRORMESSAGEBOX_HPP

#ifdef WINDOWS
#include <Windows.h>
#include <string>
#elif defined(IOS) || defined(OSX)
#include "../MacHelper.hpp"
#elif defined(ANDROID)
#include "../AndroidHelper.hpp"
#endif

class ErrorMessageBox
{
private:
    std::string m_message;

public:
    ErrorMessageBox(std::string message) :
        m_message(message)
    {
        show();
    }

    int show()
    {
#ifdef WINDOWS
         return MessageBox(0, m_message.c_str(), "Error:", MB_OK | MB_ICONSTOP);
#elif defined(IOS) || defined(OSX)
        showErrorApple(m_message);
        return 1;
#elif defined(ANDROID)
        showErrorAndroid(m_message);
        return 1;
#endif
    }
};

#endif
