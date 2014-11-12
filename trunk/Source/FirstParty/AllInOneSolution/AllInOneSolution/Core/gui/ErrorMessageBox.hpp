#pragma once

#ifndef ERRORMESSAGEBOX_HPP
#define ERRORMESSAGEBOX_HPP

#ifdef WINDOWS

#include <Windows.h>
#include <iostream>

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
         return MessageBox(0, m_message.c_str(), "Error:", MB_OK | MB_ICONSTOP);
    }
};

#endif  // WINDOWS

#endif
