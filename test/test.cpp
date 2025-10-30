#include <igloo/igloo.h>
#include <Application.h>

using namespace igloo;


Context(AppTest){
    Application* app = new Application();

    Spec(Test_Window_Width) {
        Assert::That(app->window->w(), Equals(400));
    }

    Spec(Test_Window_Height) {
        Assert::That(app->window->h(), Equals(400));
    }

};

int main(int argc, const char *argv[]) {
    return TestRunner::RunAllTests(argc, argv);
}
