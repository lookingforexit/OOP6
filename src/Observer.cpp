#include "Observer.h"

#include <iostream>

void IConsoleObserver::msg_kill(const std::string& killer, const std::string& victim)
{
    std::cout << killer << " killed " << victim << std::endl;
}

FileObserver::FileObserver()
{
    file.open("../logs.txt", std::ios::app);
}

FileObserver::~FileObserver()
{
    if (file.is_open())
    {
        file.close();
    }
}

void FileObserver::msg_kill(const std::string& killer, const std::string& victim)
{
    if (file.is_open())
    {
        file << killer << " killed " << victim << std::endl;
    }
}