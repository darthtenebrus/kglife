//
// Created by esorochinskiy on 30.09.23.
//

#ifndef KGLIFE_LOADGAMEEXCEPTION_H
#define KGLIFE_LOADGAMEEXCEPTION_H


#include <exception>
#include <QString>


class LoadGameException : public std::exception {

public:
    LoadGameException(const std::string &message) : message_(message) {}

    virtual const char* what() const throw() {

        return message_.c_str();
    }

private:
    std::string message_;
};


#endif //KGLIFE_LOADGAMEEXCEPTION_H
