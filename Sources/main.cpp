#include <iostream>
#include <dlfcn.h>
#include <unistd.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

inline bool file_exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    void *lib_handle = nullptr;

    if (file_exists("./libpizdryk.so"))
    {
        lib_handle = dlopen("./libpizdryk.so", RTLD_LAZY);
        if (!lib_handle)
        {
            std::cout<< "Failed to load libpizdryk" << std::endl;
        }
    }
    else
    {
        std::cout<< "Missing libpizdryk" << std::endl;
    }

    typedef void (*listen_funcptr)();
    listen_funcptr listen = nullptr;

    if (lib_handle)
    {
        listen = reinterpret_cast<listen_funcptr> (dlsym(lib_handle, "listen"));
        if (listen)
        {
            listen();
        }
        else
        {
            std::cout << "Failed to find function \"listen\"" << std::endl;
        }
        
    }

    QPushButton Goodbye( "Goodbye world!", 0 );
    QPushButton callbutton( "call", 0 );
    Goodbye.resize( 160, 30 );
    callbutton.resize(100,30);
    Goodbye.show();
    callbutton.show();
    Goodbye.setObjectName("baton");
    callbutton.setObjectName("Call");

    QObject::connect(&Goodbye, &QPushButton::clicked, [&] () 
        {
            std::cout << "Button clicked" << std::endl;
            app.quit();
        });
    QObject::connect(&callbutton, &QPushButton::clicked, [&] () 
        {
            std::cout << "Call button clicked" << std::endl;
        });
    app.exec();

    if (lib_handle)
    {
        dlclose(lib_handle);
    }
}
