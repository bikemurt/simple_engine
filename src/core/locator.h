#ifndef LOCATOR_H
#define LOCATOR_H

// issues with this
// service requires that each "service" class is an abstract class, with a "null service" implementation
// extra bloat, overhead
// should try dependency injection instead and see how that goes

class Renderer;

class Locator {

public:

    static Renderer* getRenderer();
    static void provideRenderer(Renderer* service);

private:

    static Renderer* rendererService;
};

#endif