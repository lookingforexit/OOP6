#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <fstream>

class IObserver
{
public:
    virtual void msg_kill(const std::string& killer, const std::string& victim) = 0;

    virtual ~IObserver() = default;
};

class IConsoleObserver final: public IObserver
{
public:
    void msg_kill(const std::string& killer, const std::string& victim) override;
};

class FileObserver final: public IObserver
{
private:
    std::ofstream file;
public:
    FileObserver();
    ~FileObserver() override;
public:
    void msg_kill(const std::string& killer, const std::string& victim) override;
};

#endif //OBSERVER_H