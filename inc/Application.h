#ifndef APPLICATION_H
#define APPLICATION_H

#include <bobcat_ui/all.h>

class Application: public bobcat::Application_{
    // Declare your app components here
    bobcat::Window *window;


    // Also declare functions here (signature only)


public:
    Application();

    friend struct ::AppTest;
};

#endif